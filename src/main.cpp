/**
 * @file    main.cpp
 * @author  Lukas Piwowarski
 * @date    2021 November
 * @brief   This file contains code for Portal Occlusion Culling. 
 */


#include <algorithm>
#include <stdio.h>

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../include/shader.hpp"
#include "../include/controls.hpp"
#include "../include/objloader.hpp"
#include "../include/text2D.hpp"
#include "../include/scene.hpp"
#include "../include/config.h"


/**
 * @brief Function that checks whether given @param coordinates are inside 
 *        bounding box of given @param object.
 * @param coordinates Coordinates to be checked.
 * @param object      Object with the bounding box. 
 * @return void
 */
 template<typename T>
bool isInsideObject(glm::vec3 coordinates, T* object) 
{
    if (coordinates.x < object->boundingBoxMax.x &&
        coordinates.y < object->boundingBoxMax.y &&
        coordinates.z < object->boundingBoxMax.z &&
        coordinates.x > object->boundingBoxMin.x &&
        coordinates.y > object->boundingBoxMin.y &&
        coordinates.z > object->boundingBoxMin.z)
        return true;
    else
        return false;
}

/**
 * @brief Function that returns active cell (cell with bounding box that 
 *        contains camera).
 * @param graph Graph that contains candidate active cells.
 * @return Cell_T* 
 */
Cell_T *getCurrentCell(Graph_T *graph) 
{
    glm::vec3 cameraPosition = computeMatricesFromInputs();

    for(auto const& cell: graph->cells) 
    {
        if(isInsideObject(cameraPosition, cell)) 
            return cell;
    }

    return nullptr;
}


/**
 * @brief Renders given object.
 * @param object Object to be rendered.
 * @return void
 */
template<typename T>
void drawObject(T* object, GLuint programID) 
{
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, object->vertexBuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    // 2nd attribute buffer : uvs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, object->uvBuffer);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, object->normalBuffer);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    GLuint translationVectorID = glGetUniformLocation(programID, "translationVector");
    glUniform3f(translationVectorID, 
                object->translationVector.x, 
                object->translationVector.y, 
                object->translationVector.z);

    // draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, object->vertices->size() );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    return;
}

/**
 * @brief Prepare variables that are necessary for a group of shaders (vertex
 *        shader, fragment shader, ...) and activate program that represents
 *        the group. The function supports two "shader 
 *        groups": 
 *          1) "room" - set of variables required by ./shaders/room.* shaders
 *          2) "room_overview_red" - set of variables required by ./shaders/room.*
 *                                   shaders (renders red rooms, with static 
 *                                   camera)
 *          3) "room_overview_green" - set of variables required by ./shaders/room.*
 *                                     shaders (renders green rooms, with static
 *                                     camera)
 *          4) "portal" - set of variables required by ./shaders/vert.* shaders 
 * @param shadersGroupName Name of the group of shaders that should be used.
 * @param programID  ID of the compiled shaders.
 */
