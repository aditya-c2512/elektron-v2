#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
class DrawableStatic : public Drawable
{
protected:
	static bool isStaticInitialized() noexcept
	{
		return !staticBindables.empty();
	}
	static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept
	{
		staticBindables.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<class IndexBuffer> idxBuff) noexcept
	{
		pIndexBuffer = idxBuff.get();
		staticBindables.push_back(std::move(idxBuff));
	}
	void SetIndexFromStatic() noexcept
	{
		for (const auto& b : staticBindables)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
	{
		return staticBindables;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticBindables;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableStatic<T>::staticBindables;