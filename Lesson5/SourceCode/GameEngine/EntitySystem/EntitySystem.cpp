#include "EntitySystem.h"
#include "ecsSystems.h"
#include "ecsMesh.h"
#include "ecsControl.h"
#include "ecsPhys.h"
#include "ecsScript.h"
#include "ScriptProxy.h"
#include "ScriptSystem.h"
#include "../InputHandler.h"

EntitySystem::EntitySystem(RenderEngine* renderEngine, InputHandler* inputHandler, IScriptSystem* scriptSystem)
{
    ecs.entity("inputHandler")
        .set(InputHandlerPtr{ inputHandler });
    ecs.entity("renderEngine")
        .set(RenderEnginePtr{ renderEngine });

    register_ecs_mesh_systems(ecs);
    register_ecs_control_systems(ecs);
    register_ecs_phys_systems(ecs);
    register_ecs_script_systems(ecs);

    auto cubeControl = ecs.entity()
        .set(Position{ 0.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 0.f, 0.f })
        //.set(Speed{ 10.f })
        .set(FrictionAmount{ 0.9f })
        .set(JumpSpeed{ 10.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
        .set(Bounciness{ 0.3f })
        .add<Controllable>()
        .add<CubeMesh>();
        
    auto script = static_cast<CScriptProxy*>(scriptSystem->CreateProxy("../../../Assets/scripts/movable.lua"));
    script->AddType<Velocity>("Velocity",
        "x", &Velocity::x,
        "y", &Velocity::y,
        "z", &Velocity::z
        );
    script->AddVar("vel", cubeControl.get_mut<Velocity>());
    script->AddFunc("delta_time", [ecs=&ecs]() { return ecs->delta_time(); });
    script->AddFunc("left_move", [inputHandler]() { return inputHandler->GetInputState().test(eIC_GoLeft); });
    script->AddFunc("right_move", [inputHandler]() { return inputHandler->GetInputState().test(eIC_GoRight); });
    script->AddFunc("set_vel_x",
        [ecs = &ecs, id = cubeControl.id()](float val) { ecs->entity(id).get_mut<Velocity>()->x += val; });

    cubeControl.set(ScriptProxyPtr{ script });

    auto cubeMoving = ecs.entity()
        .set(Position{ 0.f, 0.f, 0.f })
        .set(Velocity{ 0.f, 3.f, 0.f })
        .set(Gravity{ 0.f, -9.8065f, 0.f })
        .set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
        .set(Bounciness{ 1.f })
        .add<CubeMesh>();
}

void EntitySystem::Update()
{
    ecs.progress();
}