void useProgram(std::string shadersGroupName, GLuint programID) 
{
    glUseProgram(programID);

    GLuint matrixID = glGetUniformLocation(programID, "MVP");
    GLuint viewmatrixID = glGetUniformLocation(programID, "V");
    GLuint modelmatrixID = glGetUniformLocation(programID, "M");
    GLuint lightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    GLuint cameraID = glGetUniformLocation(programID, "CameraPosition_worldspace");
    GLuint roomColorID = glGetUniformLocation(programID, "roomColor");
    GLuint portalColorID = glGetUniformLocation(programID, "portalColor");

    if (shadersGroupName == "room") 
    {
        // glm::vec3 roomColor = glm::vec3(0.118,0.565,1);  
        glm::vec3 roomColor = glm::vec3(0.0204,0.355,0.51);  
        glm::vec3 cameraPosition = computeMatricesFromInputs();
        glm::mat4 projectionmatrix = getProjectionMatrix();
        glm::mat4 viewmatrix = getViewMatrix();

	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);

        glUniform3f(lightID, cameraPosition.x, cameraPosition.y + 5, cameraPosition.z);
	    glUniform3f(roomColorID, roomColor.x, roomColor.y, roomColor.z);
    } 
    else if (shadersGroupName == "room_overview_green") 
    {
        glm::vec3 roomColor = glm::vec3(0.0f,1.0f,0.0f);  
        glm::vec3 cameraPosition = computeMatricesFromInputs();
        cameraPosition.y = 120;
        cameraPosition.x -= 20;
        glm::mat4 projectionmatrix = getProjectionMatrix();
        glm::mat4 viewmatrix = glm::lookAt(cameraPosition,cameraPosition+glm::vec3(0.653, -2.0, 0.0f), glm::vec3(0,1,0));

	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);


        glm::vec3 cameraPosition2;
        cameraPosition2 = computeMatricesFromInputs();
        glUniform3f(lightID, cameraPosition2.x, cameraPosition2.y + 10, cameraPosition2.z);
	    glUniform3f(roomColorID, roomColor.x, roomColor.y, roomColor.z);
    } 
    else if (shadersGroupName == "room_overview_red") 
    {
        glm::vec3 roomColor = glm::vec3(1.0f,0.0f,0.0f);  
        glm::vec3 cameraPosition = computeMatricesFromInputs();
        cameraPosition.y = 120;
        cameraPosition.x -= 20;
        glm::mat4 projectionmatrix = getProjectionMatrix();
        glm::mat4 viewmatrix = glm::lookAt(cameraPosition,cameraPosition+glm::vec3(0.653, -2.0, 0.0f), glm::vec3(0,1,0));

	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);

        glm::vec3 cameraPosition2;
        cameraPosition2 = computeMatricesFromInputs();
        glUniform3f(lightID, cameraPosition2.x, cameraPosition2.y + 10, cameraPosition2.z);
	    glUniform3f(roomColorID, roomColor.x, roomColor.y, roomColor.z);
    }
    else if (shadersGroupName == "portal_overview") 
    {
        glm::vec3 roomColor = glm::vec3(0.0f,1.0f,0.0f);  
        glm::vec3 cameraPosition = computeMatricesFromInputs();
        cameraPosition.y = 120;
        cameraPosition.x -= 20;
        glm::mat4 projectionmatrix = getProjectionMatrix();
        glm::mat4 viewmatrix = glm::lookAt(cameraPosition,cameraPosition+glm::vec3(0.653, -2.0, 0.0f), glm::vec3(0,1,0));

	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);

        glm::vec3 cameraPosition2;
        cameraPosition2 = computeMatricesFromInputs();
        glUniform3f(lightID, cameraPosition2.x, cameraPosition2.y + 10, cameraPosition2.z);
	    glUniform3f(roomColorID, roomColor.x, roomColor.y, roomColor.z);
    }
    else if(shadersGroupName == "portal")
    {
	    glm::vec3 cameraPosition = computeMatricesFromInputs();
	    glm::mat4 projectionmatrix = getProjectionMatrix();
	    glm::mat4 viewmatrix = getViewMatrix();
	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);

	    glUniform3f(lightID, cameraPosition.x, cameraPosition.y + 5, cameraPosition.z);
    }
    else if(shadersGroupName == "cube")
    {
        glm::vec3 roomColor = glm::vec3(0.0f,0.0f,1.0f);  
        glm::vec3 cameraPosition = computeMatricesFromInputs();
        cameraPosition.y = 120;
        cameraPosition.x -= 20;
        glm::mat4 projectionmatrix = getProjectionMatrix();
        glm::mat4 viewmatrix = glm::lookAt(cameraPosition,cameraPosition+glm::vec3(0.653, -2.0, 0.0f), glm::vec3(0,1,0));

	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);

        glm::vec3 cameraPosition2;
        cameraPosition2 = computeMatricesFromInputs();
        glUniform3f(lightID, cameraPosition2.x, cameraPosition2.y + 10, cameraPosition2.z);
	    glUniform3f(roomColorID, roomColor.x, roomColor.y, roomColor.z);
    }
}


