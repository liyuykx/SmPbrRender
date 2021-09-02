#pragma once
#include <stb_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <glad/glad.h>
struct PBRtextures {
	unsigned int albedoMap;
	unsigned int normalMap;
	unsigned int metallicMap;
	unsigned int roughnessMap;
	unsigned int aoMap;
};
unsigned int loadTexture(const char* path, bool gammaCorrection = false);
unsigned int loadTexture(const char* path, const std::string& directory, bool gammaCorrection = false);
unsigned int loadHDRTexture(const char* path);
void loadPBRTextures(const std::string& path, PBRtextures& pbrTex);
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int genIrradianceMap();
unsigned int genPrefilterMap();
unsigned int genEnvcubemap();
unsigned int genBrdfLUTTexture(int w, int h);
unsigned int generateTexture2D(int w, int h);
void bindTexture2D(unsigned int tex, int unit = 0);