#include "bone.h"

void Bone::init(std::string name, glm::mat4 boneOffset, aiNode node)
{
	this->boneOffset = boneOffset;
	this->name = name;
}

void Bone::addVert(int vertIndex, float weight) {
	this->indicesWeights.insert(std::make_pair(vertIndex, weight));
}

glm::vec4 Bone::rotateAround(glm::vec4 aPointToRotate, glm::vec4 aRotationCenter, glm::mat4x4 aRotationMatrix)
{
	glm::mat4 translate =
		glm::translate(
			glm::mat4(1),
			glm::vec3(aRotationCenter.x, aRotationCenter.y, aRotationCenter.z));
	glm::mat4 invTranslate = glm::inverse(translate);

	// The idea:
	// 1) Translate the object to the center
	// 2) Make the rotation
	// 3) Translate the object back to its original location

	glm::mat4 transform = translate * aRotationMatrix * invTranslate;

	return transform * aPointToRotate;
}