/**
 * @brief Checks whether given portal is visible by the camera. First it renders
 *        the (invisible) portal and then checks whether any fragment of the
 *        rendered portal is visible using occlusion query.
 * @param portal           Portal to be checked whether it is visible or not. 
 * @param portalProgramID  ID of program to be used to render the portal.
 * @return bool            True  when portal is visible.
 *                         False when portal is not visible.
 */
bool portalIsVisible(Portal_T *portal, GLuint portalProgramID) 
{
    static int i = 0;
    glDepthMask(GL_FALSE);  

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint queryID;
    GLint anyFragRendered;
    glGenQueries(1, &queryID);
    glBeginQuery(GL_ANY_SAMPLES_PASSED, queryID);

    useProgram("portal", portalProgramID);
    drawObject(portal, portalProgramID);

    glEndQuery(GL_ANY_SAMPLES_PASSED);
    glGetQueryObjectiv(queryID, GL_QUERY_RESULT, &anyFragRendered);

    glDisable(GL_BLEND);

    glDepthMask(GL_TRUE);  

    if (anyFragRendered) {
        return true;
    } else {
        return false;
    }
}


/**
 * @brief Returns list all portals connected to activeCell that are visible.
 * @param activeCell     Cell whose portals are to be checked.
 * @param portalProgramID ID of program to be used to render the portal.
 * @return std::vector<Portal_T *> 
 */
std::vector<Portal_T *> getVisiblePortals(Cell_T *activeCell, GLuint portalProgramID) 
{
    glm::mat4 projectionmatrix = getProjectionMatrix();
    glm::mat4 viewmatrix = getViewMatrix();
    glm::mat4 modelmatrix = glm::mat4(1.0);
    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;
    
    std::vector<Portal_T *> visiblePortals;
    for (auto& portal: activeCell->portals) 
    {
        if (portalIsVisible(portal, portalProgramID))
            visiblePortals.insert(visiblePortals.end(), portal);
    }

    return visiblePortals;
}

/**
 * @brief This function does portal occlusion culling of scene represented by 
 *        scene graph G.
 *        
 *        The scene graph G = (V, E) has vertices V that represent sections
 *        of scene (usually rooms) and edges E that represent portals that 
 *        connect scene sections (rooms). The function starts at cell and
 *        than it traverses the scene graph to find all visible cells. The cell
 *        is immediately rendered when the function discoveres that it is visible.
 *
 * @param cell              Active cell (the camera is located inside the room).
 * @param visitedCells      Auxiliary variable used to remember already visited 
 *                          cells.
 * @param portalProgramID   ID of program to be used to render portals.
 * @param cellProgramID     ID of program to be used to render cells.
 * @return std::vector<int> List of IDs of visible rooms.
 */
std::vector<unsigned int> portalCulling(Cell_T * cell, 
                                        std::vector<unsigned int> &visitedCells,
                                        GLuint portalProgramID,
                                        GLuint cellProgramID,
                                        std::string shadersGroupName)
{
    if(std::find(visitedCells.begin(), visitedCells.end(), cell->id) != visitedCells.end())
        return visitedCells;

    visitedCells.insert(visitedCells.end(), {cell->id});

    useProgram(shadersGroupName, cellProgramID);
    drawObject(cell, cellProgramID);

    std::vector<Portal_T *> visiblePortals = getVisiblePortals(cell, portalProgramID);

    for(const auto &portal: visiblePortals) {
        if(portal->leftCell->id == cell->id) 
            visitedCells = portalCulling(portal->rightCell, visitedCells, portalProgramID, cellProgramID, shadersGroupName);
        else
            visitedCells = portalCulling(portal->leftCell, visitedCells, portalProgramID, cellProgramID, shadersGroupName);
    }

    return visitedCells;
}

