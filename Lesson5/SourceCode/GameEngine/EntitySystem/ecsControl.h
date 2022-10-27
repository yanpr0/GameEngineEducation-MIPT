#pragma once
#include "flecs.h"

struct Controllable {};
struct JumpSpeed { float val; };

typedef float Speed;

void register_ecs_control_systems(flecs::world &ecs);

