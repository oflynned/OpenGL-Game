#pragma once

#include <string>
#include <vector>

#include "Shader.cpp"
#include "GLIncludes.h"

#include "Math\maths_funcs.h"
#include "assimp\Importer.hpp"

class Mesh {
public:
	struct Vertex {
		vec3 position;
		vec3 normal;
		vec2 texCoords;
		vec3 tangent;
		vec2 bitangent;
	};

	struct Texture {
		GLuint id;
		std::string type;
		aiString path;
	};

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		this->setupMesh();
	}

	~Mesh() {}

	void draw(Shader shader) {
		GLuint diffuse_no = 1, specular_no = 1, normal_no = 1, height_no = 1;
		std::string number;

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string name = this->textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuse_no++);
			else if (name == "texture_specular")
				std::to_string(specular_no++);
			else if (name == "texture_normal")
				std::to_string(normal_no++);
			else if (name == "texture_height")
				std::to_string(height_no++);
		}

		// draw to bound vertex array
		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// reset to default and deallocate memory
		for (auto i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	GLuint getVAO() { return this->vao; }

private:
	GLuint vao, vbo, ebo;

	void setupMesh() {
		glGenVertexArrays(1, &this->vao);
		glGenBuffers(1, &this->vbo);
		glGenBuffers(1, &this->ebo);

		glBindVertexArray(this->vao);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
			&this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
			&this->indices[0], GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)0);

		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, normal));

		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, texCoords));

		// Vertex Tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
			(GLvoid*)offsetof(Vertex, tangent));

		// Vertex Bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
			(GLvoid*)offsetof(Vertex, bitangent));

		glBindVertexArray(0);
	}
};