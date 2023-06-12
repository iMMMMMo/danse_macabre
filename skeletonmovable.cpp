#include "skeletonmovable.h"

void SkeletonMovable::setRotationPoint(glm::vec4 rotationPoint) {
	this->rotationPoint = rotationPoint;
}

glm::mat4 SkeletonMovable::rotateAround(glm::mat4 M, float angle, glm::vec3 axis, glm::vec3 jumped) {
	glm::mat4 jumpedMat = glm::translate(glm::mat4(1), jumped);
	glm::mat4 jumpedMatInv = glm::inverse(jumpedMat);

	glm::mat4 translate =
		glm::translate(
			glm::mat4(1),
			glm::vec3(rotationPoint.x, rotationPoint.y, rotationPoint.z));
	glm::mat4 invTranslate = glm::inverse(translate);

	glm::mat4 rotate = glm::rotate(glm::mat4(1), angle, axis);

	// The idea:
	// 1) Translate the object to the center
	// 2) Make the rotation
	// 3) Translate the object back to its original location

	glm::mat4 transform = jumpedMat * translate * rotate * invTranslate;

	return transform * M;
}

void SkeletonMovable::draw(float playerY, float angle_x, ShaderProgram* sp) {
	glm::mat4 M = glm::mat4(1.0f);
	M = rotateAround(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, playerY));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex")); //Enable sending data to the attribute vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts.data()); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("normal")); //Enable sending data to the attribute color
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms.data()); //Specify source of the data for the attribute normal

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords.data());

	glUniform1i(sp->u("textureMap0"), 0);
	glUniform1i(sp->u("textureMap1"), 1);
	glUniform1i(sp->u("textureMap2"), 2);
	glUniform1i(sp->u("textureMap3"), 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex3);


	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data()); //Draw the object

	glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("normal")); //Disable sending data to the attribute normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}