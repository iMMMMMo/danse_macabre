#pragma once
#include "model.h"

class Skeleton :
    public Model
{
    public:
        void draw(float playerY, ShaderProgram* sp);
};

