#pragma once
#include "Shader.h"
class Bloom
{
public:
	Bloom();
	~Bloom();
	void setup();
	void draw();
private:
	Shader* shaderBlur, * shaderBloomFinal;
	unsigned int pingpongFBO[2] = {0};
	unsigned int pingpongColorbuffers[2] = {0};
	unsigned int colorBuffers[2] = {0};
};

