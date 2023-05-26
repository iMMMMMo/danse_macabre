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
#include "model.h"

glm::vec4 Model::rotateAround(
	glm::vec4 aPointToRotate,
	glm::vec4 aRotationCenter,
	glm::mat4 aRotationMatrix)
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

void Model::loadModel(std::string plik) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(plik,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (scene == NULL) {
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	aiMesh* mesh = scene->mMeshes[0];

	//aiNode* rootNode = scene->mRootNode;
	//processBoneHierarchy(rootNode, mesh);

	for (int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		verts.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
	}

	for (int i = 0; i < leftHandBoneIndices.size(); i++) {
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1), AI_DEG_TO_RAD(5), glm::vec3(0.0f,1.0f, 0.0f));
		//verts[leftHandBoneIndices[i]] = rotationMat * glm::vec4(verts[leftHandBoneIndices[i]]);
		verts[leftHandBoneIndices[i]] = rotateAround(verts[leftHandBoneIndices[i]], glm::vec4(0.0f, 0.0f, -20.0f, 1.0f), rotationMat);
		//verts[leftHandBoneIndices[i]] *= glm::vec4(2.0f, 1.0f, 1.0f, 1.0f);
	}


	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);

			//int vertexIndex = face.mIndices[j];
			/*for (int k = 0; k < leftHandBoneIndices.size(); k++) {
				if (leftHandBoneIndices[k] = vertexIndex) {
					indices.push_back(vertexIndex);
					break;
				}
			}*/
		}
	}

}

void Model::processBoneHierarchy(aiNode* node, const aiMesh* mesh)
{
	std::string boneName = node->mName.C_Str();
	std::cout << boneName << std::endl;
	std::string bodyElement = "Bip01_Head";
	if (boneName.substr(0, bodyElement.length()) == bodyElement) {
		getBoneIndices(node, mesh);
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		processBoneHierarchy(node->mChildren[i], mesh);
	}
}

void Model::getBoneIndices(const aiNode* node, const aiMesh* mesh)
{
	for (int i = 0; i < mesh->mNumBones; i++) {
		aiBone* bone = mesh->mBones[i];

		if (bone->mName == node->mName) {
			for (int j = 0; j < bone->mNumWeights; j++) {
				aiVertexWeight& weight = bone->mWeights[j];
				unsigned int vertexIndex = weight.mVertexId;
				leftHandBoneIndices.push_back(vertexIndex);
			}
		}
	}
}

