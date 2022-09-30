#pragma once

#include "Common.h"
#include "RenderProxy.h"
#include "InputHandler.h"

struct GameTickData {
public:
	float delta_time;
	float total_time;
	std::bitset<eIC_Max> input_state;
};

class GameObject
{
public:
	GameObject() = default;

	virtual ~GameObject()
	{
		delete m_pRenderProxy;
	}

	virtual void Update(GameTickData* data) {}

	void SetPosition(float x, float y, float z);

	RenderProxy* const GetRenderProxy() { return m_pRenderProxy; }
protected:
	RenderProxy* m_pRenderProxy;

	float m_vPosition[3];
};

