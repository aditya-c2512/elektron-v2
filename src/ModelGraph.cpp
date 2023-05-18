#include "../include/drawables/ModelGraph.h"
#include "../include/Vertex.h"
#include "../include/bindables/Texture.h"
#include "../include/bindables/Sampler.h"

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

ModelGraph::ModelGraph(ElektronGFX& gfx, const std::string fileName)
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals
	);

	name = pScene->mName.C_Str();
	OutputDebugStringA(name.c_str());

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i]));
	}

	pRoot = ParseNode(*pScene->mRootNode);
}

void ModelGraph::Draw(ElektronGFX& gfx) const
{
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

std::unique_ptr<Mesh> ModelGraph::ParseMesh(ElektronGFX& gfx, const aiMesh& mesh)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int vIdx = 0; vIdx < mesh.mNumVertices; vIdx++)
	{
		aiVector3D aiVertex = mesh.mVertices[vIdx];
		aiVector3D aiNorm = mesh.mNormals[vIdx];

		aiVector3D aiTexCoord = mesh.mTextureCoords[0][vIdx];

		Vertex vert;
		vert.pos.x = aiVertex.x;
		vert.pos.y = aiVertex.y;
		vert.pos.z = aiVertex.z;

		vert.n.x = aiNorm.x;
		vert.n.y = aiNorm.y;
		vert.n.z = aiNorm.z;

		vert.texCoords.x = aiTexCoord.x;
		vert.texCoords.y = aiTexCoord.x;

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

	auto pvs = std::make_unique<VertexShader>(gfx, L"VS_Phong.cso");
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back(std::move(pvs));

	bindablePtrs.push_back(std::make_unique<PixelShader>(gfx, L"PS_Phong.cso"));

	bindablePtrs.push_back(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	std::string texturePath = "C:/Projects/elektron-v2/assets/models/nanosuit/glass_dif.png";
	bindablePtrs.push_back(std::make_unique<Texture>(gfx, texturePath));
	bindablePtrs.push_back(std::make_unique<Sampler>(gfx));

	struct PSMaterialBuffer
	{
		float intensity_specular;
		float power_specular;
		float padding[2];
	} materialBuff;
	materialBuff.intensity_specular = 1.0f;
	materialBuff.power_specular = 35.0f;
	bindablePtrs.push_back(std::make_unique<PixelConstantBuffer<PSMaterialBuffer>>(gfx, materialBuff, 1));

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
