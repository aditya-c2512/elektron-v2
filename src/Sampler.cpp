#include "../include/bindables/Sampler.h"

Sampler::Sampler(ElektronGFX& gfx)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GetDevice(gfx)->CreateSamplerState(&desc, &pSamplerState);
}

void Sampler::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
}
