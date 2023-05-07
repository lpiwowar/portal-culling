PGR - Portal Occlusion Culling
==============================
This folder contains implementation of Portal Occlusion Culling algorithm. 
To run the program (portal_culling) it is necessary to first build the program 
using the **build.sh** script. 

How to controll the program:
----------------------------
- To move throughout the model use **W**,**A**,**S**,**D** keys and mouse.
- To turn off the Portal Culling algorithm use key **O**.
- To turn on the Portal Culling algorithm use key **P**.
- To change model use keys **1** and **2**.

Project structure
-----------------
- *external* - Contains external libraries used in the project.
- *include*  - Contains header files used in the projec.
- *doc*      - Contains documentation (written in Czech).
- *res*      - Resources used by the project (models of scenes).
- *shaders*  - Vertex and fragment shaders.
- *src*      - .cpp files 

```
├── build.sh
├── external
├── include
├── PGR-doc-xpiwow00.pdf
├── README.md
├── res
│   ├── scenes
│   └── textures
├── shaders
│   ├── fragmentShaders
│   └── vertexShaders
└── src
```
