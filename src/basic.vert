#version 330 core

// Input vertex data
layout(location = 0) in vec3 vertexPosition_modelspace; // Vertex position
layout(location = 1) in vec3 vertexNormal_modelspace;   // Vertex normal

// Output vertex data
out vec3 Normal_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	
	Normal_cameraspace = vec3(1,1,1);
}

