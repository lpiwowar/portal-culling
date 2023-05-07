/**
 * @file    scene.cpp
 * @author  opengl-tutorial.org, Lukas Piwowarski
 * @date    2021 November
 */

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <GL/gl.h>
#include "scene.hpp"

template<typename T>
bool loadOBJ(const char * path, T * object);
extern template bool loadOBJ<Cube_T>(const char* path, Cube_T* object);
extern template bool loadOBJ<Cell_T>(const char* path, Cell_T* object);
extern template bool loadOBJ<Portal_T>(const char* path, Portal_T* object);

bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif
