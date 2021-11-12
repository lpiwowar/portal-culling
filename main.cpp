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

#include "include/shader.hpp"
#include "include/controls.hpp"
#include "include/objloader.hpp"
#include "include/text2D.hpp"
#include "include/scene.hpp"
#include "include/config.hpp"

/**
 * @brief Function that checks whether given @param coordinates are inside 
 *        bounding box of given @param object.
 * @param coordinates Coordinates to be checked.
 * @param object      Object with the bounding box. 
 * @return void
 */
bool isInsideObject(glm::vec3 coordinates, Cell_T * object) {
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
Cell_T *getCurrentCell(Graph_T *graph) {
    glm::vec3 cameraPosition = computeMatricesFromInputs();

    for(auto const& cell: graph->cells) {
        if(isInsideObject(cameraPosition, cell)) {
            return cell;
        } 
    }

    return nullptr;
}


/**
 * @brief Renders given object.
 * @param object Object to be rendered.
 * @return void
 */
template<typename T>
void drawObject(T * object) {
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

    // draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, object->vertices.size() );

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
void useProgram(std::string shadersGroupName, GLuint programID) {
    glUseProgram(programID);

    if (shadersGroupName == "room" || shadersGroupName == "room_overview_green" || shadersGroupName == "room_overview_red" ) {

        GLuint matrixID = glGetUniformLocation(programID, "MVP");
	    GLuint viewmatrixID = glGetUniformLocation(programID, "V");
	    GLuint modelmatrixID = glGetUniformLocation(programID, "M");
	    GLuint lightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	    GLuint cameraID = glGetUniformLocation(programID, "CameraPosition_worldspace");
	    GLuint roomColorID = glGetUniformLocation(programID, "roomColor");
        
        glm::vec3 cameraPosition;
	    glm::mat4 projectionmatrix;
        glm::mat4 viewmatrix;
        glm::vec3 roomColor;
        
        if (shadersGroupName == "room") {
            roomColor = glm::vec3(0.118,0.565,1);  
        } else if (shadersGroupName == "room_overview_green") {
            roomColor = glm::vec3(0.0f,1.0f,0.0f);  
        } else if (shadersGroupName == "room_overview_red") {
            roomColor = glm::vec3(1.0f,0.0f,0.0f);  
        }

        if (shadersGroupName == "room") {
            cameraPosition = computeMatricesFromInputs();
        } else if (shadersGroupName == "room_overview_green" || shadersGroupName == "room_overview_red" ) {
            cameraPosition = glm::vec3(-5,60,10);
        }
        
        if (shadersGroupName == "room") {
            projectionmatrix = getProjectionMatrix();
        } else if (shadersGroupName == "room_overview_green" || shadersGroupName == "room_overview_red" ) {
            projectionmatrix = getProjectionMatrix();
        }
        
        if (shadersGroupName == "room") {
            viewmatrix = getViewMatrix();
        } else if (shadersGroupName == "room_overview_green" || shadersGroupName == "room_overview_red" ) {
            viewmatrix = glm::lookAt(cameraPosition,cameraPosition+glm::vec3(0.653, -2.0, 0.0f), glm::vec3(0,1,0));
        }

	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);
        
        if (shadersGroupName == "room") {
            glUniform3f(lightID, cameraPosition.x, cameraPosition.y + 5, cameraPosition.z);
        } else if (shadersGroupName == "room_overview_green" || shadersGroupName == "room_overview_red") {
            glUniform3f(lightID, 10, 20, 10);
        }

	    glUniform3f(roomColorID, roomColor.x, roomColor.y, roomColor.z);

    } else if (shadersGroupName == "portal") {

	    GLuint matrixID = glGetUniformLocation(programID, "MVP");
	    GLuint viewmatrixID = glGetUniformLocation(programID, "V");
	    GLuint modelmatrixID = glGetUniformLocation(programID, "M");
	    GLuint lightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	    GLuint cameraID = glGetUniformLocation(programID, "CameraPosition_worldspace");

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
bool portalIsVisible(Portal_T *portal, GLuint portalProgramID) {

    glDepthMask(GL_FALSE);  
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint queryID;
    GLint anyFragRendered;
    glGenQueries(1, &queryID);
    glBeginQuery(GL_ANY_SAMPLES_PASSED, queryID);

    useProgram("portal", portalProgramID);
    drawObject(portal);

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
std::vector<Portal_T *> getVisiblePortals(Cell_T *activeCell, GLuint portalProgramID) {
    glm::mat4 projectionmatrix = getProjectionMatrix();
    glm::mat4 viewmatrix = getViewMatrix();
    glm::mat4 modelmatrix = glm::mat4(1.0);
    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;
    
    std::vector<Portal_T *> visiblePortals;
    for (auto& portal: activeCell->portals) {
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
    drawObject(cell);

    std::vector<Portal_T *> visiblePortals = getVisiblePortals(cell, portalProgramID);
    
    for(const auto &portal: visiblePortals) {
        if(portal->leftCell->id == cell->id) 
            visitedCells = portalCulling(portal->rightCell, visitedCells, portalProgramID, cellProgramID, shadersGroupName);
        else
            visitedCells = portalCulling(portal->leftCell, visitedCells, portalProgramID, cellProgramID, shadersGroupName);
    }

    return visitedCells;
}

int main( void )
{
    const int width = 1848;
    const int height = 768;

	/** Initialise glfw. */
	if( !glfwInit() )
	{
		fprintf( stderr, "failed to initialize glfw\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // to make macos happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/** Open a window and create its opengl context. */
	window = glfwCreateWindow( width, height, "tutorial 08 - basic shading", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "failed to open glfw window. if you have an intel gpu, they are not 3.3 compatible. try the 2.1 version of the tutorials.\n" );
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

	/** Cull triangles which normal is not towards the camera. */
	// glEnable(gl_cull_face);

	GLuint vertexarrayid;
	glGenVertexArrays(1, &vertexarrayid);
	glBindVertexArray(vertexarrayid);

	/** Create and compile our glsl program from the shaders. */
	GLuint cellProgramID = LoadShaders(SOURCE_DIR "/shaders/room.vert", SOURCE_DIR "/shaders/room.frag");
	GLuint portalProgramID = LoadShaders(SOURCE_DIR "/shaders/portal.vert", SOURCE_DIR "/shaders/portal.frag");

    Graph_T *graph = createSceneGraph("pgr_scene2");

    bool wireframe = false;
    initText2D( SOURCE_DIR "/textures/Holstein.DDS");
	do{
        
        /** Set left viewport **/
        glEnable(GL_SCISSOR_TEST);
        glViewport(0, 0, width/2, height);
        glScissor(0, 0, width/2, height);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        /** Clear the screen. */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		if (glfwGetKey( window, GLFW_KEY_M ) == GLFW_PRESS){
		  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
          wireframe = true;
		}

		if (glfwGetKey( window, GLFW_KEY_N ) == GLFW_PRESS){
		  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
          wireframe = false;
		}
        
        /** Start query - counting number of rendered primitives */
        GLuint NumPrimitivesQueryID;
        GLint NumPrimitivesQueryResult;
        glGenQueries(1, &NumPrimitivesQueryID);
        glBeginQuery(GL_PRIMITIVES_GENERATED, NumPrimitivesQueryID);

        Cell_T *active_cell = NULL;
        active_cell = getCurrentCell(graph);
        std::vector<unsigned int> visitedCells; 

        if (!active_cell) {
            useProgram("room", cellProgramID);
            for(auto const& cell: graph->cells) {
                drawObject(cell);
            }
   
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            useProgram("portal", portalProgramID);
            for(auto const& portal: graph->portals) {
                drawObject(portal);
            }

            if (!wireframe)
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glDisable(GL_BLEND);
        
    
        } else {
            visitedCells = portalCulling(active_cell, visitedCells, portalProgramID, cellProgramID, "room");

        }
        
        /** End query - counting number of rendered primitives */
        glEndQuery(GL_PRIMITIVES_GENERATED);
        glGetQueryObjectiv(NumPrimitivesQueryID, GL_QUERY_RESULT, &NumPrimitivesQueryResult);
        std::string str_num_vertices = "# Primitives: " + std::to_string(NumPrimitivesQueryResult);
        printText2D(str_num_vertices.c_str(), 0, 0, 10);

        /** Set right viewport */
        glViewport((width)/2, 0, width/2, height);
        glScissor((width)/2, 0, width/2, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!active_cell) {

            useProgram("room_overview_red", cellProgramID);
            for (const auto& cell: graph->cells) {
                drawObject(cell);
            }
               
        } else {
            /** Print green rooms - visible ones */
            useProgram("room_overview_green", cellProgramID);
            for(auto const& cellID: visitedCells) {
                auto lambdaCheckID = [cellID](Cell_T *cell){ return cell->id == cellID; };
                std::vector<Cell_T*>::iterator it = std::find_if(graph->cells.begin(), graph->cells.end(), lambdaCheckID);
                if (it != graph->cells.end()) {
                    drawObject(*it);
                }
            }

            /** Print red rooms - unvisible ones */
            useProgram("room_overview_red", cellProgramID);
            for(auto const& cellID: visitedCells) {
                for(auto const& cell: graph->cells) {
                    if (std::find(visitedCells.begin(), visitedCells.end(), cell->id) == visitedCells.end()) {
                        drawObject(cell);
                    }
                }
            } 
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

