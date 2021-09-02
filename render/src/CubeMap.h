#pragma once
#include "texture.h"
#include "buffers.h"
#include "Shader.h"
class CubeMap {
public:
	CubeMap(const char* path);
	~CubeMap();
	inline unsigned int  getPreFilterMap()
	{
		return prefilterMap;
	}
	inline unsigned int  getIrradianceMap()
	{
		return irradianceMap;
	}
	inline unsigned int  getBrdfLUT()
	{
		return brdfLUT;
	}
	inline unsigned int  getEnvCubemap()
	{
		return envCubemap;
	}
private:
	unsigned int prefilterMap;
	unsigned int irradianceMap;
	unsigned int brdfLUT;
	unsigned int envCubemap;
	unsigned int captureFBO;
	unsigned int captureRBO;
	unsigned int hdrTexture;
	Shader* equirectangularToCubemapShader, *irradianceShader,* prefilterShader,* brdfShader;
	static const glm::mat4 captureViews[6];
	static const glm::mat4 captureProjection;

};