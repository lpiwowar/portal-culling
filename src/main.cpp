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

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>


void calcBoundingBox(Object_T * object) {
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

void fillObjectBuffers(Object_T * object) {
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

void drawObject(Object_T * object) {
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
	Object_T room_1;
	bool res1 = loadOBJ("./scene/simple_scene_room_1.obj", &room_1);
	fillObjectBuffers(&room_1);
    calcBoundingBox(&room_1);
    std::cout << "box_min.x: " << room_1.bounding_box_min.x << " box_min.y: " << room_1.bounding_box_min.y << " box_min.z: " << room_1.bounding_box_min.z << std::endl;
    std::cout << "box_max.x: " << room_1.bounding_box_max.x << " box_max.y: " << room_1.bounding_box_max.y << " box_max.z: " << room_1.bounding_box_max.z << std::endl;

	Object_T room_2;
	bool res2 = loadOBJ("./scene/simple_scene_room_2.obj", &room_2);
	fillObjectBuffers(&room_2);
    calcBoundingBox(&room_2);

	Object_T portal_1;
	bool res3 = loadOBJ("./scene/portal.obj", &portal_1);
	fillObjectBuffers(&portal_1);
    calcBoundingBox(&portal_1);
    
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

		useProgram("room", room_programID);
		drawObject(&room_1);
		drawObject(&room_2);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		useProgram("portal", portal_programID);
		drawObject(&portal_1);

        if (!wireframe)
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDisable(GL_BLEND);

		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // check if the esc key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// cleanup vbo and shader
	glDeleteBuffers(1, &room_1.vertexbuffer);
	glDeleteBuffers(1, &room_1.uvbuffer);
	glDeleteBuffers(1, &room_1.normalbuffer);
	glDeleteProgram(room_programID);
	glDeleteVertexArrays(1, &vertexarrayid);

	// close opengl window and terminate glfw
	glfwTerminate();

	return 0;
}

