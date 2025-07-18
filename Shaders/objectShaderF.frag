#version 330 core

uniform sampler2D tex0;


in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

out vec4 FragColor;

void main(){
	vec3 normal = normalize(normCoord);
	//Point Light
	vec3 lightDir = normalize(lightPos - fragPos);

	float lightObjectDist=length(lightPos-fragPos);
	float apparentBrightness=lightLumens/(lightObjectDist*lightObjectDist);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec* specStr * lightColor;
	vec4 pointLightVal = vec4(specColor + diffuse + ambientCol, 1.0) * apparentBrightness;

	vec3 dirLightDir = normalize(dirLightDirection);

	float dirLightDiff = max(dot(normal, dirLightDir), 0.0);
	vec3 dirLightDiffuse = dirLightDiff * dirLightColor;

	vec3 dirAmbientCol = dirAmbientColor * dirAmbientStr;
	FragColor = texture(tex0, texCoord);
}