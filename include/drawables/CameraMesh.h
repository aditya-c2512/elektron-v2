#pragma once
#include "DrawableStatic.h"

struct CamVertex
{
	DirectX::XMFLOAT3 pos;
};

class CameraMesh : public DrawableStatic<CameraMesh>
{
public:
	CameraMesh(ElektronGFX& gfx);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRotation(DirectX::XMFLOAT3 rot) noexcept;
	DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rot = { 0.0f,0.0f,0.0f };
};

