#pragma once
#include "model.h"

class SkeletonMovable :
    public Model
{
    public:
        void setRotationPoint(glm::vec4 rotationPoint);
        glm::mat4 rotateAround(glm::mat4 M, float angle, glm::vec3 axis, glm::vec3 jumped);
        void draw(float playerY, float angle_x, ShaderProgram* sp);
    private:
        glm::vec4 rotationPoint;
};

