#include "../include/ElektronGFX.h"
#include "../include/ElekException.h"

namespace wrl = Microsoft::WRL;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

ElektronGFX::ElektronGFX(HWND hWnd, int width, int height) : width(width), height(height)
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

	// Create Depth Stencil Buffer
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	wrl::ComPtr<ID3D11DepthStencilState> pDepthStencilState = nullptr;
	pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);

	pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC pDSTexState = {};
	pDSTexState.Width = width;
	pDSTexState.Height = height;
	pDSTexState.Format = DXGI_FORMAT_D32_FLOAT;
	pDSTexState.MipLevels = 1u;
	pDSTexState.ArraySize = 1u;
	pDSTexState.SampleDesc.Count = 1u;
	pDSTexState.SampleDesc.Quality = 0u;
	pDSTexState.Usage = D3D11_USAGE_DEFAULT;
	pDSTexState.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pDevice->CreateTexture2D(&pDSTexState, nullptr, &pDepthStencil);

	// Get a View to the Depth Stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDSView);

	pDeviceContext->OMSetRenderTargets(1u, pRTView.GetAddressOf(), pDSView.Get());

	// Configure Viewport
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pDeviceContext->RSSetViewports(1, &vp);
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
	pDeviceContext->ClearDepthStencilView(pDSView.Get(), D3D11_CLEAR_DEPTH, 1u, 0u);
}

void ElektronGFX::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX ElektronGFX::GetProjection() const noexcept
{
	return projection;
}

void ElektronGFX::DrawIndexed(int count)
{
	pDeviceContext->DrawIndexed((UINT)count, 0u, 0u);
}
