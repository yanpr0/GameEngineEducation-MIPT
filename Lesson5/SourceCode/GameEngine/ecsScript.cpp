#include "ecsScript.h"
#include "IScriptProxy.h"
#include "flecs.h"

void register_ecs_script_systems(flecs::world& ecs)
{
    ecs.system<ScriptProxyPtr>()
        .each([&](ScriptProxyPtr& script)
            {
                script.ptr->ProcessScript();
            });
}
