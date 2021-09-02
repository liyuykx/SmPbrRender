#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "imgui.h"
#include "Model.h"
#include "WindowManager.h"
#include "buffers.h"
#include "shapes.h"
#include "Camera.h"
#include "drawableObject.h"
#include "Bloom.h"
class ShadowMapping : public drawableObject
{
public:
	ShadowMapping();
	~ShadowMapping();
	virtual void display() override;
private:
	Model* marry;
	Shader* simpleDepthShader, * pcfShader, * pcssShader, *lightShader;
	Shader* skyboxShader;
	int cubemapTexture;
	FrameBufferObject* depthMapFBOs[3];
	int resolutionID = 0;;
	int exampleID = 0;
	float bias = 0.005f;
	int pcfKernelSize = 0;;
	int blockerKernelSize = 2;
	float lightSourceSize = 10.0f;
	bool  pcfUsePossion = false;
	bool  pcssUsePossion = false;
	float yaw = 0;
	float pitch = 0;
	float formerYaw = 0;
	float formorPitch = 0;
	Camera* SMcamera;

	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5));
	glm::mat4 initialModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 lightPos = glm::vec3(-2.0f, 15.0f, -2.0f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 15.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 50.0f);
	glm::mat4 lightSpaceMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 50.0f) * glm::lookAt(glm::vec3(-2.0f, 15.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 15.0f, -2.0f));
	glm::vec3 viewPos = glm::vec3(1.0f);
	
	unsigned int woodTexture;
	void setGui();
	void draw();
	void update();
	void pcfDraw();
	void pcssDraw();
	void lightDraw();
	void skyBoxDraw();
	void depthMapDraw();
};
