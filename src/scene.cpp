// Include GLEW
#include <GL/glew.h>

#include "libs/objloader.hpp"
#include "libs/graph.hpp"


template<typename T> 
void fillObjectBuffers(T * object) {
    glGenBuffers(1, &object->vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, object->vertices.size() * sizeof(glm::vec3), &object->vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &object->uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, object->uvs.size() * sizeof(glm::vec2), &object->uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &object->normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, object->normals.size() * sizeof(glm::vec3), &object->normals[0], GL_STATIC_DRAW);
	
    return;
}

void makeEdge(Cell_T *leftCell, Portal_T *portal, Cell_T *rightCell) {

    leftCell->portals.insert(leftCell->portals.end(), {portal});
    rightCell->portals.insert(rightCell->portals.end(), {portal});
    portal->leftCell = leftCell;
    portal->rightCell = rightCell;

}

template<typename T>
void calcBoundingBox(T * object) {
    glm::vec3 min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

    for(glm::vec3 it : object->vertices) {
        if ( it.x < min.x ) min.x = it.x;
        if ( it.y < min.y ) min.y = it.y;
        if ( it.z < min.z ) min.z = it.z;
        if ( it.x > max.x ) max.x = it.x;
        if ( it.y > max.y ) max.y = it.y;
        if ( it.z > max.z ) max.z = it.z;
    }

    object->boundingBoxMin = min;
    object->boundingBoxMax = max;

    return;
}


Graph_T *createSceneGraph(std::string sceneName) {
    if (sceneName == "pgr_scene") {
        Cell_T *cell_1 = new Cell_T;
        loadOBJ("./scenes/pgr_scene/pgr_room1.obj", cell_1);
        fillObjectBuffers(cell_1);
        calcBoundingBox(cell_1);

        Cell_T *cell_2 = new Cell_T;
        loadOBJ("./scenes/pgr_scene/pgr_room2.obj", cell_2);
        fillObjectBuffers(cell_2);
        calcBoundingBox(cell_2);

        Cell_T *cell_3 = new Cell_T;
        loadOBJ("./scenes/pgr_scene/pgr_room3.obj", cell_3);
        fillObjectBuffers(cell_3);
        calcBoundingBox(cell_3);

        Cell_T *cell_4 = new Cell_T;
        loadOBJ("./scenes/pgr_scene/pgr_room4.obj", cell_4);
        fillObjectBuffers(cell_4);
        calcBoundingBox(cell_4);

        Cell_T *cell_5 = new Cell_T;
        loadOBJ("./scenes/pgr_scene/pgr_room5.obj", cell_5);
        fillObjectBuffers(cell_5);
        calcBoundingBox(cell_5);

        Portal_T *portal_1 = new Portal_T;
        loadOBJ("./scenes/pgr_scene/pgr_portal1.obj", portal_1);
        fillObjectBuffers(portal_1);
        calcBoundingBox(portal_1);

        Portal_T *portal_2 = new Portal_T;
        loadOBJ("./scenes/pgr_scene/pgr_portal2.obj", portal_2);
        fillObjectBuffers(portal_2);
        calcBoundingBox(portal_2);

        Portal_T *portal_3 = new Portal_T;
        loadOBJ("./scenes/pgr_scene/pgr_portal3.obj", portal_3);
        fillObjectBuffers(portal_3);
        calcBoundingBox(portal_3);

        Portal_T *portal_4 = new Portal_T;
        loadOBJ("./scenes/pgr_scene/pgr_portal4.obj", portal_4);
        fillObjectBuffers(portal_4);
        calcBoundingBox(portal_4);

        Portal_T *portal_5 = new Portal_T;
        loadOBJ("./scenes/pgr_scene/pgr_portal5.obj", portal_5);
        fillObjectBuffers(portal_5);
        calcBoundingBox(portal_5);
        
        Graph_T *graph = new Graph_T;
        graph->cells.insert(graph->cells.end(), {cell_1, cell_3, cell_4, cell_5});
        graph->portals.insert(graph->portals.end(), {portal_1, portal_3, portal_4, portal_5});
        
        makeEdge(cell_1, portal_1, cell_5);
        makeEdge(cell_3, portal_3, cell_5);
        makeEdge(cell_3, portal_4, cell_5);
        makeEdge(cell_4, portal_5, cell_5);
        
        return graph;
    }

    return nullptr;
}


void destroySceneGraph(Graph_T *graph) {
    for (auto const &cell: graph->cells) {
	    glDeleteBuffers(1, &cell->vertexbuffer);
	    glDeleteBuffers(1, &cell->uvbuffer);
	    glDeleteBuffers(1, &cell->normalbuffer);
        delete cell;
    }

    for (auto const &portal: graph->portals) {
        delete portal;
    }

    delete graph;

    return;
}


