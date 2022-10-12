#include <algorithm>
#include <cmath>

#include "ecsLogic.h"
#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "RenderThread.h"
#include "RenderEngine.h"
#include "CubeRenderProxy.h"

void register_ecs_logic_systems(flecs::world& ecs)
{
    static auto bulletQuery = ecs.query<const Bullet, ActiveBullet, const HitRadius, const Position, const BulletOrigin>();
    static auto freeProxiesQuery = ecs.query<FreeProxies>();

    ecs.system<const Position, const BouncePlane, const SetTTLOnFloor>()
        .each([&](flecs::entity e, const Position& pos, const BouncePlane& plane, const SetTTLOnFloor& ttl)
            {
                constexpr float planeEpsilon = 0.1f;
                if (plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon)
                {
                    e.set(TTL{ ttl.ttl, ttl.ttl });
                    e.remove<SetTTLOnFloor>();
                }
            });

    ecs.system<ActiveBullet, TTL, RenderProxyPtr>()
        .each([&](flecs::entity e, ActiveBullet& active, TTL& ttl, RenderProxyPtr& renderProxy)
            {
                ttl.left -= e.delta_time();
                if (ttl.left <= 0.f)
                {
                    active.val = false;
                }
            });

    ecs.system<const Dead, const Collidable, RenderProxyPtr>()
        .each([&](flecs::entity e, const Dead&, const Collidable&, RenderProxyPtr& renderProxy)
            {
                renderProxy.ptr->SetVisibility(false);
                e.remove<Dead>();
                e.add<Respawn>();
            });

    ecs.system<const Collidable, const Alive, const HitRadius, const Position>()
        .each([&](flecs::entity e, const Collidable&, const Alive&, const HitRadius& rad, const Position& pos)
            {
                bulletQuery.each([&](flecs::entity b, const Bullet&, ActiveBullet& active, const HitRadius& bullet_rad, const Position& bullet_pos, const BulletOrigin& mag)
                    {
                        auto x = bullet_pos.x - pos.x;
                        auto y = bullet_pos.y - pos.y;
                        auto z = bullet_pos.z - pos.z;
                        if (std::sqrt(x * x + y * y + z * z) <= rad.val + bullet_rad.val)
                        {
                            e.remove<Alive>();
                            e.add<Dead>();
                            ++mag.mag->max; // increasing magazine
                            active.val = false; // bullet is no longer in use
                        }
                    });
            });

    ecs.system<const Respawn, const Collidable, Position, Velocity, RenderProxyPtr>()
        .each([&](flecs::entity e, const Respawn&, const Collidable&, Position& pos, Velocity& v, RenderProxyPtr& renderProxy)
            {
                pos.y += 8.f;
                v = Velocity{ 0.f, 0.f, 0.f };
                renderProxy.ptr->SetVisibility(true);
                e.remove<Respawn>();
                e.add<Alive>();
            });

    ecs.system<const Bullet, const ActiveBullet, RenderProxyPtr>()
        .each([&](flecs::entity e, const Bullet&, const  ActiveBullet& active, RenderProxyPtr& renderProxy)
            {
                if (!active.val)
                {
                    freeProxiesQuery.each([&](FreeProxies& free_proxies)
                        {
                            free_proxies.push_back(renderProxy.ptr);
                        });
                    renderProxy.ptr->SetVisibility(false);
                    e.destruct();
                }
            });
}

