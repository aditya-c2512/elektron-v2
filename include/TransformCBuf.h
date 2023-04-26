#pragma once

#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCBuf : public Bindable
{
public:
	TransformCBuf(ElektronGFX& gfx, const Drawable& parent);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVCBuf;
	const Drawable& parent;
};

