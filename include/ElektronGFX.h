#pragma once
#include "ElekWin.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

class ElektronGFX
{
public :
	ElektronGFX(HWND hWnd);
	ElektronGFX(const ElektronGFX&) = delete;
	ElektronGFX& operator=(const ElektronGFX&) = delete;

	void PresentFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
	void DrawTestTriangle(float angle);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTView;
};

