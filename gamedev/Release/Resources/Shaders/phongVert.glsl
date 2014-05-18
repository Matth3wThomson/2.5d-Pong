#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;
in vec3 normal;	//Our new vertex attribuute stream!

out Vertex {
	vec3 worldPos;
	vec2 texCoord;
	vec3 normal;
} OUT;

void main(void){
	vec4 worldPos = modelMatrix * vec4(position, 1.0);

	gl_Position	= (projMatrix * viewMatrix) * worldPos;

	OUT.texCoord = texCoord;
	OUT.worldPos = worldPos.xyz;

	//Cast to remove transformations.
	//Inverse the matrix to move back from model to local matrix
	//Transpose to invert any rotation (So inverse undesired effect is un done.)
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	//Normalize the matrix and multiply it by a normalized normal
	OUT.normal	= normalize(normalMatrix * normalize(normal));
}