/**
 * @file    scene.hpp
 * @author  Lukas Piwowarski
 * @date    2021 November
 * @brief   This file contains all functions and types that are neccessary to 
 *          create scene graph used by portal occlusion culling.
 *
 *          Scene graph used by portal occlusion culling is graph G = (V, E) 
 *          with vertices V that represent sections of rendered scenes (usually 
 *          rooms) and edges E that represent portals.
 */

#ifndef SCENEHPP
#define SCENEHPP

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


typedef struct Cell_T Cell_T;
typedef struct Portal_T Portal_T;
typedef struct Graph_T Graph_T;

/**
 * @brief Represents cells in portal occlussion culling (vertices in the scene
 *        graph)
 */
struct Cell_T {
    unsigned int id;                 /** < Unique identifier of the cell.    */

	std::vector<glm::vec3> vertices; /** < Vertices of the object.           */
	std::vector<glm::vec2> uvs;      /** < UV coordinates.                   */
	std::vector<glm::vec3> normals;  /** < Normals of the vertices.          */

	GLuint vertexBuffer;             /** < ID of vertex buffer.              */
	GLuint uvBuffer;                 /** < ID of buffer with UV coordinates. */
	GLuint normalBuffer;             /** < ID of buffer with normals.        */

    glm::vec3 boundingBoxMin;        /** < Min coordinates of bounding box.  */
    glm::vec3 boundingBoxMax;        /** < Max coordinates of bounding box.  */

    std::vector<Portal_T *> portals; /** < Portals associated with the cell. */
};

/**
 * @brief Represents portals in portal occlussion culling (edges in the scene
 *        graph).
 */
struct Portal_T {
    unsigned int id;                 /** < Unique identifier of the cell.    */

	std::vector<glm::vec3> vertices; /** < Vertices of the object.           */
	std::vector<glm::vec2> uvs;      /** < UV coordinates.                   */
	std::vector<glm::vec3> normals;  /** < Normals of the vertices.          */

	GLuint vertexBuffer;             /** < ID of vertex buffer.              */
	GLuint uvBuffer;                 /** < ID of buffer with UV coordinates. */
	GLuint normalBuffer;             /** < ID of buffer with normals.        */

    glm::vec3 boundingBoxMin;        /** < Min coordinates of bounding box.  */
    glm::vec3 boundingBoxMax;        /** < Max coordinates of bounding box.  */

    Cell_T *leftCell = nullptr;                /** < Pointer to 1st cell.              */
    Cell_T *rightCell = nullptr;               /** < Pointer to 2nd cell.              */
};

/**
 * @brief Represents scene graph in the portal occlussion culling.
 */
struct Graph_T {
    std::vector<Portal_T *> portals;
    std::vector<Cell_T *> cells;
};

/**
 * @brief Creates scene graph that represents scene with given sceneName. When
 *        this function is used then the destroySceneGraph() function must be
 *        called when the scene graph is no longer required.
 * @param sceneName Name of the scene whose scene graph should be created.
 * @return Graph_T* Scene graph
 */
Graph_T *createSceneGraph(std::string sceneName);

/**
 * @brief Destroyes scene graph created by createSceneGraph(). It must be 
 *        called to free resources created by createSceneGraph().
 * @return void
 */
void destroySceneGraph(Graph_T *graph);

#endif
