#ifndef GRAPH_H
#define GRAPH_H

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

    glm::vec3 bounding_box_min;
    glm::vec3 bounding_box_max;

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

    glm::vec3 bounding_box_min;
    glm::vec3 bounding_box_max;

    Cell_T *left_cell;
    Cell_T *right_cell;
};

struct Graph_T {
    std::vector<Portal_T *> portals;
    std::vector<Cell_T *> cells;
};

#endif
