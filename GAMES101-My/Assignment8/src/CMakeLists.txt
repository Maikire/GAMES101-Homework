# Application source
set(APPLICATION_SOURCE application.cpp application.h getopt.c getopt.h main.cpp
    mass.h rope.cpp rope.h spring.h
)

#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
  ${GLEW_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
  ${CGL_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)

#-------------------------------------------------------------------------------
# Set link directories
#-------------------------------------------------------------------------------
link_directories(
  ${GLEW_LIBRARY_DIRS}
  ${GLFW_LIBRARY_DIRS}
  ${CGL_LIBRARY_DIRS}
  ${FREETYPE_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Add executable
#-------------------------------------------------------------------------------
add_executable(Assignment8 ${APPLICATION_SOURCE})

target_link_libraries(Assignment8
    CGL ${CGL_LIBRARIES}
    glew ${GLEW_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${CMAKE_THREADS_INIT}
)

#-------------------------------------------------------------------------------
# Platform-specific configurations for target
#-------------------------------------------------------------------------------
if(APPLE)
  set_property( TARGET Assignment8 APPEND_STRING PROPERTY COMPILE_FLAGS
                "-Wno-deprecated-declarations -Wno-c++11-extensions")
endif(APPLE)

# Put executable in build directory root
set(EXECUTABLE_OUTPUT_PATH ..)

# Install to project root
install(TARGETS Assignment8 DESTINATION ${Assignment8_SOURCE_DIR})
