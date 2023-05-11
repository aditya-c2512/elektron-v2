#pragma once

#include "ElektronGFX.h"
#include "drawables/SolidSphere.h"
#include "bindables/ConstantBuffer.h"

class PointLight
{
public:
	PointLight(ElektronGFX& gfx);
	void SpawnControlWindow() noexcept;
	//void Draw(ElektronGFX& gfx) const noexcept;
	void Bind(ElektronGFX& gfx) const noexcept;
private:
	struct PointLightBuffer
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};

	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	//mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightBuffer> pointCBuf;
};