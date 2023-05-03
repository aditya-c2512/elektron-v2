#include "../include/Texture.h"

Texture::Texture(ElektronGFX& gfx, std::string& texturePath)
{
	texture = stbi_load(texturePath.c_str(), &width, &height, &numChannels, 0);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subResData = {};
	subResData.pSysMem = texture;
	subResData.SysMemPitch = width * numChannels;

	GetDevice(gfx)->CreateTexture2D(&desc, &subResData, &pTexture2D);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	
	GetDevice(gfx)->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, &pTextureView);
}

void Texture::Bind(ElektronGFX& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());
}
