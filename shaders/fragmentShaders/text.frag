#version 330 core
/**
 * @file    scene.cpp
 * @author  opengl-tutorial.org
 * @date    2021 November
 */

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

	color = texture( myTextureSampler, UV );
	
	
}
