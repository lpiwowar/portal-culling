/**
 * @file    scene.cpp
 * @author  opengl-tutorial.org
 * @date    2021 November
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

typedef struct Cell_T Cell_T;
typedef struct Portal_T Portal_T;
typedef struct Graph_T Graph_T;

struct Cell_T {
    unsigned int id;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer; 

    glm::vec3 boundingBoxMin;
    glm::vec3 boundingBoxMax;

    std::vector<Portal_T *> portals;
};

struct Portal_T {
    unsigned int id;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer; 

    glm::vec3 boundingBoxMin;
    glm::vec3 boundingBoxMax;

    Cell_T *leftCell;
    Cell_T *rightCell;
};

struct Graph_T {
    std::vector<Portal_T *> portals;
    std::vector<Cell_T *> cells;
};

#endif
