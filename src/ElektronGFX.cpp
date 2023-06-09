#include "../include/ElektronGFX.h"
#include "../include/ElekException.h"

namespace wrl = Microsoft::WRL;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

ElektronGFX::ElektronGFX(HWND hWnd, int width, int height) : width(width), height(height)
{
	// Configure Swap Chain
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferDesc.Width = width;
	SwapChainDesc.BufferDesc.Height = height;
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

	D3D11_TEXTURE2D_DESC pDSTexState = {};
	pDSTexState.Width = width;
	pDSTexState.Height = height;
	pDSTexState.Format = DXGI_FORMAT_R32_TYPELESS;
	pDSTexState.MipLevels = 1u;
	pDSTexState.ArraySize = 1u;
	pDSTexState.SampleDesc.Count = 1u;
	pDSTexState.SampleDesc.Quality = 0u;
	pDSTexState.Usage = D3D11_USAGE_DEFAULT;
	pDSTexState.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	pDevice->CreateTexture2D(&pDSTexState, nullptr, &pDepthStencil);

	// Get a View to the Depth Stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDSView);

	pDeviceContext->OMSetRenderTargets(1u, pRTView.GetAddressOf(), pDSView.Get());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	pDevice->CreateShaderResourceView(pDepthStencil.Get(), &srvDesc, &pDSTexView);

	// Configure Viewport
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pDeviceContext->RSSetViewports(1, &vp);

	ImGui_ImplDX11_Init(pDevice.Get(), pDeviceContext.Get());
}

void ElektronGFX::PresentFrame()
{
	if (isGuiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	if (isShadowPass)
	{
		pDeviceContext->PSSetShaderResources(3, 1, pDSTexView.GetAddressOf());
		return;
	}

	if (FAILED(pSwapChain->Present(1u, 0u)))
	{
		throw ElekException(__LINE__, __FILE__);
	}
}

void ElektronGFX::BeginFrame(float r, float g, float b) noexcept
{
	if (isGuiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { r,g,b,1.0f };
	pDeviceContext->ClearRenderTargetView(pRTView.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDSView.Get(), D3D11_CLEAR_DEPTH, 1u, 0u);
}

void ElektronGFX::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projectionMatrix = proj;
}

DirectX::XMMATRIX ElektronGFX::GetProjection() const noexcept
{
	return projectionMatrix;
}

void ElektronGFX::SetCamera(DirectX::XMMATRIX camMatrix, ElekShadowMap passType) noexcept
{
	if (passType == ELEK_SHADOW_PASS) isShadowPass = true;
	else isShadowPass = false;

	cameraMatrix = camMatrix;
}

DirectX::XMMATRIX ElektronGFX::GetCamera() const noexcept
{
	return cameraMatrix;
}

void ElektronGFX::EnableGui() noexcept
{
	isGuiEnabled = true;
}

void ElektronGFX::DisableGui() noexcept
{
	isGuiEnabled = false;
}

bool ElektronGFX::IsGuiEnabled() const noexcept
{
	return isGuiEnabled;
}

void ElektronGFX::BindShadowMap()
{
	ID3D11RenderTargetView* nullRTV = nullptr; pDeviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
	pDeviceContext->PSSetShaderResources(3, 1, pDSTexView.GetAddressOf());
}

void ElektronGFX::DrawIndexed(int count)
{
	pDeviceContext->DrawIndexed((UINT)count, 0u, 0u);
}
