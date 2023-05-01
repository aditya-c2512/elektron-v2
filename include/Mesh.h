#pragma once

#include "../include/Vertex.h"
#include <DirectXMath.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class Mesh
{
public:
	Mesh(aiMesh* mesh);
	std::vector<Vertex>& getVertices() noexcept;
	std::vector<unsigned short> getIndices() noexcept;
	void Transform(DirectX::FXMMATRIX matrix);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};