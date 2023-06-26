#include "../include/bindables/Sampler.h"

Sampler::Sampler(ElektronGFX& gfx)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	desc.MipLODBias = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	GetDevice(gfx)->CreateSamplerState(&desc, &pSamplerState);
}

void Sampler::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
}
