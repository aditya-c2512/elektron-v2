#pragma once

#include "Bindable.h"
#include "../../DirectXTex/DirectXTex.h"
#include <string>
#include <unordered_map>

class ElekTexCube : public Bindable
{
public:
	ElekTexCube(ElektronGFX& gfx, ElekTexMap& elekTexMap, std::string path, unsigned int slot = 0);
	void Bind(ElektronGFX& gfx) noexcept override;
private:
	unsigned int slot;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};

class ElekTex : public Bindable
{
public:
	ElekTex(ElektronGFX& gfx, ElekTexMap& elekTexMap, std::string textureName, unsigned int slot = 0);
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
	~ElekTexMap();
	DirectX::ScratchImage* GetScratchImage(std::string key);
private:
	std::unordered_map<std::string, DirectX::ScratchImage*> textureMap;
};