void drawNeighboringCells(Cell_T& cell, 
                          int depth, 
                          std::vector<uint>& visitedCellsID, 
                          GLuint cellProgramID,
                          std::vector<Cell_T*>& outVisitedCells) 
{
    if(depth < 0)
        return;

    if(std::find(visitedCellsID.begin(), visitedCellsID.end(), cell.id) == visitedCellsID.end()) 
    {
        drawObject(&cell, cellProgramID);
        visitedCellsID.insert(visitedCellsID.end(), {cell.id});
    }

    int cellID = cell.id;
    auto lambdaCheckID = [cellID](Cell_T *cell){ return cell->id == cellID; };
    if(std::find_if(outVisitedCells.begin(), outVisitedCells.end(), lambdaCheckID) == outVisitedCells.end())
    {
        outVisitedCells.insert(outVisitedCells.end(), {&cell});
    }

    for(auto& portal: cell.portals) 
    {
        Cell_T* cell2Draw;
        if (portal->leftCell->id != cell.id)
            cell2Draw = portal->leftCell;
        else
            cell2Draw = portal->rightCell;

        drawNeighboringCells(*cell2Draw, depth-1, visitedCellsID, cellProgramID, outVisitedCells);
    }
}

void drawPortals(std::vector<Cell_T*> cells, GLuint portalProgramID)
{
    useProgram("portals", portalProgramID);
    for(const auto& cell: cells)
    {
        for(const auto& portal: cell->portals)
        {
            drawObject(portal, portalProgramID);
        }
    }
}

Cell_T* getActiveCell(Cell_T* activeCell, Graph_T* graph)
{
    assert(graph != nullptr);
    static int i = 0;
    if(!activeCell)
        return getCurrentCell(graph);

    glm::vec3 cameraPosition = computeMatricesFromInputs();
    if(isInsideObject(cameraPosition, activeCell))
        return activeCell;

    for(const auto& portal: activeCell->portals)
    {
        Cell_T* neighboringCell;
        if(portal->leftCell->id == activeCell->id) 
            neighboringCell = portal->rightCell;
        else
            neighboringCell = portal->leftCell;

        if(isInsideObject(cameraPosition, neighboringCell))
            return neighboringCell;
    }

    return getCurrentCell(graph);
}

