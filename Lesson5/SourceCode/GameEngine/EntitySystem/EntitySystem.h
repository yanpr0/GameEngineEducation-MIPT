#pragma once

#include <type_traits>

#include "flecs.h"
#include "RenderEngine.h"
#include "IScriptSystem.h"
#include "../InputHandler.h"
#include "XMLReader.h"


class EntitySystem
{
public:
	EntitySystem() = delete;
	EntitySystem(RenderEngine* renderEngine, InputHandler* inputHandler, IScriptSystem* scriptSystem);

	void SpawnEntities(const char* filename);

	template <typename F>
	std::enable_if_t<std::is_invocable_v<F, flecs::entity>, void> SpawnEntities(const char* filename, F&& callback)
	{
		XMLReader reader{ filename, ecs };

		std::optional<flecs::entity> ent;
		while (ent = reader.NextEntity())
		{
			callback(ent);
		}
	}

	void Update();
private:
	flecs::world ecs;
};
