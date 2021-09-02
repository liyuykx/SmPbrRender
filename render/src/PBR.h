#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "imgui.h"
#include "Model.h"
#include "WindowManager.h"
#include "buffers.h"
#include "texture.h"
#include "drawableObject.h"
#include "CubeMap.h"
#include "Bloom.h"
class PBR : public drawableObject
{
public:
	virtual void display() override;
	PBR();
	~PBR();
private:
	float metallic = 0.6f;
	float roughness = 0.15f;
	float colorPicker[3] = { 1.0f, 0.0f, 0.0f };
	int mapCurrent = 0;
	int texCurrent = 0;
	unsigned int hdrFBO = 0;
	unsigned int PBRFBO = 0;
	glm::mat4 view = glm::mat4(1.0f);
	bool useBloom = true;
	int useTexture = 0;
	PBRtextures textures[4];
	Bloom* bloom;
	CubeMap* pbrMaps[4];
	Camera* PBRcamera;
	Shader* pbrShader,* backgroundShader,* pbrLightShader, *pbrShaderTexture,*shaderUse;
	void setGui();
	void update();
	void draw();
	void sphereDraw();
	void lightDraw();
	void SkyboxDraw();
	void bloomDraw();
};

