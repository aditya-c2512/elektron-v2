#include "../include/bindables/Sampler.h"

Sampler::Sampler(ElektronGFX& gfx)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MAXIMUM_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = D3D11_DEFAULT_MAX_ANISOTROPY;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;

	GetDevice(gfx)->CreateSamplerState(&desc, &pSamplerState);
}

void Sampler::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
}
