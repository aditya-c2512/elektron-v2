#pragma once
#include "Bindable.h"

class ShadowSampler: public Bindable
{
public:
	ShadowSampler(ElektronGFX& gfx);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
};

