#pragma once

#include "ElektronGFX.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

class Bindable;

class Drawable
{
	template<class T>
	friend class DrawableStatic;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
	void Draw(ElektronGFX& gfx) const noexcept;
	virtual void Update(float deltaTime) noexcept = 0;
	virtual ~Drawable() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> idxBuff) noexcept;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> bindables;
};

