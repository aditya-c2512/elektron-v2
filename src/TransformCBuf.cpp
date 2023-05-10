#include "../include/bindables/TransformCBuf.h"

TransformCBuf::TransformCBuf(ElektronGFX& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVCBuf)
	{
		pVCBuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCBuf::Bind(ElektronGFX& gfx) noexcept
{
	pVCBuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransform() * gfx.GetCamera() * gfx.GetProjection()
		)
	);
	pVCBuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuf::pVCBuf;