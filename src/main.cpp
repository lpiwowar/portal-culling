// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include 
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include "text2D.hpp"

void makeEdge(Cell_T *left_cell, Portal_T *portal, Cell_T *right_cell) {

    left_cell->portals.insert(left_cell->portals.end(), {portal});
    right_cell->portals.insert(right_cell->portals.end(), {portal});
    portal->left_cell = left_cell;
    portal->right_cell = right_cell;

}

bool isInsideObject(glm::vec3 coordinates, Cell_T * object) {
    if (coordinates.x < object->bounding_box_max.x &&
        coordinates.y < object->bounding_box_max.y &&
        coordinates.z < object->bounding_box_max.z &&
        coordinates.x > object->bounding_box_min.x &&
        coordinates.y > object->bounding_box_min.y &&
        coordinates.z > object->bounding_box_min.z)
        return true;
    else
        return false;
}

Cell_T *getCurrentCell(Graph_T *graph) {
    glm::vec3 camera_position = computeMatricesFromInputs();

    for(auto const& cell: graph->cells) {
        if(isInsideObject(camera_position, cell)) {
            return cell;
        } 
    }

    return NULL;
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

    object->bounding_box_min = min;
    object->bounding_box_max = max;

    return;
}

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

void useProgram(std::string shader_name, GLuint programid) {
    if (shader_name == "room") {
	    glUseProgram(programid);

        GLuint matrixID = glGetUniformLocation(programid, "MVP");
	    GLuint viewmatrixID = glGetUniformLocation(programid, "V");
	    GLuint modelmatrixID = glGetUniformLocation(programid, "M");
	    GLuint lightID = glGetUniformLocation(programid, "LightPosition_worldspace");
	    GLuint cameraID = glGetUniformLocation(programid, "CameraPosition_worldspace");

	    glm::vec3 camera_position = computeMatricesFromInputs();
	    glm::mat4 projectionmatrix = getProjectionMatrix();
	    glm::mat4 viewmatrix = getViewMatrix();
	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);
        // std::cout << "camera.x" << camera_position.x << " camera.y: " << camera_position.y << " camera.z " << camera_position.z << std::endl;
	    glUniform3f(lightID, camera_position.x, camera_position.y + 5, camera_position.z);

    } else if (shader_name == "portal") {
	    glUseProgram(programid);

	    GLuint matrixID = glGetUniformLocation(programid, "MVP");
	    GLuint viewmatrixID = glGetUniformLocation(programid, "V");
	    GLuint modelmatrixID = glGetUniformLocation(programid, "M");
	    GLuint lightID = glGetUniformLocation(programid, "LightPosition_worldspace");
	    GLuint cameraID = glGetUniformLocation(programid, "CameraPosition_worldspace");

	    glm::vec3 camera_position = computeMatricesFromInputs();
	    glm::mat4 projectionmatrix = getProjectionMatrix();
	    glm::mat4 viewmatrix = getViewMatrix();
	    glm::mat4 modelmatrix = glm::mat4(1.0);
	    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

	    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
	    glUniformMatrix4fv(modelmatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	    glUniformMatrix4fv(viewmatrixID, 1, GL_FALSE, &viewmatrix[0][0]);

	    glUniform3f(lightID, camera_position.x, camera_position.y + 5, camera_position.z);

    }
}

