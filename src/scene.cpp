/**
 * @file    scene.cpp
 * @author  Lukas Piwowarski
 * @date    2021 November
 * @brief   This file contains all functions that are connected with the scene
 *          graph. 
 */

#include <GL/glew.h>

#include "libs/objloader.hpp"
// #include "libs/graph.hpp"


template<typename T> 
void fillObjectBuffers(T * object) {
    glGenBuffers(1, &object->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, object->vertices.size() * sizeof(glm::vec3), &object->vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &object->uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, object->uvs.size() * sizeof(glm::vec2), &object->uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &object->normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->normalBuffer);
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
    } else if (sceneName == "pgr_scene2") {

        Cell_T *cell_1 = new Cell_T;
        loadOBJ("./scenes/pgr_scene2/room1.obj", cell_1);
        fillObjectBuffers(cell_1);
        calcBoundingBox(cell_1);

        Cell_T *cell_2 = new Cell_T;
        loadOBJ("./scenes/pgr_scene2/room2.obj", cell_2);
        fillObjectBuffers(cell_2);
        calcBoundingBox(cell_2);

        Cell_T *cell_3 = new Cell_T;
        loadOBJ("./scenes/pgr_scene2/room3.obj", cell_3);
        fillObjectBuffers(cell_3);
        calcBoundingBox(cell_3);

        Cell_T *cell_4 = new Cell_T;
        loadOBJ("./scenes/pgr_scene2/room4.obj", cell_4);
        fillObjectBuffers(cell_4);
        calcBoundingBox(cell_4);

        Portal_T *portal_1 = new Portal_T;
        loadOBJ("./scenes/pgr_scene2/portal1.obj", portal_1);
        fillObjectBuffers(portal_1);
        calcBoundingBox(portal_1);
        // portal_1->portal_name = "portal1";

        Portal_T *portal_2 = new Portal_T;
        loadOBJ("./scenes/pgr_scene2/portal2.obj", portal_2);
        fillObjectBuffers(portal_2);
        calcBoundingBox(portal_2);
        // portal_2->portal_name = "portal2";

        Portal_T *portal_3 = new Portal_T;
        loadOBJ("./scenes/pgr_scene2/portal3.obj", portal_3);
        fillObjectBuffers(portal_3);
        calcBoundingBox(portal_3);
        // portal_3->portal_name = "portal3";

        Portal_T *portal_4 = new Portal_T;
        loadOBJ("./scenes/pgr_scene2/portal4.obj", portal_4);
        fillObjectBuffers(portal_4);
        calcBoundingBox(portal_4);
        // portal_4->portal_name = "portal4";

        Portal_T *portal_5 = new Portal_T;
        loadOBJ("./scenes/pgr_scene2/portal5.obj", portal_5);
        fillObjectBuffers(portal_5);
        calcBoundingBox(portal_5);
        // portal_5->portal_name = "portal5";

        Portal_T *portal_6 = new Portal_T;
        loadOBJ("./scenes/pgr_scene2/portal6.obj", portal_6);
        fillObjectBuffers(portal_6);
        calcBoundingBox(portal_6);
        // portal_6->portal_name = "portal6";

        Portal_T *portal_7 = new Portal_T;
        loadOBJ("./scenes/pgr_scene2/portal7.obj", portal_7);
        fillObjectBuffers(portal_7);
        calcBoundingBox(portal_7);
        // portal_7->portal_name = "portal7";

        Graph_T *graph = new Graph_T;
        graph->cells.insert(graph->cells.end(), {cell_1, cell_2, cell_3, cell_4});
        graph->portals.insert(graph->portals.end(), {portal_1, portal_2, portal_3, portal_4, portal_5, portal_6, portal_7});

        makeEdge(cell_1, portal_1, cell_2);
        makeEdge(cell_1, portal_7, cell_4);

        makeEdge(cell_2, portal_2, cell_3);
        makeEdge(cell_2, portal_3, cell_3);
        makeEdge(cell_2, portal_5, cell_4);

        makeEdge(cell_3, portal_4, cell_4);

        makeEdge(cell_4, portal_6, cell_1);

        return graph;
    }

    return nullptr;
}


void destroySceneGraph(Graph_T *graph) {
    for (auto const &cell: graph->cells) {
	    glDeleteBuffers(1, &cell->vertexBuffer);
	    glDeleteBuffers(1, &cell->uvBuffer);
	    glDeleteBuffers(1, &cell->normalBuffer);
        delete cell;
    }

    for (auto const &portal: graph->portals) {
        delete portal;
    }

    delete graph;

    return;
}

