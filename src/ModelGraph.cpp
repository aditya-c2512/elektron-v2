#include "../include/drawables/ModelGraph.h"
#include "../include/Vertex.h"
#include "../include/bindables/Texture.h"
#include "../include/bindables/Sampler.h"
#include "../include/bindables/ShadowSampler.h"

Mesh::Mesh(ElektronGFX& gfx, std::vector<std::unique_ptr<Bindable>> bindPtrs, std::string name) : name(name)
{
	if (!isStaticInitialized())
	{
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (auto& pb : bindPtrs)
	{
		if (auto pi = dynamic_cast<IndexBuffer*>(pb.get()))
		{
			AddIndexBuffer(std::unique_ptr<IndexBuffer>{ pi });
			pb.release();
		}
		else
		{
			AddBind(std::move(pb));
		}
	}

	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void Mesh::Update(float dt) noexcept
{
}

void Mesh::Draw(ElektronGFX& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Draw(gfx);
}

DirectX::XMMATRIX Mesh::GetTransform() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}

std::string Mesh::GetName() const noexcept
{
	return name;
}

Node::Node(std::string name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept : name(name), meshPtrs(std::move(meshPtrs))
{
	DirectX::XMStoreFloat4x4(&this->transform, transform);
}

void Node::Draw(ElektronGFX& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept
{
	const auto built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;
	for (const auto pm : meshPtrs)
	{
		pm->Draw(gfx, built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->Draw(gfx, built);
	}
}

void Node::RenderNodeTree() noexcept
{
	if (childPtrs.empty())
	{
		ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf);
		ImGui::TreePop();
		return;
	}
	if (ImGui::TreeNode(name.c_str()))
	{
		for (const auto& pc : childPtrs)
		{
			pc->RenderNodeTree();
		}
		ImGui::TreePop();
	}
}

void Node::AddChild(std::unique_ptr<Node> pChild) noexcept
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}

ModelGraph::ModelGraph(ElektronGFX& gfx, ElekTexMap& elekTexMap, const std::string basePath, const std::string modelName, ELEKTRON_MODEL_FORMAT model_format) : basePath(basePath), pShadowCBuf(std::make_unique<ShadowCBuf>(gfx))
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile((basePath+modelName).c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace
	);

	OutputDebugStringA("ModelGraph\n");

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, elekTexMap, *pScene->mMeshes[i], pScene->mMaterials));
	}
	pRoot = ParseNode(*pScene->mRootNode);
}

void ModelGraph::Draw(ElektronGFX& gfx, bool isShadowPass) const
{
	this->isShadowPass = isShadowPass;

	if (isShadowPass) pShadowCBuf->Update(gfx);

	const auto transform = dx::XMMatrixRotationRollPitchYaw(pos.pitch, pos.yaw, pos.roll) * dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	pRoot->Draw(gfx, transform);
}

void ModelGraph::SpawnModelGraphControlWindow() noexcept
{
	if (ImGui::Begin("Model", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Columns(2, nullptr, true);
		pRoot->RenderNodeTree();

		ImGui::NextColumn();
		ImGui::Text("POSITION");
		ImGui::DragFloat3("World Space", &pos.x, 0.001f, -1000.0f, 1000.0f, "%.1f");
		ImGui::Text("ROTATION");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f, "%.1f");
		if (ImGui::Button("RESET"))
		{
			pos.x = pos.y = pos.z = 0.0f;
			pos.roll = pos.pitch = pos.yaw = 0.0f;
		}
	}
	ImGui::End();
}

