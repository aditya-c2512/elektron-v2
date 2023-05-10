#pragma once
#include "Bindable.h"
class Topology : public Bindable
{
public:
	Topology(ElektronGFX& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	D3D11_PRIMITIVE_TOPOLOGY type;
};

