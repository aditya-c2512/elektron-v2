#pragma once
#include "ConstantBuffer.h"
#include "../Camera.h"

class ShadowCBuf: public Bindable
{
private:
	struct Transform
	{
		DirectX::XMMATRIX viewProj;
	};
public:
	ShadowCBuf(ElektronGFX& gfx, UINT slot = 1);
	void Bind(ElektronGFX& gfx) noexcept override;
	void Update(ElektronGFX& gfx);
	void SetCamera(const Camera* pCamera);
private:
	static std::unique_ptr<VertexConstantBuffer<Transform>> pVCBuf;
	//static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVCBuf;
	const Camera* pCam;
};

