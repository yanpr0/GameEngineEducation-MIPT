#pragma once

#include "../RenderObject.h"

namespace TinyCube
{
	struct PosColorVertex
	{
		float x;
		float y;
		float z;
		uint32_t abgr;
	};
};

class RENDERENGINE_API TinyCubeRenderObject final : public RenderObject
{
public:
	TinyCubeRenderObject(RenderProxy * pRenderProxy);

	virtual void* GetVertices() const override { return (void*)s_cubeVertices; }
	virtual UINT32 GetVerticesSize() const override { return sizeof(s_cubeVertices); }
	virtual void* GetIndices() const override { return (void*)s_cubeTriList; }
	virtual UINT32 GetIndicesSize() const override { return sizeof(s_cubeTriList); }
	virtual std::string GetVsShaderName() const override { return "vs_cubes"; }
	virtual std::string GetPsShaderName() const override { return "fs_cubes"; }

private:
	inline static const TinyCube::PosColorVertex s_cubeVertices[] =
	{
		{-0.15f,  0.15f,  0.15f, 0xff000000 },
		{ 0.15f,  0.15f,  0.15f, 0xff0000ff },
		{-0.15f, -0.15f,  0.15f, 0xff00ff00 },
		{ 0.15f, -0.15f,  0.15f, 0xff00ffff },
		{-0.15f,  0.15f, -0.15f, 0xffff0000 },
		{ 0.15f,  0.15f, -0.15f, 0xffff00ff },
		{-0.15f, -0.15f, -0.15f, 0xffffff00 },
		{ 0.15f, -0.15f, -0.15f, 0xffffffff },
	};

	inline static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,
	};
};
