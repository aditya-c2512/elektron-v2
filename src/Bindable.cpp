#include "../include/bindables/Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(ElektronGFX& gfx) noexcept
{
	return gfx.pDeviceContext.Get();
}

ID3D11Device* Bindable::GetDevice(ElektronGFX& gfx) noexcept
{
	return gfx.pDevice.Get();
}
