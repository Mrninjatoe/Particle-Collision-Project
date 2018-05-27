#include "meshloader.hpp"
#include <vector>

MeshLoader::MeshLoader() {

	
}

MeshLoader::~MeshLoader() {

}

//Mesh& MeshLoader::loadMesh(const std::string& path) {
//
//}

Mesh* MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint> indices;

	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Mesh::Vertex vertex;
		glm::vec3 tempV;

		tempV.x = mesh->mVertices[i].x;
		tempV.y = mesh->mVertices[i].y;
		tempV.z = mesh->mVertices[i].z;
		vertex.pos = tempV;
		min = glm::min(min, vertex.pos);
		max = glm::max(max, vertex.pos);

		tempV.x = mesh->mNormals[i].x;
		tempV.y = mesh->mNormals[i].y;
		tempV.z = mesh->mNormals[i].z;

		vertex.normal = tempV;
		// cool colors :)
		vertex.color = glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else
			vertex.uv = glm::vec2(0.f);

		vertices.push_back(vertex);
	}

	std::vector<Mesh::Triangle> triangles;
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		Mesh::Triangle temp;
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
			temp.verts[j] = glm::vec4(vertices[face.mIndices[j]].pos, 1);
			//temp.indices[j] = indices.back();
		}
		temp.id.x = i;
		triangles.push_back(temp);
	}

	//for (int i = 0; i < vertices.size() / 3; i += 3) {
	//	for (int j = 0; j < 3; j++) {
	//		//temp.verts[j] = vertices[indices[j + i]].pos;
	//	}
	//	triangles.push_back(temp);
	//}
	//printf("%zu\n", triangles.size());

	return new Mesh(vertices, indices, min, max, true, triangles);
}

void MeshLoader::processNode(aiNode* node, const aiScene* scene, Model& models) {
	printf("NumMeshes in aiScene: %u\n", scene->mNumMeshes);
	for (GLuint i = 0; i < node->mNumMeshes;i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		models.meshes.push_back(processMesh(mesh, scene));

		models.boundingBox.min = glm::min(models.boundingBox.min, glm::vec4(models.meshes.back()->getMin(), 1));
		models.boundingBox.max = glm::max(models.boundingBox.max, glm::vec4(models.meshes.back()->getMax(), 1));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, models);
	}
}

Model MeshLoader::loadMesh(const char* path, bool hasModelMX) {
	// For models without normals we need aiProcess_GenNormals. Should always have them with .fbx though.
	const aiScene* test = aiImportFile(path, aiProcess_PreTransformVertices | aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!test || test->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !test->mRootNode)
		printf("ERROR::ASSIMP::", aiGetErrorString());

	Model model;
	model.boundingBox.min = glm::vec4(0);
	model.boundingBox.max = glm::vec4(0);
	processNode(test->mRootNode, test, model);

	return model;
}

Mesh* MeshLoader::getQuad() {
	std::vector<Mesh::Vertex> vertices{
		Mesh::Vertex{ { -1, 1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 0, 1 }},
		Mesh::Vertex{ { 1, 1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 1, 1 }},
		Mesh::Vertex{ { 1, -1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 1, 0 }},
		Mesh::Vertex{ { -1, -1, 0 },{ 0, 0, -1 },{ 1, 0, 1 },{ 0, 0 }}
	};
	std::vector<GLuint> indices{ 0, 2, 1, 2, 0, 3 };
	return new Mesh(vertices, indices, false);
};

Mesh* MeshLoader::getCube() {
	//std::vector<Mesh::Vertex> vertices{
	//	Mesh::Vertex{ {-1, 0, -1}, {0,0,0}, {0,0,0}, {0,0}}, // 0
	//	Mesh::Vertex{ { -1, 1, -1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { 1, 1, -1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { 1, 0, -1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { -1, 1, -1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },

	//	Mesh::Vertex{ { -1, 0, 1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } }, // 5
	//	Mesh::Vertex{ { -1, 1, 1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { -1, 1, -1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { -1, 0, -1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { -1, 0, 1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },

	//	Mesh::Vertex{ { -1, 0, 1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { -1, 1, 1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//	Mesh::Vertex{ { -1, 1, 1 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	//};
	//std::vector<unsigned short> indices{
	//	0, 1, 2, 3, 4,
	//	5, 6, 7, 8, 9

	//};

	std::vector<Mesh::Vertex> vertices{
		Mesh::Vertex{ { -0.5, -0.5, -0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } }, // 0
		Mesh::Vertex{ { -0.5, 0.5, -0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
		Mesh::Vertex{ { 0.5, 0.5, -0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
		Mesh::Vertex{ { 0.5, -0.5, -0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
		Mesh::Vertex{ { 0.5, -0.5, 0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
		Mesh::Vertex{ { 0.5, 0.5, 0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
		Mesh::Vertex{ { -0.5, 0.5, 0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
		Mesh::Vertex{ { -0.5, -0.5, 0.5 },{ 0,0,0 },{ 0,0,0 },{ 0,0 } },
	};

	std::vector<GLuint> indices{
		0, 1, 
		1, 2, 
		2, 3,
		3, 0,
		3, 4, 
		4, 5, 
		5, 2,
		5, 6,
		6, 7, 
		7, 4,
		7, 0,
		6, 1
	};

	return new Mesh(vertices, indices, false);
}