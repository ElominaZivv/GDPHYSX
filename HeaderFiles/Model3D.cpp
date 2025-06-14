#include "Model3D.h"

Model3D::Model3D(std::shared_ptr<VAO> newVAO) {
	modelVAO = newVAO;
	identity_matrix = mat4(1.f);
	transform_matrix = mat4(1.f);

	// Default Color: White
	color = vec3(1.0f, 1.0f, 1.0f);
	size = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Model3D::draw(Shader shader, Camera camera) {
	shader.activate();

	//Camera Manipulation
	unsigned int viewLoc = glGetUniformLocation(shader.getShader(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera.getViewMatrix()));

	unsigned int projLoc = glGetUniformLocation(shader.getShader(), "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(camera.getProjectionMatrix()));

	//Model position in the screen
	unsigned int transformLoc = glGetUniformLocation(shader.getShader(), "transform");

	//Transformation (translation, scale, and rotations)
	transform_matrix = translate(identity_matrix, modelPos);
	transform_matrix = scale(transform_matrix, size);

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform_matrix));

	//Change Color of Model
	GLuint solidColorAddress = glGetUniformLocation(shader.getShader(), "solidColor");
	glUniform3fv(solidColorAddress, 1, value_ptr(color));

	//Draw
	glBindVertexArray(modelVAO->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, modelVAO->getFullVertexData().size() / 8);

	//Unbind
	glBindVertexArray(0);
	glActiveTexture(0);
}

void Model3D::setModelPos(float x, float y, float z)
{
	modelPos = vec3(x, y, z);
}

void Model3D::setModelPosVec(physics::Vector vecPos)
{
	modelPos = vec3(vecPos);
}

void Model3D::setSize(float _size)
{
	size = glm::vec3(_size, _size, _size);
}