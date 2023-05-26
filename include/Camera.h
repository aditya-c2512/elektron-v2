#pragma once
#include "ElektronGFX.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset();
private:
	float r = 30.0f;
	float theta = 0.0f, phi = 0.0f;
	float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;
public:
	float near_plane = 0.5f, far_plane = 500.0f;
};

