#include<iostream>
#include<vector>

#include<SDL.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_access.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>



#include<common/objloader.hpp>
#include<common/shader.hpp>

using namespace ge::gl;

int main(int argc,char*argv[]){
  SDL_Init(SDL_INIT_VIDEO);//init. video

  auto window = SDL_CreateWindow("PGR_examples",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1024,768,SDL_WINDOW_OPENGL);

  uint32_t version = 460;//context version
  uint32_t profile = SDL_GL_CONTEXT_PROFILE_CORE;//context profile
  uint32_t flags   = SDL_GL_CONTEXT_DEBUG_FLAG;//context flags
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version/100 );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,(version%100)/10);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK ,profile );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS ,flags );

  auto context = SDL_GL_CreateContext(window);

  ge::gl::init();

  // Set background color to dark blue
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

   // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS); 

  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "basic.vertexshader", "basic.fragmentshader" );
  return 0;
/*
  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  // Read our .obj file
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals; // Won't be used at the moment.
  bool res = loadOBJ("../../scene/pgr_scene.obj", vertices, uvs, normals);

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  auto projId = glGetUniformLocation(programID,"proj");
  auto viewId = glGetUniformLocation(programID,"view");
  auto proj = glm::perspective(glm::half_pi<float>(),1024.f/768.f,0.01f,1000.f);

  float angleX=0.f;
  float angleY=0.f;
  // DOPLNIT
  bool running = true;
  while(running){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT)
        running = false;
      if(event.type == SDL_KEYDOWN){
        float sensitivity = 0.1f;
        if(event.key.keysym.sym == SDLK_a)          angleY += sensitivity;
        if(event.key.keysym.sym == SDLK_d)          angleY -= sensitivity;
        if(event.key.keysym.sym == SDLK_w)          angleX += sensitivity;
        if(event.key.keysym.sym == SDLK_s)          angleX -= sensitivity;
      }
    }

    auto view = 
      glm::translate(glm::mat4(1.f),glm::vec3(0.f,0.f,-10.f))*
      glm::rotate(glm::mat4(1.f),angleY,glm::vec3(0.f,1.f,0.f))*
      glm::rotate(glm::mat4(1.f),angleX,glm::vec3(1.f,0.f,0.f));

    // Set background color to dark blue
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glProgramUniformMatrix4fv(programID,projId,1,GL_FALSE,glm::value_ptr(proj));
    glProgramUniformMatrix4fv(programID,viewId,1,GL_FALSE,glm::value_ptr(view)); 

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
    glDisableVertexAttribArray(0);

    // Use our shader
    glUseProgram(programID);

 
    SDL_GL_SwapWindow(window);
  }

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  return 0;
*/
}
