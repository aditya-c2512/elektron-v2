#pragma once
#include <vector>
#include <memory>

class Camera;
class ElektronGFX;

class Cameras
{
public:
	void SpawnControlWindow();
	void Bind(ElektronGFX& gfx);
	void Draw(ElektronGFX& gfx);
	void AddCamera(std::unique_ptr<Camera> pCam);
	Camera& GetCamera();
	~Cameras();
private:
	std::vector<std::unique_ptr<Camera>> cameras;
	int selected = 0;
};

