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

#include "libs/shader.hpp"
#include "libs/controls.hpp"
#include "libs/objloader.hpp"
#include "libs/text2D.hpp"
#include "scene.hpp"

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
    glBindBuffer(GL_ARRAY_BUFFER, object->vertexbuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    // 2nd attribute buffer : uvs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, object->uvbuffer);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, object->normalbuffer);
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
 *          2) "portal" - set of variables required by ./shaders/vert.* shaders 
 * @param shaderName Name of the group of shaders that should be used.
 * @param programID  ID of the compiled shaders.
 */
void useProgram(std::string shaderName, GLuint programID) {
    if (shaderName == "room") {
	    glUseProgram(programID);

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

    } else if (shaderName == "portal") {
	    glUseProgram(programID);

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
    if (anyFragRendered)
        return true;
    else 
        return false;
}

std::vector<Portal_T *> getVisiblePortals(Cell_T *active_cell, GLuint portalProgramID) {
    glm::mat4 projectionmatrix = getProjectionMatrix();
    glm::mat4 viewmatrix = getViewMatrix();
    glm::mat4 modelmatrix = glm::mat4(1.0);
    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;
    
    std::vector<Portal_T *> visiblePortals;
    for (auto& portal: active_cell->portals) {

        if (portalIsVisible(portal, portalProgramID))
            visiblePortals.insert(visiblePortals.end(), portal);

    }

    return visiblePortals;
}

void portalCulling( Cell_T * cell, 
                    std::vector<unsigned int> &visitedCells,
                    GLuint portalProgramID,
                    GLuint cellProgramID)
{

    if(std::find(visitedCells.begin(), visitedCells.end(), cell->id) != visitedCells.end())
        return;

    visitedCells.insert(visitedCells.end(), {cell->id});

    useProgram("room", cellProgramID);
    drawObject(cell);

    std::vector<Portal_T *> visiblePortals = getVisiblePortals(cell, portalProgramID);
    
    for(const auto &portal: visiblePortals) {
        if(portal->leftCell->id == cell->id) 
            portalCulling(portal->rightCell, visitedCells, portalProgramID, cellProgramID);
        else
            portalCulling(portal->leftCell, visitedCells, portalProgramID, cellProgramID);
    }

    return;
}


int main( void )
{
	// initialise glfw
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

	// open a window and create its opengl context
	window = glfwCreateWindow( 1024, 768, "tutorial 08 - basic shading", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "failed to open glfw window. if you have an intel gpu, they are not 3.3 compatible. try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialize glew
	glewExperimental = true; // needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "failed to initialize glew\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// cull triangles which normal is not towards the camera
	// glEnable(gl_cull_face);

	GLuint vertexarrayid;
	glGenVertexArrays(1, &vertexarrayid);
	glBindVertexArray(vertexarrayid);

	// create and compile our glsl program from the shaders
	GLuint cellProgramID = LoadShaders("./shaders/room.vert", "./shaders/room.frag");
	GLuint portalProgramID = LoadShaders("./shaders/portal.vert", "./shaders/portal.frag");

    Graph_T *graph = createSceneGraph("pgr_scene");

    bool wireframe = false;
    initText2D("textures/Holstein.DDS");
	do{

        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (glfwGetKey( window, GLFW_KEY_M ) == GLFW_PRESS){
		  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
          wireframe = true;
		}

		if (glfwGetKey( window, GLFW_KEY_N ) == GLFW_PRESS){
		  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
          wireframe = false;
		}

        Cell_T *active_cell = NULL;
        active_cell = getCurrentCell(graph);

        GLuint NumPrimitivesQueryID;
        GLint NumPrimitivesQueryResult;
        glGenQueries(1, &NumPrimitivesQueryID);
        glBeginQuery(GL_PRIMITIVES_GENERATED, NumPrimitivesQueryID);

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

            std::vector<unsigned int> visitedCells; 
            portalCulling(active_cell, visitedCells, portalProgramID, cellProgramID);

        }

        glEndQuery(GL_PRIMITIVES_GENERATED);
        glGetQueryObjectiv(NumPrimitivesQueryID, GL_QUERY_RESULT, &NumPrimitivesQueryResult);

        std::string str_num_vertices = "# Primitives: " + std::to_string(NumPrimitivesQueryResult);
        printText2D(str_num_vertices.c_str(), 0, 0, 10);

		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
    

	} // check if the esc key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// cleanup vbo and shader
	glDeleteProgram(cellProgramID);
	glDeleteProgram(portalProgramID);
	glDeleteVertexArrays(1, &vertexarrayid);
    cleanupText2D();
    destroySceneGraph(graph);

	// close opengl window and terminate glfw
	glfwTerminate();

	return 0;
}

