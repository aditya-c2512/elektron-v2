#include "../include/Model.h"
#include "../include/ElekException.h"
#include "../include/ConstantBuffer.h"
#include "../include/IndexBuffer.h"
#include "../include/InputLayout.h"
#include "../include/PixelShader.h"
#include "../include/VertexBuffer.h"
#include "../include/VertexShader.h"
#include "../include/Topology.h"
#include "../include/TransformCBuf.h"
#include <assimp/Importer.hpp>

Model::Model(ElektronGFX& gfx, std::string assetPath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(assetPath,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    // If the import failed, report it
    if (scene == nullptr) {
        throw ElekException(__LINE__,__FILE__);
    }

    for (unsigned int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
    {
        Mesh mesh(scene->mMeshes[meshIdx]);
        meshes.push_back(mesh);
    }

	namespace dx = DirectX;

	if (!isStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};

		meshes[0].Transform(dx::XMMatrixScaling(8.0f, 8.0f, 8.0f));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, meshes[0].getVertices()));

		auto pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pVSByteCode = pVertexShader->GetBytecode();
		AddStaticBind(std::move(pVertexShader));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, meshes[0].getIndices()));

		struct PSConstantBuffer
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const PSConstantBuffer cb2 =
		{
			{
				{ 1.0f,0.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,1.0f,1.0f },
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSConstantBuffer>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
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
	return DirectX::XMLoadFloat3x3(&modelTransform) * DirectX::XMMatrixRotationY(angle) * DirectX::XMMatrixTranslation(0.25f, -1.0f, 1.0f);
}
