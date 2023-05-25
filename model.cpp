#include "model.h"

void Model::loadModel(std::string plik) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(plik,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	std::cout << importer.GetErrorString() << std::endl;

	if (scene == NULL) {
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

	//for (int i = 0; i < leftHandBoneIndices.size(); i++) {
	//	glm::mat4 rotationMat(1);
	//	glm::vec3 pivotPoint(0.0f, 0.0f, 0.0f);
	//	// Translate the matrix to the pivot point
	//	rotationMat = glm::translate(rotationMat, pivotPoint);
	//	rotationMat = glm::rotate(rotationMat, AI_DEG_TO_RAD(10), glm::vec3(0.0f, 1.0f, 0.0f));
	//	rotationMat = glm::translate(rotationMat, -pivotPoint);
	//	verts[leftHandBoneIndices[i]] = rotationMat * verts[leftHandBoneIndices[i]];
	//}

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

