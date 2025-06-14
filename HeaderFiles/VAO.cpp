#include "VAO.h"
VAO::VAO(string objFilePath) {
	//Initialization
	filePath = objFilePath;
	bool success = tinyobj::LoadObj(
		&attributes, //Overall def
		&shapes,  //Refers to the object itself
		&material, //Refers to the texture/image
		&warning,
		&error,
		filePath.c_str()
	);
	/* We need to instruct the EBO from the Mesh Data */
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		meshIndices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		tinyobj::index_t vData = shapes[0].mesh.indices[i];

		fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
		fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
		fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

		fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
		fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
		fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);

		fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
		fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);
	}

	//VAO VBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * fullVertexData.size(), fullVertexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(float),
		(void*)0
	);


	GLintptr normalPtr = 3 * sizeof(float);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(float),
		(void*)normalPtr
	);

	GLintptr uvPtr = 6 * sizeof(float);
	glVertexAttribPointer(
		//0 = position, 1 ?, 2 = UV/Texture data
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(void*)uvPtr
	);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_TEXTURE_2D, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

GLuint VAO::getVAO() {
	return vao;
}

vector<GLfloat> VAO::getFullVertexData() {
	return fullVertexData;
}