#include "../include/PointLight.h"
#include "../include/Camera.h"

PointLight::PointLight(ElektronGFX& gfx) : mesh(gfx,0.1f), pointCBuf(gfx)
{
	pCam = std::make_shared<Camera>(gfx, "Point Light", plBuf.pos, 0, 0);
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Position");
		ImGui::DragFloat3("World Space", &plBuf.pos.x, 0.1f, -100.0f, 100.0f, "%.1f");

		pCam->SetPos(plBuf.pos);

		ImGui::Text("Light Parameters");
		ImGui::SliderFloat("Diffuse Intensity", &plBuf.intensity_diffuse, 0.0f, 100.0f, "%.3f");
		ImGui::ColorEdit3("Diffuse Color", &plBuf.diffuse_color.x);

		if (ImGui::Button("Reset"))
		{
			plBuf = {
				{0.0f, 0.0f, 0.0f},
				{ 0.282f, 0.302f, 0.38f },
				{ 1.0f,1.0f,1.0f },
					1.0f,
			};
		}
	}
	ImGui::End();
}

void PointLight::Draw(ElektronGFX& gfx) const noexcept
{
	mesh.SetPos(plBuf.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(ElektronGFX& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto plCopy = plBuf;

	const auto pos = DirectX::XMLoadFloat3(&plCopy.pos);
	DirectX::XMStoreFloat3(&plCopy.pos, DirectX::XMVector3Transform(pos, view));

	pointCBuf.Update(gfx, plCopy);
	pointCBuf.Bind(gfx);
}

std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
{
	return pCam;
}
