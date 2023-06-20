#include "../include/bindables/ElekTexMap.h"

ElekTexMap::ElekTexMap()
{
}

ElekTexMap::~ElekTexMap()
{
	for (auto texture : textureMap)
	{
		delete texture.second;
	}
}

DirectX::ScratchImage* ElekTexMap::GetScratchImage(std::string key)
{
	if (textureMap.find(key) == textureMap.end())
	{
		std::wstring w_key(key.begin(), key.end());
		DirectX::ScratchImage* image = new DirectX::ScratchImage();
		HRESULT hr;
		if(key.ends_with(".hdr"))
			hr = DirectX::LoadFromHDRFile(w_key.c_str(), nullptr, *image);
		else if (key.ends_with(".dds"))
			hr = DirectX::LoadFromDDSFile(w_key.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, *image);
		else if(key.ends_with(".tga"))
			hr = DirectX::LoadFromTGAFile(w_key.c_str(), nullptr, *image);
		else
			hr = DirectX::LoadFromWICFile(w_key.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, *image);
		
		textureMap[key] = image;
	}

	return textureMap[key];
}

ElekTex::ElekTex(ElektronGFX& gfx, ElekTexMap& elekTexMap, std::string textureName, unsigned int slot): slot(slot)
{
	if (textureName == "") return;
	auto image = elekTexMap.GetScratchImage(textureName);

	// Create Texture
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = image->GetMetadata().width;
	desc.Height = image->GetMetadata().height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = image->GetMetadata().format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = image->GetImage(0, 0, 0)->pixels;
	subResData.SysMemPitch = image->GetImage(0, 0, 0)->rowPitch;
			
	GetDevice(gfx)->CreateTexture2D(&desc, &subResData, &pTexture2D);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	GetDevice(gfx)->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pTextureView);
}

void ElekTex::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(slot, 1, pTextureView.GetAddressOf());
}

ElekTexCube::ElekTexCube(ElektronGFX& gfx, ElekTexMap& elekTexMap, std::string path, unsigned int slot): slot(slot)
{
	std::vector<std::string> sides = { "px.png","nx.png", "py.png", "ny.png", "pz.png", "nz.png" };
	std::vector<DirectX::ScratchImage*> textureCube;
	for (auto side : sides)
	{
		textureCube.push_back(elekTexMap.GetScratchImage(path+side));
	}

	// Create Texture
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = textureCube[0]->GetMetadata().width;
	desc.Height = textureCube[0]->GetMetadata().height;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = textureCube[0]->GetMetadata().format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA subResData[6];
	for (int i = 0; i < 6; i++)
	{
		subResData[i].pSysMem = textureCube[i]->GetImage(0, 0, 0)->pixels;
		subResData[i].SysMemPitch = textureCube[i]->GetImage(0, 0, 0)->rowPitch;
		subResData[i].SysMemSlicePitch = 0;
	}
	

	GetDevice(gfx)->CreateTexture2D(&desc, subResData, &pTexture2D);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	GetDevice(gfx)->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pTextureView);
}

void ElekTexCube::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(slot, 1, pTextureView.GetAddressOf());
}
