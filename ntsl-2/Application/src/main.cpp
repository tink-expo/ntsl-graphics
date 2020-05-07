// STL
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdio>
#include <algorithm>

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

double mouse_x = 0;
double mouse_y = 0;
bool mouse_pressed = false;

static void mouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
	if (a_button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (a_action == GLFW_PRESS) {
			mouse_pressed = true;
        } else if (a_action == GLFW_RELEASE) {
			mouse_pressed = false;
        }
	}
}

static void cursorPosCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{
	mouse_x = a_xpos;
	mouse_y = a_ypos;
}

glm::vec3 calcIforce(double s_mouse_x, double s_mouse_y)
{
    float n_mouse_x;
    float n_mouse_y;

    float screen_height = g_window_height / 2.0f;
    float screen_width = g_window_width / 2.0f;
    
    n_mouse_y = screen_height - s_mouse_y;
    n_mouse_y -= screen_height / 2.0f;
    n_mouse_y /= (screen_height / 2.0f);

    n_mouse_x = s_mouse_x;
    n_mouse_x -= screen_width / 2.0f;
    n_mouse_x = std::max(-screen_height / 2.0f, 
            std::min(screen_height / 2.0f, n_mouse_x));
    n_mouse_x /= (screen_height / 2.0f);

    return glm::vec3(n_mouse_x, n_mouse_y, 0);
}

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
	glCullFace(GL_BACK);

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
	glfwSetCursorPosCallback(g_window, cursorPosCallback);

    Engine::Camera main_camera;
    glViewport(0, 0, g_framebuffer_width, g_framebuffer_height);
    float ratio = g_framebuffer_width / (float)g_framebuffer_height;
    main_camera.SetProjection(ratio, 60.0f, 0.01f, 1000.0f);
    
    const auto& main_p_data = main_camera.GetProjection();
    float rad_pixel = 
            (main_p_data.zNear * tan(glm::radians(main_p_data.fov / 2)))
            / (float) g_framebuffer_height;

    float grid_size = 0.25f;

    HullMaterial hull_material;
    hull_material.CreateMaterial();
    marchingCubesInitialize(&hull_material, grid_size, rad_pixel);

	Geometry geometry = Geometry();
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Engine::Mesh grid_mesh;
    grid_mesh.SetDrawMode(GL_POINTS);
    geometry.GenerateGrid(&grid_mesh, glm::vec3(-20, -20, -30), glm::vec3(20, 20, -10), grid_size);
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

        // std::cout << elapsed_time << std::endl;

        glfwGetFramebufferSize(g_window, &width, &height);

        if (width != g_framebuffer_width || height != g_framebuffer_height) {
            g_framebuffer_width = width;
            g_framebuffer_height = height;
            g_window_width = (float)width;
            g_window_height = (float)height;

            glViewport(0, 0, width, height);
            main_camera.SetProjection(width / (float)height, 60.0f, 0.01f, 1000.0f);
        }

        if (mouse_pressed) {
            hull_material.UpdateIforce(calcIforce(mouse_x, mouse_y));
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