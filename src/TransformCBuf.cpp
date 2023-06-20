#include "../include/bindables/TransformCBuf.h"

TransformCBuf::TransformCBuf(ElektronGFX& gfx, const Drawable& parent, UINT slot)
	:
	parent(parent)
{
	if (!pVCBuf)
	{
		pVCBuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx,slot);
		//pVCBuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCBuf::Bind(ElektronGFX& gfx) noexcept
{
	const auto modelView = parent.GetTransform() * gfx.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(parent.GetTransform()),
		DirectX::XMMatrixTranspose(gfx.GetCamera()),
		DirectX::XMMatrixTranspose(gfx.GetProjection())
	};
	pVCBuf->Update(gfx, tf);
	/*pVCBuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransform() * gfx.GetCamera() * gfx.GetProjection()
		)
	);*/
	pVCBuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::pVCBuf;
//std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuf::pVCBuf;