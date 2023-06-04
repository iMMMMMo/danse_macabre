#pragma once
#include "model.h"

class Wall :
    public Model
{
    public:
        void draw(glm::mat4 transform, ShaderProgram* sp);
};

