#include "../include/bindables/ShadowCBuf.h"

ShadowCBuf::ShadowCBuf(ElektronGFX& gfx, UINT slot)
{
	if(!pVCBuf)
		pVCBuf = std::make_unique<VertexConstantBuffer<Transform>>(gfx, slot);
}

void ShadowCBuf::Bind(ElektronGFX& gfx) noexcept
{
	pVCBuf->Bind(gfx);
}

void ShadowCBuf::Update(ElektronGFX& gfx)
{

	const Transform t{
		DirectX::XMMatrixTranspose(gfx.GetCamera() * gfx.GetProjection())
	};
	pVCBuf->Update(gfx, t);
}

void ShadowCBuf::SetCamera(const Camera* pCamera)
{
	pCam = pCamera;
}

std::unique_ptr<VertexConstantBuffer<ShadowCBuf::Transform>> ShadowCBuf::pVCBuf;
