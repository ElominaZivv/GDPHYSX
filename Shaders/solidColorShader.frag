#version 330 core

uniform vec3 solidColor;
out vec4 FragColor;

void main(){
	FragColor = vec4(solidColor,1.0f);
}