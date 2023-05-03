#pragma once
#include "Bindable.h"
class Sampler : public Bindable
{
public:
	Sampler(ElektronGFX& gfx);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
};

