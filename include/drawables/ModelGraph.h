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
	Mesh(ElektronGFX& gfx, std::vector<std::unique_ptr<Bindable>> bindPtrs);
	void Update(float dt) noexcept override;
	void Draw(ElektronGFX& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
	DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node
{
	friend class ModelGraph;
public:
	Node(std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept;
	void Draw(ElektronGFX& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
private:
	void AddChild(std::unique_ptr<Node> pChild) noexcept;
private:
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
};

class ModelGraph
{
public:
	ModelGraph(ElektronGFX& gfx, const std::string fileName);
	void Draw(ElektronGFX& gfx, DirectX::FXMMATRIX transform) const;
private:
	static std::unique_ptr<Mesh> ParseMesh(ElektronGFX& gfx, const aiMesh& mesh);
	std::unique_ptr<Node> ParseNode(const aiNode& node);
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
};

