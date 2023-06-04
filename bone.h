#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "convert.h"
#include <assimp/scene.h>
#include <map>

class Bone
{
	public:
		std::string name;
		glm::mat4 boneOffset;
		std::map<int, float> indicesWeights;
		aiNode node;
		void init(std::string name, glm::mat4 boneOffset, aiNode node);
		void addVert(int vertIndex, float weight);
		glm::vec4 rotateAround(glm::vec4 aPointToRotate, glm::vec4 aRotationCenter, glm::mat4x4 aRotationMatrix);
};

