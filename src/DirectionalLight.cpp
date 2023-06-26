#include "../include/DirectionalLight.h"

DirectionalLight::DirectionalLight(ElektronGFX& gfx) : dirCBuf(gfx)
{
}

void DirectionalLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Directional Light", NULL, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::DragFloat3("Direction", &dlBuf.lightDir.x, 0.01f, -1.0f, 1.0f, "%.2f");
	}
	ImGui::End();
}

void DirectionalLight::Bind(ElektronGFX& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto dlCopy = dlBuf;
	const auto dir = DirectX::XMLoadFloat3(&dlCopy.lightDir);
	DirectX::XMStoreFloat3(&dlCopy.lightDir, DirectX::XMVector3Transform(dir, view));

	dirCBuf.Update(gfx, dlCopy);
	dirCBuf.Bind(gfx);
}
