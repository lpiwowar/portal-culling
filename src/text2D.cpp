/**
 * @file    scene.cpp
 * @author  opengl-tutorial.org
 * @date    2021 November
 */

#include <vector>
#include <cstring>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../include/shader.hpp"
#include "../include/texture.hpp"

#include "../include/text2D.hpp"
#include "../include/config.h"
#include <iostream>

unsigned int Text2DTextureID;
unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;
unsigned int Text2DShaderID;
unsigned int Text2DUniformID;

void initText2D(const char * texturePath){

	// Initialize texture
	Text2DTextureID = loadDDS(texturePath);

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	// Initialize Shader
	Text2DShaderID = LoadShaders(SOURCE_DIR "/shaders/vertexShaders/text.vert", SOURCE_DIR "/shaders/fragmentShaders/text.frag");

	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation( Text2DShaderID, "myTextureSampler" );

}

void printText2D(const char * text, int x, int y, int size){

	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for ( unsigned int i=0 ; i<length ; i++ ){

#if 1
        size = 10;
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size+10);
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size+10);
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y);
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y);
#endif
#if 0
		glm::vec2 vertex_up_left    = glm::vec2( 20     , 600 );
		glm::vec2 vertex_up_right   = glm::vec2( 500 , 600 );
		glm::vec2 vertex_down_right = glm::vec2( 500 , 10      );
		glm::vec2 vertex_down_left  = glm::vec2( 20, 10 );
#endif
		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
        // std::cout << "Character: " << character << "(" << (int)character << ")" << std::endl;
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;
        // std::cout << "uv_x: " << uv_x << " uv_y: " << uv_y << std::endl;

#if 1
        float corr_hor = 0.02;
		glm::vec2 uv_up_left    = glm::vec2( uv_x+corr_hor           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f-corr_hor, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f-corr_hor, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x+corr_hor           , (uv_y + 1.0f/16.0f) );
        
#endif

#if 0
        float corr_hor = 0.00;
		glm::vec2 uv_down_right  = glm::vec2( uv_x+corr_hor           , uv_y+0.3 );
		glm::vec2 uv_down_left   = glm::vec2( uv_x+1.0f/16.0f-corr_hor, uv_y+0.3);
		glm::vec2 uv_up_left     = glm::vec2( uv_x+1.0f/16.0f-corr_hor, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_up_right    = glm::vec2( uv_x+corr_hor           , (uv_y + 1.0f/16.0f) );
#endif

#if 0 
		glm::vec2 uv_up_left    = glm::vec2( 0, 1);
		glm::vec2 uv_up_right   = glm::vec2( 1, 1);
		glm::vec2 uv_down_right = glm::vec2( 1, 0);
		glm::vec2 uv_down_left  = glm::vec2( 0, 0); 
#endif
#if 0
		glm::vec2 uv_up_left    = glm::vec2( 19 * (1/26.0f), 1);
		glm::vec2 uv_up_right   = glm::vec2( 20 * (1/26.0f), 1);
		glm::vec2 uv_down_right = glm::vec2( 20 * (1/26.0f), 1-(2 * (1/26.0f)));
		glm::vec2 uv_down_left  = glm::vec2( 19 * (1/26.0f), 1-(2 * (1/26.0f))); 
#endif
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(Text2DShaderID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void cleanupText2D(){

	// Delete buffers
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &Text2DTextureID);

	// Delete shader
	glDeleteProgram(Text2DShaderID);
}
