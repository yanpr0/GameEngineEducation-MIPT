#pragma once

#include "InputHandler.h"

#include "CubeGameObject.h"

class ControllableCubeGameObject final : public CubeGameObject
{
public:
	virtual void Update(GameTickData* data) override
	{
		auto [dt, t, input_state] = *data;
		auto [x, y, z] = m_vPosition;

		float velocity = 0.0f;
		if (input_state.test(eIC_GoLeft))
		{
			velocity += 3.0f;
		}
		if (input_state.test(eIC_GoRight))
		{
			velocity -= 3.0f;
		}
		x += velocity * dt;

		SetPosition(x, y, z);
	}
};
