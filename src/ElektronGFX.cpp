#include "../include/ElektronGFX.h"

#pragma comment(lib,"d3d11.lib")

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
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext
	);

	// Get access to pSwapChain's Back Buffer (Buffer ID = 0)
	ID3D11Resource* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	// Create the Render Target View from pBackBuffer
	if (pBackBuffer != nullptr)
	{
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRTView);
		pBackBuffer->Release();
	}
}

ElektronGFX::~ElektronGFX()
{
	if (pRTView != nullptr) pRTView->Release();
	if (pSwapChain != nullptr) pSwapChain->Release();
	if (pDeviceContext != nullptr) pDeviceContext->Release();
	if (pDevice != nullptr) pDevice->Release();
}

void ElektronGFX::PresentFrame()
{
	pSwapChain->Present(1u, 0);
}

void ElektronGFX::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r,g,b,1.0f };
	pDeviceContext->ClearRenderTargetView(pRTView, color);
}
