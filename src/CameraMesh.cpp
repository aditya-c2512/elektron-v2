#include "../include/drawables/CameraMesh.h"
#include "../include/bindables/Bindables.h"

CameraMesh::CameraMesh(ElektronGFX& gfx)
{
	std::vector<CamVertex> vertices;
	std::vector<unsigned int> indices;

	const float x = 4.0f / 3.0f * 0.75f;
	const float y = 1.0f * 0.75f;
	const float z = -2.0f;
	const float thalf = x * 0.5f;
	const float tspace = y * 0.2f;
	vertices.emplace_back(dx::XMFLOAT3{ -x,y,0.0f });
	vertices.emplace_back(dx::XMFLOAT3{ x,y,0.0f });
	vertices.emplace_back(dx::XMFLOAT3{ x,-y,0.0f });
	vertices.emplace_back(dx::XMFLOAT3{ -x,-y,0.0f });
	vertices.emplace_back(dx::XMFLOAT3{ 0.0f,0.0f,z });
	vertices.emplace_back(dx::XMFLOAT3{ -thalf,y + tspace,0.0f });
	vertices.emplace_back(dx::XMFLOAT3{ thalf,y + tspace,0.0f });
	vertices.emplace_back(dx::XMFLOAT3{ 0.0f,y + tspace + thalf,0.0f });

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(0);
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(1);
	indices.push_back(4);
	indices.push_back(2);
	indices.push_back(4);
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(7);
	indices.push_back(5);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"VS_Solid.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PS_Solid.cso"));

	struct PSColorConstant
	{
		dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	} colorConst;
	colorConst.color = { 1.0f,1.0f,1.0f };
	AddBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void CameraMesh::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

void CameraMesh::SetRotation(DirectX::XMFLOAT3 rot) noexcept
{
	this->rot = rot;
}

DirectX::XMMATRIX CameraMesh::GetTransform() const noexcept
{
	return dx::XMMatrixRotationRollPitchYawFromVector(dx::XMLoadFloat3(&rot)) *
		dx::XMMatrixTranslationFromVector(dx::XMLoadFloat3(&pos));
}
