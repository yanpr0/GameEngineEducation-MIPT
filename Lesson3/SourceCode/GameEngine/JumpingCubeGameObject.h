#pragma once

#include "CubeGameObject.h"

class JumpingCubeGameObject final : public CubeGameObject
{
public:
	// JumpingCubeGameObject(float start_velocity) : start_velocity_{start_velocity} {}

	virtual void Update(GameTickData* data) override
	{
		auto [dt, t, input_state] = *data;
		auto [x, y, z] = m_vPosition;

		if (z == 0.0)
		{
			velocity_ = start_velocity_;
		}

		z += velocity_ * dt + g * dt * dt / 2.0f;
		velocity_ += g * dt;

		if (z < 0.0f)
		{
			z = 0.0f;
		}

		SetPosition(x, y, z);
	}

private:
	static constexpr float g = -9.8f; // scale
	const float start_velocity_ = 20.0f;
	float velocity_;
};
