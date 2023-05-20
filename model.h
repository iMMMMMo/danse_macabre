#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

class Model {
	public:
		std::vector<glm::vec4> verts;
		std::vector<glm::vec4> norms;
		std::vector<glm::vec2> texCoords;
		std::vector<unsigned int> indices;
		std::vector<int> leftHandBoneIndices;
		void loadModel(std::string plik);
		void processBoneHierarchy(aiNode* node, const aiMesh* mesh);
		void getBoneIndices(const aiNode* node, const aiMesh* mesh);
};

#endif
