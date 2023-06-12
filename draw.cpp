#include "draw.h"

void Draw::draw(glm::mat4 transform, glm::vec3 scaleVec, ShaderProgram* sp) {
	glm::mat4 M = glm::mat4(1.0f);
	//M = glm::rotate(M, AI_DEG_TO_RAD(90), glm::vec3(1.0f, 0.0f, 0.0f));
	M = transform * M;
	M = glm::scale(M, scaleVec);

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