#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	//DirectX::XMFLOAT2 texCoords;
};

const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
{
	{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	//{ "TexCoords",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
};