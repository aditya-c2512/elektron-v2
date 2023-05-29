#include "../include/Camera.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);
	return dx::XMMatrixLookAtLH(
		pos, dx::XMVectorZero(), dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * dx::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("POSITION");
		ImGui::SliderFloat("Radius", &r, 0.1f, 100.0f, "%.3f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("ORIENTATION");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		ImGui::Text("PROJECTION PLANES");
		ImGui::SliderFloat("Near Plane", &near_plane, 0.01f, 10.0f,"%.3f");
		ImGui::SliderFloat("Far Plane", &far_plane, 0.01f, 1000.0f);
		if (ImGui::Button("RESET"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset()
{
	r = 30.0f;
	theta = phi = roll = pitch = yaw = 0.0f;
	near_plane = 0.5f;
	far_plane = 500.0f;
}
