#include "../include/drawables/SkySphere.h"
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
#include <assimp/scene.h>
#include <assimp/postprocess.h>

SkySphere::SkySphere(ElektronGFX& gfx, ElekTexMap& elekTexMap)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile("C:/Projects/elektron-v2/assets/models/sphere.obj",
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

    // If the import failed, report it
    if (scene == nullptr) {
        throw ElekException(__LINE__, __FILE__);
    }

    aiMesh* mesh = scene->mMeshes[0];
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int vIdx = 0; vIdx < mesh->mNumVertices; vIdx++)
    {
        aiVector3D aiVertex = mesh->mVertices[vIdx];
        aiVector3D aiNorm = mesh->mNormals[vIdx];
        aiVector3D aiTexCoord = mesh->mTextureCoords[0][vIdx];

        Vertex vert;
        vert.pos.x = aiVertex.x;
        vert.pos.y = aiVertex.y;
        vert.pos.z = aiVertex.z;

        vert.n.x = aiNorm.x;
        vert.n.y = aiNorm.y;
        vert.n.z = aiNorm.z;

        vert.texCoords.x = aiTexCoord.x;
        vert.texCoords.y = aiTexCoord.y;
        //vert.texCoords.x = vert.texCoords.y = 0;

        vertices.push_back(vert);
    }
    for (unsigned int fIdx = 0; fIdx < mesh->mNumFaces; fIdx++)
    {
        aiFace face = mesh->mFaces[fIdx];
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    namespace dx = DirectX;

    if (!isStaticInitialized())
    {

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto pVertexShader = std::make_unique<VertexShader>(gfx, L"VS_PBR.cso");
        auto pVSByteCode = pVertexShader->GetBytecode();
        AddStaticBind(std::move(pVertexShader));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PS_Environment.cso"));

        std::string texturePath = "C:/Projects/elektron-v2/assets/models/sky/skymap_hdri.png";
        AddStaticBind(std::make_unique<ElekTex>(gfx, elekTexMap, texturePath, 0));

        AddStaticBind(std::make_unique<Sampler>(gfx));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pVSByteCode));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }
    AddBind(std::make_unique<TransformCBuf>(gfx, *this));

    dx::XMStoreFloat3x3(&modelTransform, dx::XMMatrixScaling(100.0f, 100.0f, 100.0f));
}

void SkySphere::Update(float dt) noexcept
{
}

DirectX::XMMATRIX SkySphere::GetTransform() const noexcept
{
    return DirectX::XMLoadFloat3x3(&modelTransform);
}
