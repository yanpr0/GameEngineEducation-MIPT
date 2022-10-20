#include "ecsScript.h"
#include "IScriptProxy.h"
#include "flecs.h"

void register_ecs_script_systems(flecs::world& ecs)
{
    ecs.system<ScriptProxyPtr>()
        .each([&](flecs::entity e, ScriptProxyPtr& script)
            {
                e.remove<ScriptProxyPtr>();
                //script.ptr->ProcessScript();
            });
}
