#pragma once

#include "DrawableStatic.h"

class SkySphere : public DrawableStatic<SkySphere>
{
public:
	SkySphere(ElektronGFX& gfx);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	DirectX::XMFLOAT3X3 modelTransform;
};

