#pragma once

#include "DrawableStatic.h"
#include "Mesh.h"

class Model : public DrawableStatic<Model>
{
public:
	Model(ElektronGFX& gfx, std::string assetPath);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;
	void SpawnControlWindow() noexcept override;
public:
	std::vector<Mesh> meshes;
	DirectX::XMFLOAT3X3 modelTransform;
private:
	float pos[3] = { 0,0,0 };
	float angles[3] = { 0,0,0 };
	DirectX::XMFLOAT3 material_color = { 0.639f, 0.961f, 0.729f };
	float intensity_specular = 0.6f;
	float power_specular = 30.0f;
};