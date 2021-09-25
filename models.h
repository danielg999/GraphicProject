#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include <iostream>
#include "lodepng.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model {
	
public:
	
	std::string plik;
	glm::mat4 P,V, M;
	int vertexCount;
	float* vertices;
	float* normals;
	float* vertexNormals;
	float* texCoords;
	float* colors;
	ShaderProgram* sp;

	std::vector<glm::vec4> verts;
	std::vector<glm::vec4> norms;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned int> indices;
	GLuint Tex;

	void loadModel(std::string plik) {
		using namespace std;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(plik,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
		);
		std::cout << importer.GetErrorString() << std::endl;

		aiMesh* mesh = scene->mMeshes[0];
		for (int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D vertex = mesh->mVertices[i];
			verts.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

			aiVector3D normal = mesh->mNormals[i];
			norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			textureCoords.push_back(glm::vec2(texCoord.x, texCoord.y));

		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}
	}

	void drawSolid(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
		sp->use();//Aktywacja programu cieniuj¹cego

		//Przeslij parametry programu cieniuj¹cego do karty graficznej
		glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

		glm::vec4 light = glm::vec4(0, 21, -13, 1);
		glm::vec4 light2 = glm::vec4(13, 11, 12, 1);

		glUniform4fv(sp->u("lp"), 1, glm::value_ptr(light));
		glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(light2));

		glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts.data()); //Wska¿ tablicê z danymi dla atrybutu vertex

		glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms.data()); //Wska¿ tablicê z danymi dla atrybutu normal

		glEnableVertexAttribArray(sp->a("texCoord0"));  //W³¹cz przesy³anie danych do atrybutu normal
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, textureCoords.data()); //Wska¿ tablicê z danymi dla atrybutu normal

		glUniform1i(sp->u("textureMap0"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Tex);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

		glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
		glDisableVertexAttribArray(sp->a("texCoord0"));  //Wy³¹cz przesy³anie danych do atrybutu color
		glDisableVertexAttribArray(sp->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
	}
};
