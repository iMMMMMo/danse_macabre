#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
		glm::vec4 rotateAround(glm::vec4 aPointToRotate, glm::vec4 aRotationCenter, glm::mat4x4 aRotationMatrix);
		void loadModel(std::string plik);
		void processBoneHierarchy(aiNode* node, const aiMesh* mesh);
		void getBoneIndices(const aiNode* node, const aiMesh* mesh);
};

#endif
