#include "../include/drawables/Mesh.h"

Mesh::Mesh(aiMesh* mesh)
{
    for (unsigned int vIdx = 0; vIdx < mesh->mNumVertices; vIdx++)
    {
        aiVector3D aiVertex = mesh->mVertices[vIdx];
        aiVector3D aiNorm = mesh->mNormals[vIdx];
        //aiVector3D aiTexCoord = mesh->mTextureCoords[vIdx][0];

        Vertex vert;
        vert.pos.x = aiVertex.x;
        vert.pos.y = aiVertex.y;
        vert.pos.z = aiVertex.z;
        
        vert.n.x = aiNorm.x;
        vert.n.y = aiNorm.y;
        vert.n.z = aiNorm.z;

        vertices.push_back(vert);
    }
    for (unsigned int fIdx = 0; fIdx < mesh->mNumFaces; fIdx++)
    {
        aiFace face = mesh->mFaces[fIdx];
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
}

std::vector<Vertex>& Mesh::getVertices() noexcept
{
    return vertices;
}

std::vector<unsigned int> Mesh::getIndices() noexcept
{
    return indices;
}

void Mesh::Transform(DirectX::FXMMATRIX matrix)
{
    for (auto& vert : vertices)
    {
        const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vert.pos);
        DirectX::XMStoreFloat3(
            &vert.pos,
            DirectX::XMVector3Transform(pos, matrix)
        );
    }
}