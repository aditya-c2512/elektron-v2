#pragma once
#include "ElekWin.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

namespace dx = DirectX;

enum ElekShadowMap
{
	ELEK_SHADOW_PASS,
	ELEK_NORMAL_PASS
};

class ElektronGFX
{
	friend class Bindable;
	friend class PointLight;
	friend class ElekTexMap;
public :
	ElektronGFX(HWND hWnd, int width, int height);
	ElektronGFX(const ElektronGFX&) = delete;
	ElektronGFX& operator=(const ElektronGFX&) = delete;

	void PresentFrame();
	void BeginFrame(float r, float g, float b) noexcept;
	void DrawIndexed(int count);

	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::XMMATRIX camMatrix, ElekShadowMap passType=ELEK_NORMAL_PASS) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
	void EnableGui() noexcept;
	void DisableGui() noexcept;
	bool IsGuiEnabled() const noexcept;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRTV() noexcept
	{
		return pRTView;
	}
	void BindShadowMap();
	
private:
	int width, height;
	bool isGuiEnabled = true;
	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX cameraMatrix;
	bool isShadowPass = false;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pDSTexView;
};

