#include <sstream>
#include <string>
#include <string_view>

#include "ecsScript.h"
#include "ScriptProxy.h"
#include "ecsSystems.h"
#include "ecsMesh.h"
#include "ecsControl.h"
#include "ecsPhys.h"
#include "RenderEngine.h"
#include "ScriptProxy.h"
#include "IScriptSystem.h"
#include "../InputHandler.h"

#include "flecs.h"

void init_script_env(std::string_view script, flecs::world& ecs, flecs::entity e, RenderEngine* renderEngine, InputHandler* inputHandler, IScriptSystem* scriptSystem)
{
    auto proxy = static_cast<CScriptProxy*>(scriptSystem->CreateProxy(script.data()));

    ///////////////////////////////////////////////////////////////////////////

    proxy->AddType<Position>("Position",
        "x", &Position::x,
        "y", &Position::y,
        "z", &Position::z
        );
    proxy->AddType<Velocity>("Velocity",
        "x", &Velocity::x,
        "y", &Velocity::y,
        "z", &Velocity::z
        );
    proxy->AddType<Gravity>("Gravity",
        "x", &Gravity::x,
        "y", &Gravity::y,
        "z", &Gravity::z
        );
    proxy->AddType<BouncePlane>("BouncePlane",
        "x", &BouncePlane::x,
        "y", &BouncePlane::y,
        "z", &BouncePlane::z,
        "w", &BouncePlane::w
        );
    proxy->AddType<Bounciness>("Bounciness",
        "val", &Bounciness::val
        );
    proxy->AddType<ShiverAmount>("ShiverAmount",
        "val", &ShiverAmount::val
        );
    proxy->AddType<FrictionAmount>("FrictionAmount",
        "val", &FrictionAmount::val
        );

    ///////////////////////////////////////////////////////////////////////////

    proxy->AddFunc("GetInputState", [inputHandler](EInputCommand cmd) { return inputHandler->GetInputState().test(cmd); });
    proxy->AddVar("eIC_GoLeft", eIC_GoLeft);
    proxy->AddVar("eIC_GoRight", eIC_GoRight);
    proxy->AddVar("eIC_Jump", eIC_Jump);

    ///////////////////////////////////////////////////////////////////////////
    proxy->AddType<flecs::entity>("entity",
        "GetPosition", [](flecs::entity& e) { return e.get_mut<Position>(); },
        "GetVelocity", [](flecs::entity& e) { return e.get_mut<Velocity>(); },
        "GetBouncePlane", [](flecs::entity& e) { return e.get_mut<BouncePlane>(); }
        // ...
        );

    proxy->AddType<flecs::entity_t>("entity_t");
    proxy->AddVar("entity_id", e.id());

    proxy->AddType<flecs::world>("world",
        "NewEntity", [](flecs::world& ecs) { return ecs.entity(); },
        "GetEntity", [](flecs::world& ecs, flecs::entity_t id) { return ecs.entity(id); }
        );
    proxy->AddVar("ecs", &ecs);

    ///////////////////////////////////////////////////////////////////////////

    proxy->AddFunc("delta_time", [ecs = &ecs]() { return ecs->delta_time(); });   
}

void register_ecs_script_systems(flecs::world& ecs)
{
    static auto inputQuery = ecs.query<InputHandlerPtr>();
    static auto renderQuery = ecs.query<RenderEnginePtr>();
    static auto scriptQuery = ecs.query<ScriptSystemPtr>();

    ecs.system<Scripts>()
        .each([&](flecs::entity e, Scripts& scripts)
            {
                inputQuery.each([&](InputHandlerPtr input)
                    {
                        renderQuery.each([&](RenderEnginePtr re)
                            {
                                scriptQuery.each([&](ScriptSystemPtr scriptSystem)
                                    {
                                        std::string s;
                                        std::istringstream is{ scripts.scripts };
                                        while (is >> s)
                                        {
                                            init_script_env("../../../Assets/scripts/" + s, ecs, e, re.ptr, input.ptr, scriptSystem.ptr);
                                        }
                                        delete[] scripts.scripts;
                                    });
                            });
                    });
                e.remove<Scripts>();
            });
}
