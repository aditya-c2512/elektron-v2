#pragma once

#include "Bindable.h"
#include <string>

class Texture : public Bindable
{
public:
	Texture(std::string& texturePath);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D;
};

