#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "shaderprogram.h"
#include "lodepng.h"

class Model {
	public:
		void loadModel(std::string plik);
		GLuint tex0;
		GLuint tex1;
		GLuint tex2;
		GLuint tex3;
	protected:
		std::vector<glm::vec4> verts;
		std::vector<glm::vec4> norms;
		std::vector<glm::vec2> texCoords;
		std::vector<unsigned int> indices;
};

#endif
