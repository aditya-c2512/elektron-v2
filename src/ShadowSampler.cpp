#include "../include/bindables/ShadowSampler.h"

ShadowSampler::ShadowSampler(ElektronGFX& gfx)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.BorderColor[0] = 1.0f;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	desc.MipLODBias = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	GetDevice(gfx)->CreateSamplerState(&desc, &pSamplerState);
}

void ShadowSampler::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(1, 1, pSamplerState.GetAddressOf());
}
