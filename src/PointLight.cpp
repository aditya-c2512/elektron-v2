#include "../include/PointLight.h"

PointLight::PointLight(ElektronGFX& gfx) : pointCBuf(gfx)
{
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			pos.x = pos.y = pos.z = 0.0f;
		}
	}
	ImGui::End();
}

//void PointLight::Draw(ElektronGFX& gfx) const noexcept
//{
//	mesh.SetPos(pos);
//	mesh.Draw(gfx);
//}

void PointLight::Bind(ElektronGFX& gfx) const noexcept
{
	pointCBuf.Update(gfx, PointLightBuffer{ pos });
	pointCBuf.Bind(gfx);
}
