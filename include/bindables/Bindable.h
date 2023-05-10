#pragma once

#include "../ElektronGFX.h"
#include <vector>

class Bindable
{
public:
	virtual void Bind(ElektronGFX& gfx) = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(ElektronGFX& gfx) noexcept;
	static ID3D11Device* GetDevice(ElektronGFX& gfx) noexcept;
};

