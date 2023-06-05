#pragma once

#include "Bindable.h"
#include "../../DirectXTex/DirectXTex.h"
#include <string>
#include <unordered_map>

enum class ELEKTRON_TEXTURE_FORMAT
{
	ELEKTRON_TEXTURE_PNG,
	ELEKTRON_TEXTURE_JPG,
	ELEKTRON_TEXTURE_DDS,
	ELEKTRON_TEXTURE_HDR,
};

class ElekTex : public Bindable
{
public:
	ElekTex(ElektronGFX& gfx, std::wstring textureName, unsigned int slot = 0);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	unsigned int slot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};

class ElekTexMap
{
public:
	ElekTexMap();
	ElekTex* GetTexture(ElektronGFX& gfx, std::wstring textureName, unsigned int slot = 0);
	void AddTexture(ElektronGFX& gfx, std::wstring textureName, unsigned int slot = 0);
private:
	std::unordered_map<std::wstring, std::unique_ptr<ElekTex>> textureMap;
};

