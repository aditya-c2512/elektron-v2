#include "../include/ElektronGFX.h"
#include "../include/ElekException.h"

#include <cmath>

namespace wrl = Microsoft::WRL;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

ElektronGFX::ElektronGFX(HWND hWnd)
{
	// Configure Swap Chain
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	// Create the D3D11 Device, Device Context, and Swap Chain
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext
	);
	if (hr != S_OK)
		throw ElekException(__LINE__, __FILE__);

	// Get access to pSwapChain's Back Buffer (Buffer ID = 0)
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	// Create the Render Target View from pBackBuffer
	if (pBackBuffer.Get() != nullptr)
	{
		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRTView);
	}
}

void ElektronGFX::PresentFrame()
{
	if (FAILED(pSwapChain->Present(1u, 0u)))
	{
		throw ElekException(__LINE__, __FILE__);
	}
}

void ElektronGFX::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r,g,b,1.0f };
	pDeviceContext->ClearRenderTargetView(pRTView.Get(), color);
}

void ElektronGFX::DrawTestTriangle(float angle)
{
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	wrl::ComPtr<ID3D11Buffer> pConstBuffer;

	struct Vertex
	{
		struct
		{
			float x, y;
		} pos;
		struct
		{
			unsigned char r, g, b, a;
		} color;
	};

	const Vertex verts[] = {
		{0.0f, 0.5f, 255, 0, 0, 0},
		{0.5f, -0.5f, 0, 255, 0, 0},
		{-0.5f, -0.5, 0, 0, 255, 0},
		{-0.3f, 0.3f, 0, 255, 255, 0},
		{0.3f, 0.3f, 255, 0, 255, 0},
		{0.0f, -0.8f, 255, 255, 0, 0},
	};

	D3D11_BUFFER_DESC vertBuffDesc = {};
	vertBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	vertBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBuffDesc.CPUAccessFlags = 0u;
	vertBuffDesc.MiscFlags = 0u;
	vertBuffDesc.ByteWidth = sizeof(verts);
	vertBuffDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subResDataVert = {};
	subResDataVert.pSysMem = verts;

	const UINT offset = 0;
	const UINT stride = sizeof(Vertex);

	pDevice->CreateBuffer(&vertBuffDesc, &subResDataVert, &pVertexBuffer);
	pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// Create Index Buffer
	const unsigned short indices[] =
	{
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5,
	};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA subResDataInd = {};
	subResDataInd.pSysMem = indices;

	pDevice->CreateBuffer(&indexBufferDesc, &subResDataInd, &pIndexBuffer);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Create Constant Buffer
	struct ConstantBuffer
	{
		struct
		{
			float element[4][4];
		} transformation;
	};

	const ConstantBuffer cb =
	{
		{
			std::cos(angle),	std::sin(angle),	0.0f,	0.0f,
			-std::sin(angle),	std::cos(angle),	0.0f,	0.0f,
			0.0f,				0.0f,				1.0f,	0.0f,
			0.0f,				0.0f,				0.0f,	1.0f,
		}
	};

	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0u;
	constBufferDesc.ByteWidth = sizeof(cb);
	constBufferDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA subResDataConst = {};
	subResDataConst.pSysMem = &cb;

	pDevice->CreateBuffer(&constBufferDesc, &subResDataConst, &pConstBuffer);
	pDeviceContext->VSSetConstantBuffers(0, 1, pConstBuffer.GetAddressOf());

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;

	// Create Pixel Shader
	HRESULT hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	if (hr != S_OK) throw ElekException(__LINE__, __FILE__);

	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	if (hr != S_OK) throw ElekException(__LINE__, __FILE__);

	pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	// Create Vertex Shader
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	if (hr != S_OK) throw ElekException(__LINE__, __FILE__);

	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (hr != S_OK) throw ElekException(__LINE__, __FILE__);

	pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

	// Create Input Layout
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	pDevice->CreateInputLayout(inputDesc, std::size(inputDesc), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	pDeviceContext->IASetInputLayout(pInputLayout.Get());

	// Set Render Target View
	pDeviceContext->OMSetRenderTargets(1, pRTView.GetAddressOf(), nullptr);

	// Configure Viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 800;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pDeviceContext->RSSetViewports(1, &vp);

	//pDeviceContext->Draw((UINT)std::size(verts), 0u);
	pDeviceContext->DrawIndexed((UINT)std::size(indices), 0, 0);
}
