#pragma once

#include <optional>

#include "flecs.h"
#include "tinyxml2.h"

class XMLReader
{
public:
    XMLReader(const char* filename, flecs::world& ecs);

    std::optional<flecs::entity> NextEntity();

private:
    flecs::world& ecs_;
    tinyxml2::XMLDocument doc_;
    tinyxml2::XMLElement* next_ent_ = nullptr;
};
