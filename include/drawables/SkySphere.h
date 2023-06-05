#pragma once

#include "DrawableStatic.h"
#include "../bindables/ElekTexMap.h"

class SkySphere : public DrawableStatic<SkySphere>
{
public:
	SkySphere(ElektronGFX& gfx, ElekTexMap& elekTexMap);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	DirectX::XMFLOAT3X3 modelTransform;
};

