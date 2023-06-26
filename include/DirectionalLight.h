#pragma once
#include "ElektronGFX.h"
#include "bindables/ConstantBuffer.h"

class DirectionalLight
{
public:
	DirectionalLight(ElektronGFX& gfx);
	void SpawnControlWindow() noexcept;
	void Bind(ElektronGFX& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct DirLightBuffer
	{
		alignas(16) DirectX::XMFLOAT3 lightDir;
	};
	DirLightBuffer dlBuf = {
				{0.0f,0.0f,1.0f},
	};
	mutable PixelConstantBuffer<DirLightBuffer> dirCBuf;
};

