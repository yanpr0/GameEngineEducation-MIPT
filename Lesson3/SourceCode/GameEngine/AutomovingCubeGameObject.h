#pragma once

#include <cmath>

#include "CubeGameObject.h"

class AutomovingCubeGameObject final : public CubeGameObject
{
public:
	virtual void Update(GameTickData* data) override
	{
		auto [dt, t, input_state] = *data;
		auto [x, y, z] = m_vPosition;

		float delta = std::sin(t) * 5;
		y += delta - prev_delta_;
		prev_delta_ = delta;

		SetPosition(x, y, z);
	}

private:
	float prev_delta_ = 0.0f;
};
