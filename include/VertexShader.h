#pragma once
#include "Bindable.h"
#include <string>

class VertexShader : public Bindable
{
public:
	VertexShader(ElektronGFX& gfx, const std::wstring& path);
	void Bind(ElektronGFX& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};

