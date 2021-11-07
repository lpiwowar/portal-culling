#ifndef SCENEHPP
#define SCENEHPP

#include "libs/graph.hpp"

Graph_T *createSceneGraph(std::string sceneName);
void destroySceneGraph(Graph_T *graph);

#endif
