#include "../include/bindables/ElekTexMap.h"

ElekTexMap::ElekTexMap()
{
}

ElekTex* ElekTexMap::GetTexture(ElektronGFX& gfx, std::wstring textureName, unsigned int slot)
{
	AddTexture(gfx, textureName, slot);
	return textureMap[textureName].get();
}

void ElekTexMap::AddTexture(ElektronGFX& gfx, std::wstring textureName, unsigned int slot)
{
	if (textureMap.find(textureName) == textureMap.end())
	{
		textureMap[textureName] = std::make_unique<ElekTex>(gfx, textureName, slot);
	}
}

ElekTex::ElekTex(ElektronGFX& gfx, std::wstring textureName, unsigned int slot): slot(slot)
{
	// Load Texture from file
	auto image = std::make_unique<DirectX::ScratchImage>();
	HRESULT hr = DirectX::LoadFromWICFile(textureName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, *image);

	if (SUCCEEDED(hr))
	{
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
		subResData.pSysMem = image->GetImage(0,0,0)->pixels;
		subResData.SysMemPitch = image->GetImage(0,0,0)->rowPitch;

		GetDevice(gfx)->CreateTexture2D(&desc, &subResData, &pTexture2D);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		GetDevice(gfx)->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pTextureView);
	}
	
}

void ElekTex::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(slot, 1, pTextureView.GetAddressOf());
}
