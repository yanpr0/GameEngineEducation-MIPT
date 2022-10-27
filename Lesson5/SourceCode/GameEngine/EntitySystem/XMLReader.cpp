#define _CRT_SECURE_NO_WARNINGS

#include <map>
#include <string_view>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>

#include "XMLReader.h"
#include "ecsPhys.h"
#include "ecsScript.h"
#include "ecsMesh.h"


static std::map<std::string_view, void(*)(flecs::entity, const char*)> component_callbacks =
{
    {"position", +[](flecs::entity e, const char* str)
    {
        float x, y, z;
        std::sscanf(str, "%f,%f,%f", &x, &y, &z);
        e.set(Position{ x, y, z });
    }},
    {"velocity", +[](flecs::entity e, const char* str)
    {
        float x, y, z;
        std::sscanf(str, "%f,%f,%f", &x, &y, &z);
        e.set(Velocity{ x, y, z });
    }},
    {"gravity", +[](flecs::entity e, const char* str)
    {
        float x, y, z;
        std::sscanf(str, "%f,%f,%f", &x, &y, &z);
        e.set(Gravity{ x, y, z });
    }},
    {"bounce_plane", +[](flecs::entity e, const char* str)
    {
        float x, y, z, w;
        std::sscanf(str, "%f,%f,%f,%f", &x, &y, &z, &w);
        e.set(BouncePlane{ x, y, z, w });
    }},
    {"bounciness", +[](flecs::entity e, const char* str)
    {
        float val;
        std::sscanf(str, "%f", &val);
        e.set(Bounciness{ val });
    }},
    {"friction", +[](flecs::entity e, const char* str)
    {
        float val;
        std::sscanf(str, "%f", &val);
        e.set(FrictionAmount{ val });
    }},
    {"shiver", +[](flecs::entity e, const char* str)
    {
        float val;
        std::sscanf(str, "%f", &val);
        e.set(ShiverAmount{ val });
    }},
    {"scripts", +[](flecs::entity e, const char* str)
    {
        std::size_t n = std::strlen(str);
        char* script = new char[n + 1];
        std::memcpy(script, str, n + 1);
        e.set(Scripts{ script });
    }},
    {"cube_mesh", +[](flecs::entity e, const char*)
    {
        e.add<CubeMesh>();
    }}
};

XMLReader::XMLReader(const char* filename, flecs::world& ecs) : ecs_{ ecs }
{
    static const std::string xml_path_base = "../../../Assets/Entities/";

    doc_.LoadFile((xml_path_base + filename).c_str());
    auto root = doc_.FirstChildElement("entities");
    if (root != nullptr)
    {
        next_ent_ = root->FirstChildElement("entity");
    }
}

std::optional<flecs::entity> XMLReader::NextEntity()
{
    if (next_ent_ == nullptr)
    {
        return {};
    }

    auto ent = ecs_.entity();

    const char* name = nullptr;
    if (next_ent_->QueryStringAttribute("name", &name) == tinyxml2::XML_SUCCESS)
    {
        ent.set_name(name);
    }

    auto comp = next_ent_->FirstChildElement();
    while (comp != nullptr)
    {
        if (auto f = component_callbacks[comp->Name()])
        {
            f(ent, comp->GetText());
        }
        comp = comp->NextSiblingElement();
    }

    next_ent_ = next_ent_->NextSiblingElement("entity");

    return ent;
}