int main( void )
{
    const int width = 1848;
    const int height = 768;

	/** Initialise glfw. */
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize glfw\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/** Open a window and create its opengl context. */
	window = glfwCreateWindow( width, height, "PGR - Portal Occlusion Culling", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open glfw window\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/** Initialize glew. */
	glewExperimental = true; /** < needed for core profile */
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "failed to initialize glew\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	/** Ensure we can capture the escape key being pressed below. */
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    /** Hide the mouse and enable unlimited mouvement. */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    /** Set the mouse at the center of the screen. */
    glfwPollEvents();
    glfwSetCursorPos(window, width/2, height/2);

	/** Dark blue background. */
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	/** Enable depth test. */
	glEnable(GL_DEPTH_TEST);

	/** Accept fragment if it closer to the camera than the former one. */
	glDepthFunc(GL_LESS); 

	GLuint vertexarrayid;
	glGenVertexArrays(1, &vertexarrayid);
	glBindVertexArray(vertexarrayid);

	/** Create and compile our glsl program from the shaders. */
	GLuint cellProgramID = LoadShaders(SOURCE_DIR "/shaders/vertexShaders/cell.vert", SOURCE_DIR "/shaders/fragmentShaders/cell.frag");
	GLuint portalProgramID = LoadShaders(SOURCE_DIR "/shaders/vertexShaders/portal.vert", SOURCE_DIR "/shaders/fragmentShaders/portal.frag");

    Graph_T *graph = createSceneGraph("auto_generated3");
    // Graph_T *graph = createSceneGraph("pgr_scene2");


    initText2D(SOURCE_DIR "/res/textures/Holstein.DDS");
    Cell_T *active_cell = nullptr;
    bool portalCullingMode = true;
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    int FPS = 0;
    Cube_T* cube = getCube();

	do{
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){
            FPS = nbFrames;
            nbFrames = 0; 
            lastTime += 1.0;
        }

		if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS)
            portalCullingMode = true;
		else if (glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS)
            portalCullingMode = false;
        else if (glfwGetKey( window, GLFW_KEY_1) == GLFW_PRESS) 
        {
            graph = createSceneGraph("auto_generated");
            active_cell = nullptr;
        }
        else if (glfwGetKey( window, GLFW_KEY_2) == GLFW_PRESS)
        {
            graph = createSceneGraph("auto_generated3");
            active_cell = nullptr;
        }

        /** Set left viewport **/
        glEnable(GL_SCISSOR_TEST);
        glViewport(0, 0, width/2, height);
        glScissor(0, 0, width/2, height);
        if (portalCullingMode)
            glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        else
            glClearColor(0.4f, 0.0f, 0.0f, 0.0f);

        /** Clear the screen. */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
        /** Start query - counting number of rendered primitives */
        GLuint NumPrimitivesQueryID;
        GLint NumPrimitivesQueryResult;
        glGenQueries(1, &NumPrimitivesQueryID);
        glBeginQuery(GL_PRIMITIVES_GENERATED, NumPrimitivesQueryID);


        // active_cell = getCurrentCell(graph);
        active_cell = getActiveCell(active_cell, graph);
        std::vector<unsigned int> visitedCells; 

        if (!active_cell || !portalCullingMode) 
        {
            useProgram("room", cellProgramID);
            for(auto const& cell: graph->cells) 
            {
                drawObject(cell, cellProgramID);
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            useProgram("portal", portalProgramID);
            for(auto const& portal: graph->portals) 
            {
                drawObject(portal, portalProgramID);
            }

            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glDisable(GL_BLEND);
        } 
        else 
        {
            visitedCells = portalCulling(active_cell, visitedCells, portalProgramID, cellProgramID, "room");
        }
        
        /** End query - counting number of rendered primitives */
        glEndQuery(GL_PRIMITIVES_GENERATED);
        glGetQueryObjectiv(NumPrimitivesQueryID, GL_QUERY_RESULT, &NumPrimitivesQueryResult);
        std::string str_num_vertices = "# Primitives: " + std::to_string(NumPrimitivesQueryResult) + " FPS: " + std::to_string(FPS);
        printText2D(str_num_vertices.c_str(), 0, 0, 10);

        /** Set right viewport */
        glViewport((width)/2, 0, width/2, height);
        glScissor((width)/2, 0, width/2, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!active_cell) 
        {
            useProgram("room_overview_red", cellProgramID);
            for (const auto& cell: graph->cells) 
            {
                drawObject(cell, cellProgramID);
            }
        } 
        else 
        {
            /** Print green rooms - visible ones */
            useProgram("room_overview_green", cellProgramID);
            for(auto const& cellID: visitedCells) 
            {
                auto lambdaCheckID = [cellID](Cell_T *cell){ return cell->id == cellID; };
                std::vector<Cell_T*>::iterator it = std::find_if(graph->cells.begin(), graph->cells.end(), lambdaCheckID);
                if (it != graph->cells.end()) 
                {
                    drawObject(*it, cellProgramID);
                }
            }

            /** Print red rooms - unvisible ones */
            if(portalCullingMode)
                useProgram("room_overview_red", cellProgramID);
            else
                useProgram("room_overview_green", cellProgramID);

            std::vector<Cell_T*> outVisitedCells;
            drawNeighboringCells(*active_cell, 5, visitedCells, cellProgramID, outVisitedCells);

           useProgram("portal_overview", portalProgramID);
           drawPortals(outVisitedCells, portalProgramID);

           glm::vec3 cameraPosition = computeMatricesFromInputs();
           cube->translationVector = glm::vec3(cameraPosition.x, 0, cameraPosition.z);
           useProgram("cube", cellProgramID);
           drawObject(cube, cellProgramID);
        }

        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);

		/** Swap buffers. */
		glfwSwapBuffers(window);
		glfwPollEvents();
    
	} /** Check if the esc key was pressed or the window was closed. */
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	/** Cleanup vbo and shader. */
	glDeleteProgram(cellProgramID);
	glDeleteProgram(portalProgramID);
	glDeleteVertexArrays(1, &vertexarrayid);
    cleanupText2D();
    destroySceneGraph(graph);

	/** Close opengl window and terminate glfw. */
	glfwTerminate();

	return 0;
}