std::unique_ptr<Mesh> ModelGraph::ParseMesh(ElektronGFX& gfx, ElekTexMap& elekTexMap, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	float minX = 3, minY = 3;
	for (unsigned int vIdx = 0; vIdx < mesh.mNumVertices; vIdx++)
	{
		aiVector3D aiVertex = mesh.mVertices[vIdx];
		aiVector3D aiNorm = mesh.mNormals[vIdx];
		aiVector3D aiTexCoord = mesh.mTextureCoords[0][vIdx];
		aiVector3D aiTangent = mesh.mTangents[vIdx];
		aiVector3D aiBitangent = mesh.mBitangents[vIdx];

		minX = aiTexCoord.x < minX ? aiTexCoord.x : minX;
		minY = aiTexCoord.y < minY ? aiTexCoord.y : minY;
		Vertex vert;
		vert.pos.x = aiVertex.x;
		vert.pos.y = aiVertex.y;
		vert.pos.z = aiVertex.z;

		vert.n.x = aiNorm.x;
		vert.n.y = aiNorm.y;
		vert.n.z = aiNorm.z;

		vert.texCoords.x = aiTexCoord.x;
		vert.texCoords.y = aiTexCoord.y;

		vert.tangent.x = aiTangent.x;
		vert.tangent.y = aiTangent.y;
		vert.tangent.z = aiTangent.z;

		vert.bitangent.x = aiBitangent.x;
		vert.bitangent.y = aiBitangent.y;
		vert.bitangent.z = aiBitangent.z;

		vertices.push_back(vert);
	}

	for (unsigned int fIdx = 0; fIdx < mesh.mNumFaces; fIdx++)
	{
		aiFace face = mesh.mFaces[fIdx];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	std::vector<std::unique_ptr<Bindable>> bindablePtrs;

	bindablePtrs.push_back(std::make_unique<VertexBuffer>(gfx, vertices));

	bindablePtrs.push_back(std::make_unique<IndexBuffer>(gfx, indices));

	if (isShadowPass)
	{
		bindablePtrs.push_back(std::make_unique<PixelShader>(gfx, L"PS_Solid.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colorConst;
		colorConst.color = { 1.0f,1.0f,1.0f };
		bindablePtrs.push_back(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

		auto pvs = std::make_unique<VertexShader>(gfx, L"VS_Solid.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(std::make_unique<InputLayout>(gfx, ied, pvsbc));
	}
	else
	{
		if (mesh.mMaterialIndex >= 0)
		{
			auto& material = *pMaterials[mesh.mMaterialIndex];

			aiString albedoName, normalName, metallicName;
			std::string albedo_path = "", normal_path = "", metallic_path = "";
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &albedoName) == aiReturn_SUCCESS)
			{
				albedo_path = basePath + albedoName.C_Str();
				bindablePtrs.push_back(std::make_unique<ElekTex>(gfx, elekTexMap, albedo_path, 0));
			}
			if (material.GetTexture(aiTextureType_NORMALS, 0, &normalName) == aiReturn_SUCCESS)
			{
				normal_path = basePath + normalName.C_Str();
				bindablePtrs.push_back(std::make_unique<ElekTex>(gfx, elekTexMap, normal_path, 1));
			}
			if (material.GetTexture(aiTextureType_METALNESS, 0, &metallicName) == aiReturn_SUCCESS)
			{
				metallic_path = basePath + metallicName.C_Str();
				bindablePtrs.push_back(std::make_unique<ElekTex>(gfx, elekTexMap, metallic_path, 2));
			}

			bindablePtrs.push_back(std::make_unique<Sampler>(gfx));
		}

		bindablePtrs.push_back(std::make_unique<PixelShader>(gfx, L"PS_Sponza.cso"));

		auto pvs = std::make_unique<VertexShader>(gfx, L"VS_Sponza.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		bindablePtrs.push_back(std::make_unique<ShadowCBuf>(gfx));

		bindablePtrs.push_back(std::make_unique<ShadowSampler>(gfx));
	}
	
	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs), mesh.mName.C_Str());
}

std::unique_ptr<Node> ModelGraph::ParseNode(const aiNode& node)
{
	namespace dx = DirectX;
	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(*node.mChildren[i]));
	}

	return pNode;
}
