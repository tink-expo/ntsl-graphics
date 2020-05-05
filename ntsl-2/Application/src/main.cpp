// STL
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdio>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>
#include <Shader.hpp>

// include application headers
#include <DefaultMaterial.hpp>
#include <HullMaterial.hpp>
#include <MarchingCubes.hpp>

#include <Geometry.hpp>

#define _USE_MATH_DEFINES

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

float PI = (float) M_PI;

int main(int argc, char** argv)
{
    if (argc < 2) {
        return -1;
    }
    Engine::Shader::_source_dir = std::string(argv[1]);
    
    // Initialize GLFW library
    if (!glfwInit())
    {
        return -1;
    }

    // Create window and OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    g_window = glfwCreateWindow((int)g_window_width, (int)g_window_height, "NTSL", NULL, NULL);
    if (!g_window)
    {
        const char* description;
        int code = glfwGetError(&description);
        
        if (description) {
            std::cout << description << std::endl;
        }
        
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);

    // Initialize GLEW library
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
        exit(1);
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);

    Engine::Camera main_camera;
    glViewport(0, 0, g_framebuffer_width, g_framebuffer_height);
    float ratio = g_framebuffer_width / (float)g_framebuffer_height;
    main_camera.SetProjection(ratio, 60.0f, 0.01f, 1000.0f);
    main_camera.SetPosition(glm::vec3(0, 1, 10));
    glm::mat4 cam_orientation = glm::rotate(glm::mat4(1.0f), glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cam_orientation = glm::rotate(cam_orientation, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    main_camera.SetOrientation(cam_orientation);

    float grid_size = 1.0f;

    HullMaterial hull_material;
    hull_material.CreateMaterial();
    marchingCubesInitialize(&hull_material, grid_size);

	Geometry geometry = Geometry();
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Engine::Mesh grid_mesh;
    grid_mesh.SetDrawMode(GL_POINTS);
    geometry.GenerateGrid(&grid_mesh, glm::vec3(-2, -2, -2), glm::vec3(2, 2, 2), grid_size);
    Engine::RenderObject grid(&grid_mesh, &hull_material);

    float prev_time = 0;

    int width, height;
    glfwGetFramebufferSize(g_window, &width, &height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        float total_time = (float)glfwGetTime();
        float elapsed_time = total_time - prev_time;
        prev_time = total_time;

        glfwGetFramebufferSize(g_window, &width, &height);

        if (width != g_framebuffer_width || height != g_framebuffer_height) {
            g_framebuffer_width = width;
            g_framebuffer_height = height;
            g_window_width = (float)width;
            g_window_height = (float)height;

            glViewport(0, 0, width, height);
            main_camera.SetProjection(width / (float)height, 60.0f, 0.01f, 1000.0f);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        /* render objects */
        grid.Render(&main_camera);

        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}