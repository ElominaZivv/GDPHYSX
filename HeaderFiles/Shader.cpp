#include "Shader.h"

Shader::Shader(string vertFilePath, string fragFilePath)
{
	fstream vertSrc(vertFilePath);
	stringstream vertBuff;
	vertBuff << vertSrc.rdbuf();	

	string vertS = vertBuff.str();
	const char* v = vertS.c_str();

	fstream fragSrc(fragFilePath);
	stringstream fragBuff;
	fragBuff << fragSrc.rdbuf();

	string fragS = fragBuff.str();
	const char* f= fragS.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &f, NULL);
	glCompileShader(fragmentShader);

	shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragmentShader);
	glLinkProgram(shaderProg);
}

Shader::~Shader() {
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate() {
	glUseProgram(shaderProg);
}

GLuint Shader::getShader() {
	return shaderProg;
}