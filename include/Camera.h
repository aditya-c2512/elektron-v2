#pragma once
#include "ElektronGFX.h"
#include "drawables/CameraMesh.h"
#include <string>
#include <math.h>
#include <algorithm>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
T wrap_angle(T theta)
{
	constexpr T twoPi = (T)2 * (T)PI_D;
	const T mod = fmod(theta, twoPi);
	if (mod > (T)PI_D)
	{
		return mod - twoPi;
	}
	else if (mod < (T)PI_D)
	{
		return mod + twoPi;
	}
	return mod;
}

class Camera
{
public:
	Camera(ElektronGFX& gfx, std::string name, DirectX::XMFLOAT3 homePos = {0,0,0}, float homePitch = 0, float homeYaw = 0) 
		: name(name), pos(homePos), pitch(homePitch), yaw(homeYaw), camMesh(gfx)
	{
	}
	std::string GetName()
	{
		return name;
	}
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset();
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;

	void Draw(ElektronGFX& gfx);
private:
	std::string name;
	
	DirectX::XMFLOAT3 pos;
	float pitch, yaw;
	static constexpr float tSpeed = 6.0f;
	static constexpr float rSpeed = 0.004f;

	CameraMesh camMesh;

public:
	float near_plane = 0.5f, far_plane = 500.0f;
	float fov = 90.0f;
};