std::vector<Portal_T *> getVisiblePortals(Cell_T *active_cell) {
    glm::mat4 projectionmatrix = getProjectionMatrix();
    glm::mat4 viewmatrix = getViewMatrix();
    glm::mat4 modelmatrix = glm::mat4(1.0);
    glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;
    
    std::vector<Portal_T *> visiblePortals;
    for (auto& portal: active_cell->portals) {
        
        bool visiblePortal = false;
        for (const auto& vertex: portal->vertices) {
            glm::vec4 homog_projected_vertex = mvp * vec4(vertex, 1);
            glm::vec3 projected_vertex = vec3(homog_projected_vertex.x / homog_projected_vertex.w,
                                              homog_projected_vertex.y / homog_projected_vertex.w,
                                              homog_projected_vertex.z / homog_projected_vertex.w);
            
            // TODO - upravit tuhle podminku!!! 
            if (projected_vertex.x < 1 && projected_vertex.x > -1 &&
                projected_vertex.y < 1 && projected_vertex.y > -1)
                visiblePortal = true; 
        }

        if (visiblePortal)
            visiblePortals.insert(visiblePortals.end(), portal);
    }

    return visiblePortals;
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
	GLuint room_programID = LoadShaders("./shaders/room.vert", "./shaders/room.frag");
	GLuint portal_programID = LoadShaders("./shaders/portal.vert", "./shaders/portal.frag");

	// read our .obj file
	Cell_T cell_1;
	bool res1 = loadOBJ("./scene/simple_scene_cell_1.obj", &cell_1);
	fillObjectBuffers(&cell_1);
    calcBoundingBox(&cell_1);

	Cell_T cell_2;
	bool res2 = loadOBJ("./scene/simple_scene_cell_2.obj", &cell_2);
	fillObjectBuffers(&cell_2);
    calcBoundingBox(&cell_2);

	Portal_T portal_1;
	bool res3 = loadOBJ("./scene/portal.obj", &portal_1);
	fillObjectBuffers(&portal_1);
    calcBoundingBox(&portal_1);
    
    ///////////////////////////////////////////////////////////
    //                   GRAPH DEFINITION                    //
    ///////////////////////////////////////////////////////////
    
    Graph_T graph;
    graph.portals.insert(graph.portals.end(), {&portal_1});
    graph.cells.insert(graph.cells.end(), {&cell_1, &cell_2});
    
    makeEdge(&cell_1, &portal_1, &cell_2);

    bool wireframe = false;
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

#if 0
	    glm::vec3 camera_position = computeMatricesFromInputs();
        if (isInsideObject(camera_position, &cell_1))
            std::cout << "INSIDE ROOM_1" << std::endl;
#endif

        Cell_T *active_cell = NULL;
        active_cell = getCurrentCell(&graph);
        
        initText2D("./textures/Holstein.DDS");
        printText2D("#Vertices: ", 10, 0, 0);

        if (!active_cell) {
            useProgram("room", room_programID);
            for(auto const& cell: graph.cells) {
		        drawObject(cell);
            }
        
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            useProgram("portal", portal_programID);
            for(auto const& portal: graph.portals) {
                drawObject(portal);
            }

            if (!wireframe)
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glDisable(GL_BLEND);

        } else {
            useProgram("room", room_programID);
            GLuint queryID;
            GLint succ;
            
#if 0
            glGenQueries(1, &queryID);
            glBeginQuery(GL_ANY_SAMPLES_PASSED, queryID);
            drawObject(active_cell);
            glEndQuery(GL_ANY_SAMPLES_PASSED);
            glGetQueryObjectiv(queryID, GL_QUERY_RESULT, &succ);
#endif

    /*
            std::vector<Portal_T *> visible_portals = getVisiblePortals(active_cell);
            for (const auto& portal: visible_portals) {
               useProgram("room", room_programID);

                // TODO - Tady je taky neco spatne - ten portal se nejak spatne renderuj
                if (portal->left_cell->id == active_cell->id) {
                    // std::cout << "A: " << portal->left_cell->id << std::endl;
                    drawObject(portal->right_cell);
                } else {
                    // std::cout << "B: " << portal->right_cell->id << std::endl;
                    drawObject(portal->left_cell);
                }
                
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                useProgram("portal", portal_programID);
                drawObject(portal);
                if (!wireframe)
                    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glDisable(GL_BLEND);
            }
    */
        }

#if 0
        Cell_T *active_cell = NULL;
        active_cell = getCurrentCell(&graph);
        if (active_cell != NULL)
            std::cout << "ACTIVE CELL ID: " << active_cell->id << std::endl;
        else
            std::cout << "NULL" << std::endl;

		useProgram("room", room_programID);
		drawObject(&cell_1);
		drawObject(&cell_2);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		useProgram("portal", portal_programID);
		drawObject(&portal_1);

        if (!wireframe)
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDisable(GL_BLEND);

#endif
		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // check if the esc key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// cleanup vbo and shader
	glDeleteBuffers(1, &cell_1.vertexbuffer);
	glDeleteBuffers(1, &cell_1.uvbuffer);
	glDeleteBuffers(1, &cell_1.normalbuffer);
	glDeleteProgram(room_programID);
	glDeleteVertexArrays(1, &vertexarrayid);

	// close opengl window and terminate glfw
	glfwTerminate();

	return 0;
}

