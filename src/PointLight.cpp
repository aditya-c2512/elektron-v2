#include "../include/PointLight.h"

PointLight::PointLight(ElektronGFX& gfx) : mesh(gfx,0.01f), pointCBuf(gfx)
{
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &plBuf.pos.x, -2.0f, 2.0f, "%.3f");
		ImGui::SliderFloat("Y", &plBuf.pos.y, -2.0f, 2.0f, "%.3f");
		ImGui::SliderFloat("Z", &plBuf.pos.z, -2.0f, 2.0f, "%.3f");

		ImGui::Text("Light Parameters");
		ImGui::SliderFloat("Diffuse Intensity", &plBuf.intensity_diffuse, 0.0f, 100.0f, "%.3f");
		ImGui::ColorEdit3("Diffuse Color", &plBuf.diffuse_color.x);
		ImGui::ColorEdit3("Ambient Color", &plBuf.ambient_color.x);

		ImGui::Text("Phong Parameters");
		ImGui::SliderFloat("Constant", &plBuf.att_const, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &plBuf.att_lin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &plBuf.att_quad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			plBuf = {
				{0.0f,0.0f,0.0f},
				{0.282f, 0.302f, 0.38f},
				{1.0f,1.0f,1.0f},
				1.0f,1.0f,1.0f,1.0f
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
