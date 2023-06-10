#pragma once
#include "model.h"

class Draw :
    public Model
{
    public:
        void draw(glm::mat4 transform, glm::vec3 scaleVec, ShaderProgram* sp);
};

