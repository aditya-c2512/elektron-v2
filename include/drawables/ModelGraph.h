#pragma once
#include "DrawableStatic.h"
#include "../bindables/Bindables.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

class Mesh : public DrawableStatic<Mesh>
{
public:
	Mesh(ElektronGFX& gfx, std::vector<std::unique_ptr<Bindable>> bindPtrs, std::string name);
	void Update(float dt) noexcept override;
	void Draw(ElektronGFX& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
	DirectX::XMMATRIX GetTransform() const noexcept override;
	std::string GetName() const noexcept;
private:
	mutable DirectX::XMFLOAT4X4 transform;
	std::string name;
};

class Node
{
	friend class ModelGraph;
public:
	Node(std::string name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept;
	void Draw(ElektronGFX& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
	void RenderNodeTree() noexcept;
private:
	void AddChild(std::unique_ptr<Node> pChild) noexcept;
private:
	std::string name;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
};

class ModelGraph
{
public:
	ModelGraph(ElektronGFX& gfx, const std::string fileName);
	void Draw(ElektronGFX& gfx) const;
	void SpawnModelGraphControlWindow() noexcept;
private:
	static std::unique_ptr<Mesh> ParseMesh(ElektronGFX& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	std::unique_ptr<Node> ParseNode(const aiNode& node);
private:
	std::string name;
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};

