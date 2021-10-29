#version 330 core

// Input data
in vec3 Normal_cameraspace;

// Ouput data
out vec3 color;

void main(){
	// Light properties
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0f;
		
	// Material properties
	vec3 MaterialDiffuseColor = vec3(0.5,0.5,0.5); // gray
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;

	color = vec3(0.5,0.5,0.5);
}
