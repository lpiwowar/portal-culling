/**
 * @file    scene.cpp
 * @author  Lukas Piwowarski
 * @date    2021 November
 * @brief   This file contains all functions that are connected with the scene
 *          graph. 
 */
#include <cstring>
#include <iostream>
#include <map>

#include <GL/glew.h>

#include "../include/config.h"
#include "../include/objloader.hpp"
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


void makeEdge(Cell_T* leftCell, Portal_T* portal, Cell_T* rightCell) 
{
    assert(portal != nullptr);

    if(leftCell) 
    {
        leftCell->portals.insert(leftCell->portals.end(), {portal});
        portal->leftCell = leftCell;
    }

    if(rightCell) 
    {
        rightCell->portals.insert(rightCell->portals.end(), {portal});
        portal->rightCell = rightCell;
    }
}


template<typename T>
void calcBoundingBox(T* object, glm::vec3 offset=glm::vec3(0,0,0)) {
    glm::vec3 min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    
    for(glm::vec3& it : object->vertices) {
        // std::cout << it.x << " " << it.y << " " << it.z << std::endl; 
        it = it + offset;
        if ( it.x < min.x ) min.x = it.x;
        if ( it.y < min.y ) min.y = it.y;
        if ( it.z < min.z ) min.z = it.z;
        if ( it.x > max.x ) max.x = it.x;
        if ( it.y > max.y ) max.y = it.y;
        if ( it.z > max.z ) max.z = it.z;
    }

    object->boundingBoxMin = min;
    object->boundingBoxMax = max;
    object->boundingBoxMin.y -= 100;
    object->boundingBoxMax.y += 100;

    return;
}

typedef struct hey {
    int* ukamanga;
    int* ukamanga2;
} hey_T;

void getSceneSubblock(Graph_T* graph, 
                                   glm::vec3 offset, 
                                   connectPortals_T* connectPortals, 
                                   std::map<std::string, Cell_T*> cells, 
                                   std::map<std::string, Portal_T*> portals)
{
    static int id = 20;

    Cell_T *cell_1 = new Cell_T;
    *cell_1 = *cells["cell_1"];
    cell_1->id = id++;
    calcBoundingBox(cell_1, offset);
    fillObjectBuffers(cell_1);

    Cell_T *cell_2 = new Cell_T;
    *cell_2 = *cells["cell_2"];
    cell_2->id = id++;
    calcBoundingBox(cell_2, offset);
    fillObjectBuffers(cell_2);

    Cell_T *cell_3 = new Cell_T;
    *cell_3 = *cells["cell_3"];
    cell_3->id = id++;
    calcBoundingBox(cell_3, offset);
    fillObjectBuffers(cell_3);

    Cell_T *cell_4 = new Cell_T;
    *cell_4 = *cells["cell_4"];
    cell_4->id = id++;
    calcBoundingBox(cell_4, offset);
    fillObjectBuffers(cell_4);

    Portal_T *portal_1 = new Portal_T;
    *portal_1 = *portals["portal_1"];
    portal_1->id = id++;
    calcBoundingBox(portal_1, offset);
    fillObjectBuffers(portal_1);

    Portal_T *portal_2 = new Portal_T;
    *portal_2 = *portals["portal_2"];
    portal_2->id = id++;
    calcBoundingBox(portal_2, offset);
    fillObjectBuffers(portal_2);

    Portal_T *portal_3 = new Portal_T;
    *portal_3 = *portals["portal_3"];
    portal_3->id = id++;
    calcBoundingBox(portal_3, offset);
    fillObjectBuffers(portal_3);

    Portal_T *portal_4 = new Portal_T;
    *portal_4 = *portals["portal_4"];
    portal_4->id = id++;
    calcBoundingBox(portal_4, offset);
    fillObjectBuffers(portal_4);

    Portal_T *portal_5 = new Portal_T;
    *portal_5 = *portals["portal_5"];
    portal_5->id = id++;
    calcBoundingBox(portal_5, offset);
    fillObjectBuffers(portal_5);

    Portal_T *portal_6 = new Portal_T;
    *portal_6 = *portals["portal_6"];
    portal_6->id = id++;
    calcBoundingBox(portal_6, offset);
    fillObjectBuffers(portal_6);

    Portal_T *portal_7 = new Portal_T;
    *portal_7 = *portals["portal_7"];
    portal_7->id = id++;
    calcBoundingBox(portal_7, offset);
    fillObjectBuffers(portal_7);

    Portal_T *portal_8 = new Portal_T;
    *portal_8 = *portals["portal_8"];
    portal_8->id = id++;
    calcBoundingBox(portal_8, offset);
    fillObjectBuffers(portal_8);

    graph->portals.insert(graph->portals.end(), {portal_1, portal_2, portal_3, portal_4, portal_5, portal_8});
    graph->cells.insert(graph->cells.end(), {cell_1, cell_2, cell_3, cell_4});

    makeEdge(cell_1, portal_1, cell_2);

    makeEdge(cell_2, portal_2, cell_3);
    makeEdge(cell_2, portal_3, cell_3);
    makeEdge(cell_2, portal_5, cell_4);

    makeEdge(cell_3, portal_4, cell_4);

    makeEdge(cell_4, portal_8, nullptr);

    if (connectPortals->portalRight) 
    {
        if(!connectPortals->portalRight->leftCell) 
        {
            connectPortals->portalRight->leftCell = cell_2;
            connectPortals->portalRight->leftCell->portals.insert(connectPortals->portalRight->leftCell->portals.end(), {connectPortals->portalRight});
        }
        else
        {
            connectPortals->portalRight->rightCell = cell_2;
            connectPortals->portalRight->rightCell->portals.insert(connectPortals->portalRight->rightCell->portals.end(), {connectPortals->portalRight});
        }
    }

    connectPortals->portalRight = portal_8;

    return;
}

