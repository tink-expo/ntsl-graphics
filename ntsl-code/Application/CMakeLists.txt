project(app_project)

file(GLOB APP_SRC "src/*.cpp")
file(GLOB APP_HEADERS "include/*.hpp")

if(MSVC)
    source_group("Header Files" FILES ${APP_HEADERS})

    source_group("Source Files" FILES ${APP_SRC})
endif()

add_executable(App ${APP_SRC} ${APP_HEADERS})

target_include_directories(App
    PUBLIC include
    PRIVATE src 
)

include_directories ( ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} )

link_directories ( ${OPENGL_LIBRARY_DIRS} )

target_link_libraries(App Engine glfw GLEW::GLEW ${OPENGL_LIBRARIES} ${GLM_LIBRARIES} )

set_target_properties(
    App PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")