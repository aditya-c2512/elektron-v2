#pragma once

#include "ConstantBuffer.h"
#include "../drawables/Drawable.h"
#include <DirectXMath.h>

class TransformCBuf : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX modelViewProj;
	};
public:
	TransformCBuf(ElektronGFX& gfx, const Drawable& parent);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVCBuf;
	//static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVCBuf;
	const Drawable& parent;
};