Graph_T *createSceneGraph(std::string sceneName) 
{
#if 1
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
        graph->cells.insert(graph->cells.end(), {cell_1, cell_4, cell_4, cell_5});
        graph->portals.insert(graph->portals.end(), {portal_1, portal_4, portal_4, portal_5});
        
        makeEdge(cell_1, portal_1, cell_5);
        makeEdge(cell_4, portal_3, cell_5);
        makeEdge(cell_4, portal_4, cell_5);
        makeEdge(cell_4, portal_5, cell_5);
        
        return graph;
    } 
    else if (sceneName == "pgr_scene2") 
    {
        Cell_T *cell_1 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/room1.obj", cell_1);
        fillObjectBuffers(cell_1);
        calcBoundingBox(cell_1);
        
        Cell_T *cell_2 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/room2.obj", cell_2);
        fillObjectBuffers(cell_2);
        calcBoundingBox(cell_2);

        Cell_T *cell_3 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/room3.obj", cell_3);
        fillObjectBuffers(cell_3);
        calcBoundingBox(cell_3);

        Cell_T *cell_4 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/room4.obj", cell_4);
        fillObjectBuffers(cell_4);
        calcBoundingBox(cell_4);

        Portal_T *portal_1 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/portal1.obj", portal_1);
        fillObjectBuffers(portal_1);
        calcBoundingBox(portal_1);

        Portal_T *portal_2 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/portal2.obj", portal_2);
        fillObjectBuffers(portal_2);
        calcBoundingBox(portal_2);

        Portal_T *portal_3 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/portal3.obj", portal_3);
        fillObjectBuffers(portal_3);
        calcBoundingBox(portal_3);

        Portal_T *portal_4 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/portal4.obj", portal_4);
        fillObjectBuffers(portal_4);
        calcBoundingBox(portal_4);

        Portal_T *portal_5 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/portal5.obj", portal_5);
        fillObjectBuffers(portal_5);
        calcBoundingBox(portal_5);

        Portal_T *portal_6 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/portal6.obj", portal_6);
        fillObjectBuffers(portal_6);
        calcBoundingBox(portal_6);

        Portal_T *portal_7 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/pgr_scene2/portal7.obj", portal_7);
        fillObjectBuffers(portal_7);
        calcBoundingBox(portal_7);


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
#endif
    else if (sceneName == "auto_generated") 
    {
        CellMap_T cells;
        PortalMap_T portals;

        Cell_T* cell_1 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/room1.obj", cell_1);
        cells.insert(CellMapElem_T("cell_1", cell_1));

        Cell_T* cell_2 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/room2.obj", cell_2);
        cells.insert(CellMapElem_T("cell_2", cell_2));

        Cell_T* cell_3 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/room3.obj", cell_3);
        cells.insert(CellMapElem_T("cell_3", cell_3));

        Cell_T* cell_4 = new Cell_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/room4.obj", cell_4);
        cells.insert(CellMapElem_T("cell_4", cell_4));

        Portal_T* portal_1 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal1.obj", portal_1);
        portals.insert(PortalMapElem_T("portal_1", portal_1));

        Portal_T* portal_2 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal2.obj", portal_2);
        portals.insert(PortalMapElem_T("portal_2", portal_2));

        Portal_T* portal_3 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal3.obj", portal_3);
        portals.insert(PortalMapElem_T("portal_3", portal_3));

        Portal_T* portal_4 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal4.obj", portal_4);
        portals.insert(PortalMapElem_T("portal_4", portal_4));

        Portal_T* portal_5 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal5.obj", portal_5);
        portals.insert(PortalMapElem_T("portal_5", portal_5));

        Portal_T* portal_6 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal6.obj", portal_6);
        portals.insert(PortalMapElem_T("portal_6", portal_6));

        Portal_T* portal_7 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal7.obj", portal_7);
        portals.insert(PortalMapElem_T("portal_7", portal_7));

        Portal_T* portal_8 = new Portal_T;
        loadOBJ(SOURCE_DIR "/res/scenes/auto_generated/portal8.obj", portal_8);
        portals.insert(PortalMapElem_T("portal_8", portal_8));

        Graph_T* graph = new Graph_T;
        connectPortals_T connectPortals;
        for(int i = 0; i < 1000; i++) 
        {
            getSceneSubblock(graph, 
                             glm::vec3(0,0,37.125*i), 
                             &connectPortals,
                             cells,
                             portals);
        }
        
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

