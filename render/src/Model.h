#pragma once
#include<vector>
#include<string>
#include<iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "Mesh.h"
#include "texture.h"
class Model
{
public:
	Model(std::string path);
	~Model();
	void Draw();
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;
	void loadModel(std::string path);

private:

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};



