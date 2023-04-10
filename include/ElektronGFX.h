#pragma once
#include "ElekWin.h"
#include <d3d11.h>

class ElektronGFX
{
public :
	ElektronGFX(HWND hWnd);
	ElektronGFX(const ElektronGFX&) = delete;
	ElektronGFX& operator=(const ElektronGFX&) = delete;
	~ElektronGFX();

	void PresentFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pRTView = nullptr;
};

