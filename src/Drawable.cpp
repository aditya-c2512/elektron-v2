#include "../include/drawables/Drawable.h"
#include "../include/bindables/Bindable.h"
#include "../include/bindables/IndexBuffer.h"

void Drawable::Draw(ElektronGFX& gfx) const noexcept
{
	for (auto& bindable : bindables)
	{
		bindable->Bind(gfx);
	}
	for (auto& sBindable : GetStaticBinds())
	{
		sBindable->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	bindables.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> idxBuff) noexcept
{
	if (pIndexBuffer == nullptr)
	{
		pIndexBuffer = idxBuff.get();
		bindables.push_back(std::move(idxBuff));
	}
}
