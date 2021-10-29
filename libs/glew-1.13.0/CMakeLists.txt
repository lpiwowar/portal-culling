set(GLEW_SOURCE
        src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW_1130 STATIC
        ${GLEW_SOURCE}
        ${GLEW_INCLUDE}
)

target_link_libraries(GLEW_1130
        ${OPENGL_LIBRARY}
        ${EXTRA_LIBS}
)
