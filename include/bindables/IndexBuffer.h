#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(ElektronGFX& gfx, const std::vector<unsigned int>& indices);
	void Bind(ElektronGFX& gfx) noexcept override;
	UINT GetCount() const noexcept;
private:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};

