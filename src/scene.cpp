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


template<typename T> 
void fillObjectBuffers(T * object) {
    glGenBuffers(1, &object->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, object->vertices->size() * sizeof(glm::vec3), &object->vertices->front(), GL_STATIC_DRAW);

    glGenBuffers(1, &object->uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, object->uvs->size() * sizeof(glm::vec2), &object->uvs->front(), GL_STATIC_DRAW);

    glGenBuffers(1, &object->normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, object->normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, object->normals->size() * sizeof(glm::vec3), &object->normals->front(), GL_STATIC_DRAW);
	
    return;
}

Cube_T* getCube()
{
    Cube_T* cube = new Cube_T;
    loadOBJ(SOURCE_DIR "/res/cube.obj", cube);
    fillObjectBuffers(cube);

    return cube;
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
void calcBoundingBox(T* object) {
    glm::vec3 min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    
    for(glm::vec3& it : *object->vertices) {
        // std::cout << it.x << " " << it.y << " " << it.z << std::endl; 
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

Cell_T* getNewCell() 
{
    Cell_T* cell = new Cell_T;
    cell->vertices = new std::vector<glm::vec3>;
    cell->uvs = new std::vector<glm::vec2>;
    cell->normals = new std::vector<glm::vec3>;

    return cell;
}

Portal_T* getNewPortal() 
{
    Portal_T* portal = new Portal_T;
    portal->vertices = new std::vector<glm::vec3>;
    portal->uvs = new std::vector<glm::vec2>;
    portal->normals = new std::vector<glm::vec3>;

    return portal;
}

void getSceneSubblock(Graph_T* graph, 
                      glm::vec3 translationVector, 
                      connectPortals_T* connectPortals, 
                      std::map<std::string, Cell_T*> cells, 
                      std::map<std::string, Portal_T*> portals)
{
    static int id = 20;

    Cell_T *cell_1 = getNewCell();
    *cell_1 = *cells["cell_1"];
    cell_1->id = id++;
    cell_1->translationVector = translationVector;
    cell_1->boundingBoxMin += translationVector; 
    cell_1->boundingBoxMax += translationVector; 

    Cell_T *cell_2 = new Cell_T;
    *cell_2 = *cells["cell_2"];
    cell_2->id = id++;
    cell_2->translationVector = translationVector;
    cell_2->boundingBoxMin += translationVector; 
    cell_2->boundingBoxMax += translationVector; 

    Cell_T *cell_3 = new Cell_T;
    *cell_3 = *cells["cell_3"];
    cell_3->id = id++;
    cell_3->translationVector = translationVector;
    cell_3->boundingBoxMin += translationVector; 
    cell_3->boundingBoxMax += translationVector; 

    Cell_T *cell_4 = new Cell_T;
    *cell_4 = *cells["cell_4"];
    cell_4->id = id++;
    cell_4->translationVector = translationVector;
    cell_4->boundingBoxMin += translationVector; 
    cell_4->boundingBoxMax += translationVector; 

    Portal_T *portal_1 = new Portal_T;
    *portal_1 = *portals["portal_1"];
    portal_1->id = id++;
    portal_1->translationVector = translationVector;
    portal_1->boundingBoxMin += translationVector; 
    portal_1->boundingBoxMax += translationVector; 

    Portal_T *portal_2 = new Portal_T;
    *portal_2 = *portals["portal_2"];
    portal_2->id = id++;
    portal_2->translationVector = translationVector;
    portal_2->boundingBoxMin += translationVector; 
    portal_2->boundingBoxMax += translationVector; 

    Portal_T *portal_3 = new Portal_T;
    *portal_3 = *portals["portal_3"];
    portal_3->id = id++;
    portal_3->translationVector = translationVector;
    portal_3->boundingBoxMin += translationVector; 
    portal_3->boundingBoxMax += translationVector; 

    Portal_T *portal_4 = new Portal_T;
    *portal_4 = *portals["portal_4"];
    portal_4->id = id++;
    portal_4->translationVector = translationVector;
    portal_4->boundingBoxMin += translationVector; 
    portal_4->boundingBoxMax += translationVector; 

    Portal_T *portal_5 = new Portal_T;
    *portal_5 = *portals["portal_5"];
    portal_5->id = id++;
    portal_5->translationVector = translationVector;
    portal_5->boundingBoxMin += translationVector; 
    portal_5->boundingBoxMax += translationVector; 
/*
    Portal_T *portal_6 = new Portal_T;
    *portal_6 = *portals["portal_6"];
    portal_6->id = id++;
    portal_6->translationVector = translationVector;
    portal_6->boundingBoxMin += translationVector; 
    portal_6->boundingBoxMax += translationVector; 

    Portal_T *portal_7 = new Portal_T;
    *portal_7 = *portals["portal_7"];
    portal_7->id = id++;
    portal_7->translationVector = translationVector;
    portal_7->boundingBoxMin += translationVector; 
    portal_7->boundingBoxMax += translationVector; 
*/
    Portal_T *portal_8 = new Portal_T;
    *portal_8 = *portals["portal_8"];
    portal_8->id = id++;
    portal_8->translationVector = translationVector;
    portal_8->boundingBoxMin += translationVector; 
    portal_8->boundingBoxMax += translationVector; 

    // WARNING - ODKOMENTOVAT!!!
    graph->portals.insert(graph->portals.end(), {portal_1, portal_2, portal_3, portal_4, portal_5, portal_8});
    // graph->portals.insert(graph->portals.end(), {portal_1, portal_2, portal_3, portal_4, portal_5});
    graph->cells.insert(graph->cells.end(), {cell_1, cell_2, cell_3, cell_4});
    // graph->cells.insert(graph->cells.end(), {cell_1});


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
    assert(sceneName == "auto_generated" ||
           sceneName == "auto_generated3");

    std::string modelDir;
    if (sceneName == "auto_generated")
        modelDir = SOURCE_DIR "/res/scenes/auto_generated2/";
    else if (sceneName == "auto_generated3")
        modelDir = SOURCE_DIR "/res/scenes/auto_generated3/";

    CellMap_T cells;
    PortalMap_T portals;

    Cell_T* cell_1 = getNewCell();
    cell_1->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "room1.obj").c_str(), cell_1);
    cells.insert(CellMapElem_T("cell_1", cell_1));
    calcBoundingBox(cell_1);
    fillObjectBuffers(cell_1);

    Cell_T* cell_2 = getNewCell();
    cell_2->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "room2.obj").c_str(), cell_2);
    cells.insert(CellMapElem_T("cell_2", cell_2));
    calcBoundingBox(cell_2);
    fillObjectBuffers(cell_2);

    Cell_T* cell_3 = getNewCell();
    cell_3->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "room3.obj").c_str(), cell_3);
    cells.insert(CellMapElem_T("cell_3", cell_3));
    calcBoundingBox(cell_3);
    fillObjectBuffers(cell_3);

    Cell_T* cell_4 = getNewCell();
    cell_4->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "room4.obj").c_str(), cell_4);
    cells.insert(CellMapElem_T("cell_4", cell_4));
    calcBoundingBox(cell_4);
    fillObjectBuffers(cell_4);

    Portal_T* portal_1 = getNewPortal();
    portal_1->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "portal1.obj").c_str(), portal_1);
    portals.insert(PortalMapElem_T("portal_1", portal_1));
    calcBoundingBox(portal_1);
    fillObjectBuffers(portal_1);

    Portal_T* portal_2 = getNewPortal();
    portal_2->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "portal2.obj").c_str(), portal_2);
    portals.insert(PortalMapElem_T("portal_2", portal_2));
    calcBoundingBox(portal_2);
    fillObjectBuffers(portal_2);

    Portal_T* portal_3 = getNewPortal();
    portal_3->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "portal3.obj").c_str(), portal_3);
    portals.insert(PortalMapElem_T("portal_3", portal_3));
    calcBoundingBox(portal_3);
    fillObjectBuffers(portal_3);

    Portal_T* portal_4 = getNewPortal();
    portal_4->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "portal4.obj").c_str(), portal_4);
    portals.insert(PortalMapElem_T("portal_4", portal_4));
    calcBoundingBox(portal_4);
    fillObjectBuffers(portal_4);

    Portal_T* portal_5 = getNewPortal();
    portal_5->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "portal5.obj").c_str(), portal_5);
    portals.insert(PortalMapElem_T("portal_5", portal_5));
    calcBoundingBox(portal_5);
    fillObjectBuffers(portal_5);

    Portal_T* portal_8 = getNewPortal();
    portal_8->translationVector = glm::vec3(0,0,0);
    loadOBJ((modelDir + "portal8.obj").c_str(), portal_8);
    portals.insert(PortalMapElem_T("portal_8", portal_8));
    calcBoundingBox(portal_8);
    fillObjectBuffers(portal_8);

    Graph_T* graph = new Graph_T;
    connectPortals_T connectPortals;
    for(int i = 0; i < 6000; i++) 
    {
        getSceneSubblock(graph, 
                            glm::vec3(0,0,37*i), 
                            &connectPortals,
                            cells,
                            portals);
    }

    return graph;
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

