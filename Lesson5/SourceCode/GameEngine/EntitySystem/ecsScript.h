#pragma once
#include "flecs.h"


struct ScriptSystemPtr
{
	class IScriptSystem* ptr;
};

struct Scripts
{
	char* scripts; // whitespace-separated script filenames
};

void register_ecs_script_systems(flecs::world& ecs);
