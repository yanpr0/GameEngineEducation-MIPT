#pragma once
#include <vector>
#include "RenderProxy.h"

struct InputHandlerPtr
{
  class InputHandler* ptr;
};

struct RenderEnginePtr
{
  class RenderEngine* ptr;
};

using FreeProxies = std::vector<RenderProxy*>;
