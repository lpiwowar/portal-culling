#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <GL/gl.h>

typedef struct {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer; 
} Object_T;

bool loadOBJ(
	const char * path, 
	Object_T * object
);



bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif
