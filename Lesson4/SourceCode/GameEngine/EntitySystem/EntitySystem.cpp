#include "EntitySystem.h"
#include "ecsSystems.h"
#include "ecsMesh.h"
#include "ecsControl.h"
#include "ecsLogic.h"
#include "ecsPhys.h"
#include "RenderThread.h"
#include "CubeRenderProxy.h"

EntitySystem::EntitySystem(RenderEngine* renderEngine, InputHandler* inputHandler)
{
    ecs.entity("inputHandler")
        .set(InputHandlerPtr{ inputHandler });
    ecs.entity("renderEngine")
        .set(RenderEnginePtr{ renderEngine });

    ecs.entity("freeProxies")
        .set(FreeProxies{});

    register_ecs_mesh_systems(ecs);
    register_ecs_control_systems(ecs);
    register_ecs_phys_systems(ecs);
    register_ecs_logic_systems(ecs);

    constexpr int magazine_size = 3;

    auto cubeControl = ecs.entity()
        .set(Position{ -3.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 0.f, 0.f })
        .set(Speed{ 10.f })
        .set(FrictionAmount{ 0.9f })
        .set(JumpSpeed{ 15.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
        .set(Bounciness{ 0.3f })
        .set(Magazine{ magazine_size, magazine_size })
        .add<Controllable>()
        .add<CubeMesh>()
        .add<Shooting>();

    // collidable cubes

    ecs.entity()
        .set(Position{ 5.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 3.f, 0.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
        .set(Bounciness{ 0.8f })
        .set(HitRadius{ 1.f })
        .add<CubeMesh>()
        .add<Alive>()
        .add<Collidable>();

    ecs.entity()
        .set(Position{ 1.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 3.f, 0.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
        .set(Bounciness{ 0.8f })
        .set(HitRadius{ 1.f })
        .add<CubeMesh>()
        .add<Alive>()
        .add<Collidable>();

    ecs.entity()
        .set(Position{ -1.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 3.f, 0.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 4.f })
        .set(Bounciness{ 0.8f })
        .set(HitRadius{ 1.f })
        .add<CubeMesh>()
        .add<Alive>()
        .add<Collidable>();
}

void EntitySystem::Update()
{
    ecs.progress();
}