#pragma once
#include "flecs.h"

struct Controllable {};
struct JumpSpeed { float val; };

struct Shooting {};
struct Reloading { float left, max; };

void register_ecs_control_systems(flecs::world &ecs);
