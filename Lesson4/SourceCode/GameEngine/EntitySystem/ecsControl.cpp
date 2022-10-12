#include "ecsControl.h"
#include "ecsLogic.h"
#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "../InputHandler.h"
#include "RenderThread.h"
#include "RenderEngine.h"
#include "CubeRenderProxy.h"

void register_ecs_control_systems(flecs::world &ecs)
{
  static auto inputQuery = ecs.query<InputHandlerPtr>();
  static auto freeProxiesQuery = ecs.query<FreeProxies>();

  ecs.system<Velocity, const Speed, const Controllable>()
    .each([&](flecs::entity e, Velocity &vel, const Speed &spd, const Controllable&)
    {
      inputQuery.each([&](InputHandlerPtr& input)
      {
        float deltaVel = 0.f;
        if (input.ptr->GetInputState().test(eIC_GoLeft))
          deltaVel -= spd;
        if (input.ptr->GetInputState().test(eIC_GoRight))
          deltaVel += spd;
        vel.x += deltaVel * e.delta_time();
      });
    });

  ecs.system<const Position, Velocity, const Controllable, const BouncePlane, const JumpSpeed>()
    .each([&](const Position &pos, Velocity &vel, const Controllable&, const BouncePlane &plane, const JumpSpeed &jump)
    {
      inputQuery.each([&](InputHandlerPtr& input)
      {
        constexpr float planeEpsilon = 0.1f;
        if (plane.x*pos.x + plane.y*pos.y + plane.z*pos.z < plane.w + planeEpsilon)
          if (input.ptr->GetInputState().test(eIC_Jump))
            vel.y = jump.val;
      });
    });

  static auto renderQuery = ecs.query<RenderEnginePtr>();

  ecs.system<const Position, const Velocity, const Controllable, const Shooting, Magazine>()
      .each([&](flecs::entity e, const Position& pos, const Velocity& vel, const Controllable&, const Shooting&, Magazine& mag)
          {
              inputQuery.each([&](InputHandlerPtr& input)
                  {
                      if (input.ptr->GetInputState().test(eIC_Shoot))
                      {
                          freeProxiesQuery.each([&](FreeProxies& free_proxies)
                              {
                                  auto bullet = e.world().entity()
                                      .set(Position{ pos.x, pos.y + 2.f, pos.z })
                                      .set(Velocity{ vel.x + 10.f, vel.y, vel.z })
                                      .set(Gravity{ 0.f, -9.8065f, 0.f })
                                      .set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
                                      .set(Bounciness{ 1.f })
                                      .set(FrictionAmount{ 0.9f })
                                      .set(SetTTLOnFloor{ 5.f })
                                      .set(HitRadius{ 0.15f })
                                      .set(BulletOrigin{ &mag })
                                      .set(ActiveBullet{ true })
                                      .add<Bullet>();

                                  if (free_proxies.empty())
                                  {
                                      renderQuery.each([&](RenderEnginePtr& re)
                                          {
                                              RenderProxy* renderProxy = new CubeRenderProxy;
                                              re.ptr->GetRT()->EnqueueCommand(RC_CreateTinyCubeRenderObject, renderProxy);
                                              bullet.set(RenderProxyPtr{ renderProxy });
                                          });
                                  }
                                  else
                                  {
                                      auto proxy = free_proxies.back();
                                      free_proxies.pop_back();
                                      proxy->SetVisibility(true);
                                      bullet.set(RenderProxyPtr{ proxy });
                                  }
                              });

                          --mag.left;
                          if (mag.left == 0)
                          {
                              e.remove<Shooting>();
                              float reloading_time = 2.f;
                              e.set(Reloading{ reloading_time, reloading_time });
                          }
                      }
                  });
          });

  ecs.system<Reloading, Magazine>()
      .each([&](flecs::entity e, Reloading& re, Magazine& mag)
          {
              re.left -= e.delta_time();
              if (re.left <= 0.f)
              {
                  e.add<Shooting>();
                  mag.left = mag.max;
                  e.remove<Reloading>();
              }
          });
}

