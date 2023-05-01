#pragma once

#include "DrawableStatic.h"
#include "Mesh.h"

class Model : public DrawableStatic<Model>
{
public:
	Model(ElektronGFX& gfx, std::string assetPath);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;
public:
	float angle;
	std::vector<Mesh> meshes;
	DirectX::XMFLOAT3X3 modelTransform;
};