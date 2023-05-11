#include "../include/drawables/SolidSphere.h"
#include "../include/drawables/Cube.h"
#include "../include/bindables/Bindable.h"
#include "../include/bindables/Bindables.h"

SolidSphere::SolidSphere(ElektronGFX& gfx, float radius)
{
	namespace dx = DirectX;

	if (!isStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = Cube::Make<Vertex>();
		model.Transform(dx::XMMatrixScaling(radius, radius, radius));
		AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"VS_Solid.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PS_Solid.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		} colorConst;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

void SolidSphere::Update(float dt) noexcept {}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransform() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void SolidSphere::SpawnControlWindow() noexcept
{
}
