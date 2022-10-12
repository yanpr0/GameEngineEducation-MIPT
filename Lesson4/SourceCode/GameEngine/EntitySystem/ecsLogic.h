#pragma once
#include "flecs.h"

struct Bullet {};
struct Collidable {};
struct HitRadius { float val; };

struct Alive {};
struct Dead {};
struct Respawn {};

struct ActiveBullet { bool val; };
struct Magazine { int left, max; };
struct BulletOrigin { Magazine* mag; };
struct BulletSpeed { float val; };
struct SetTTLOnFloor { float ttl; };
struct TTL { float left, max; };

void register_ecs_logic_systems(flecs::world& ecs);
