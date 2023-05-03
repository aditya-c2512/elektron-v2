#pragma once

#include "Bindable.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>

class Texture : public Bindable
{
public:
	Texture(ElektronGFX& gfx, std::string& texturePath);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	int width, height, numChannels;
	unsigned char* texture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};

