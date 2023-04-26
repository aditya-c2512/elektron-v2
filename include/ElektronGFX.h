#pragma once
#include "ElekWin.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

namespace dx = DirectX;

class ElektronGFX
{
	friend class Bindable;
public :
	ElektronGFX(HWND hWnd, int width, int height);
	ElektronGFX(const ElektronGFX&) = delete;
	ElektronGFX& operator=(const ElektronGFX&) = delete;

	void PresentFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
	void DrawIndexed(int count);

	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	
private:
	int width, height;

	DirectX::XMMATRIX projection;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSView;
};

