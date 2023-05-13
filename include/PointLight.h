#pragma once

#include "ElektronGFX.h"
#include "drawables/SolidSphere.h"
#include "bindables/ConstantBuffer.h"

class PointLight
{
public:
	PointLight(ElektronGFX& gfx);
	void SpawnControlWindow() noexcept;
	void Draw(ElektronGFX& gfx) const noexcept;
	void Bind(ElektronGFX& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct PointLightBuffer
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient_color;
		alignas(16) DirectX::XMFLOAT3 diffuse_color;
		float intensity_diffuse;
		float att_const;
		float att_lin;
		float att_quad;
	};

	PointLightBuffer plBuf = {
				{0.0f,0.0f,0.0f},
				{0.282f, 0.302f, 0.38f},
				{1.0f,1.0f,1.0f},
				1.0f,1.0f,1.0f,1.0f
	};;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightBuffer> pointCBuf;
};