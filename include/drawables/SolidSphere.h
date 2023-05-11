#pragma once
#include "DrawableStatic.h"

class SolidSphere : public DrawableStatic<SolidSphere>
{
public:
	SolidSphere(ElektronGFX& gfx, float radius);
	void Update(float dt) noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void SpawnControlWindow() noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};