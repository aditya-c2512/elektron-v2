#define STB_IMAGE_IMPLEMENTATION
#include "../include/drawables/Model.h"
#include "../include/ElekException.h"

#include "../include/Vertex.h"
#include "../include/bindables/ConstantBuffer.h"
#include "../include/bindables/Texture.h"
#include "../include/bindables/Sampler.h"
#include "../include/bindables/IndexBuffer.h"
#include "../include/bindables/InputLayout.h"
#include "../include/bindables/PixelShader.h"
#include "../include/bindables/VertexBuffer.h"
#include "../include/bindables/VertexShader.h"
#include "../include/bindables/Topology.h"
#include "../include/bindables/TransformCBuf.h"


#include <assimp/Importer.hpp>

#include <iostream>

Model::Model(ElektronGFX& gfx, std::string assetPath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(assetPath,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices);

    // If the import failed, report it
    if (scene == nullptr) {
        throw ElekException(__LINE__,__FILE__);
    }

	for (int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		Mesh mesh(scene->mMeshes[meshIdx]);
		meshes.push_back(mesh);
		meshes.push_back(mesh);
	}
	
	namespace dx = DirectX;

	meshes[1].Transform(dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f));

	if (!isStaticInitialized())
	{
		//meshes[0].Transform(dx::XMMatrixScaling(8.0f, 8.0f, 8.0f));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, meshes[0].getVertices()));

		auto pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pVSByteCode = pVertexShader->GetBytecode();
		AddStaticBind(std::move(pVertexShader));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
		
		std::string texturePath = "C:/Projects/elektron-v2/assets/models/textures/bunny_red.png";
		AddStaticBind(std::make_unique<Texture>(gfx, texturePath));
		AddStaticBind(std::make_unique<Sampler>(gfx));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, meshes[0].getIndices()));

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pVSByteCode));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));

	dx::XMStoreFloat3x3(&modelTransform, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));

}

void Model::Update(float dt) noexcept
{
	angle += dt;
}

DirectX::XMMATRIX Model::GetTransform() const noexcept
{
	return DirectX::XMLoadFloat3x3(&modelTransform) * DirectX::XMMatrixTranslation(pos[0],pos[1],pos[2]) * DirectX::XMMatrixRotationX(angles[0]) * DirectX::XMMatrixRotationY(angles[1]) * DirectX::XMMatrixRotationZ(angles[2]);
}

void Model::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Model"))
	{
		ImGui::Text("POSITION");
		ImGui::DragFloat3("World Space", pos, 0.001f, -1000.0f, 1000.0f);
		ImGui::Text("ROTATION");
		ImGui::SliderAngle("X", &angles[0], -180.0f, 180.0f);
		ImGui::SliderAngle("Y", &angles[1], -180.0f, 180.0f);
		ImGui::SliderAngle("Z", &angles[2], -180.0f, 180.0f);
		if (ImGui::Button("RESET"))
		{
			pos[0] = pos[1] = pos[2] = 0.0f;
			angles[0] = angles[1] = angles[2] = 0.0f;
		}
	}
	ImGui::End();
}
