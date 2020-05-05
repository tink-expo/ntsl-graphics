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
#include <PickingMaterial.hpp>
#include <PerVertexColorMaterial.hpp>

#include <Geometry.hpp>
#include <picking.hpp>
#include <PickableObject.hpp>
#include <Snowman.hpp>
#include <Fractal.hpp>
#include <Snowflake.hpp>
#include <SnowflakeAnimation.hpp>

#define _USE_MATH_DEFINES

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);

    Engine::Camera main_camera;
    glViewport(0, 0, g_framebuffer_width, g_framebuffer_height);
    float ratio = g_framebuffer_width / (float)g_framebuffer_height;
    main_camera.SetProjection(ratio, 60.0f, 0.01f, 1000.0f);
    main_camera.SetPosition(glm::vec3(0.0f, 7.0f, 7.0f));
    main_camera.SetOrientation(glm::rotate(glm::mat4(1.0f), -(float) M_PI / 4.0f, glm::vec3(1.0f, 0.0f, 0.0f)));

    DefaultMaterial material;
    material.CreateMaterial();

	Geometry geometry = Geometry();

    Engine::Mesh rectangle_mesh;
    auto const& main_projection = main_camera.GetProjection();
    geometry.GenerateAbigRectangle(&rectangle_mesh, 1.0, 1.0);
    Engine::RenderObject rectangle = Engine::RenderObject(&rectangle_mesh, &material);

    float prev_time = 0;

    int width, height;
    glfwGetFramebufferSize(g_window, &width, &height);
    material.UpdateUresolution((float) width, (float) height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        float total_time = (float)glfwGetTime();
        float elapsed_time = total_time - prev_time;
        prev_time = total_time;

        material.UpdateUtime(total_time);

        glfwGetFramebufferSize(g_window, &width, &height);

        if (width != g_framebuffer_width || height != g_framebuffer_height) {
            g_framebuffer_width = width;
            g_framebuffer_height = height;
            g_window_width = (float)width;
            g_window_height = (float)height;

            glViewport(0, 0, width, height);
            main_camera.SetProjection(width / (float)height, 60.0f, 0.01f, 1000.0f);

            material.UpdateUresolution((float) width, (float) height);
        }
        
        // Second Pass: Object Rendering (Slide No. 11)
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* render objects */
        rectangle.Render(&main_camera);

        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}