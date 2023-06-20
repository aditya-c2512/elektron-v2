#include "../include/PointLight.h"

PointLight::PointLight(ElektronGFX& gfx) : mesh(gfx,0.1f), pointCBuf(gfx)
{
	for (int i = 0; i < numLights; i++)
	{
		pointLights.push_back(
			{
				{0.0f,0.0f,0.0f},
				{0.282f, 0.302f, 0.38f},
				{1.0f,1.0f,1.0f},
				1.0f,
			}
		);
	}
	pointCBuf = PixelConstantBuffer<PointLightBuffer>(gfx, pointLights);
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Position");
		ImGui::DragFloat3("World Space", &plBuf.pos.x, 0.001f, -1000.0f, 1000.0f, "%.1f");

		ImGui::Text("Light Parameters");
		ImGui::SliderFloat("Diffuse Intensity", &plBuf.intensity_diffuse, 0.0f, 100.0f, "%.3f");
		ImGui::ColorEdit3("Diffuse Color", &plBuf.diffuse_color.x);

		if (ImGui::Button("Reset"))
		{
			plBuf = {
				{0.0f,0.0f,0.0f},
				{0.282f, 0.302f, 0.38f},
				{1.0f,1.0f,1.0f},
				1.0f,
			};
		}
	}
	ImGui::End();
}

void PointLight::SpawnDebugWindow() noexcept
{
	if (ImGui::Begin("Lights", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Columns(2, nullptr, true);
		static int e = 0;
		for (int i = 0; i < numLights; i++)
		{
			ImGui::RadioButton(std::to_string(i + 1).c_str(), &e, i);
		}
		ImGui::Text("Selected %d", e);

		ImGui::NextColumn();
		ImGui::Text("Position");
		ImGui::DragFloat3("World Space", &pointLights[e].pos.x, 0.001f, -1000.0f, 1000.0f, "%.2f");

		ImGui::Text("Light Parameters");
		ImGui::SliderFloat("Diffuse Intensity", &pointLights[e].intensity_diffuse, 0.0f, 100.0f, "%.1f");
		ImGui::ColorEdit3("Diffuse Color", &pointLights[e].diffuse_color.x);

		if (ImGui::Button("Reset"))
		{
			pointLights[e] = {
				{0.0f,0.0f,0.0f},
				{0.282f, 0.302f, 0.38f},
				{1.0f,1.0f,1.0f},
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
	auto plCopies = pointLights;

	for (auto plCopy : plCopies)
	{
		const auto pos = DirectX::XMLoadFloat3(&plCopy.pos);
		DirectX::XMStoreFloat3(&plCopy.pos, DirectX::XMVector3Transform(pos, view));
	}

	pointCBuf.Update(gfx, plCopies);
	pointCBuf.Bind(gfx);
}
