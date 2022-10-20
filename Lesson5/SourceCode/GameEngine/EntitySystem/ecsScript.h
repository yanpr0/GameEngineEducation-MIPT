#pragma once
#include "flecs.h"

struct ScriptProxyPtr
{
	class IScriptProxy* ptr;
};

void register_ecs_script_systems(flecs::world& ecs);
