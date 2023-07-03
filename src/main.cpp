#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/vec2.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>


#include<iostream>
#include"Renderer/ShaderProgram.h"
#include"Resources/ResourceManager.h"
#include"Renderer/Texture2D.h"
#include"Renderer/Sprite.h"
#include"Game/Game.h"
using namespace std;


glm::ivec2 g_windowSize(640, 480);
Game g_game(g_windowSize);


void glfwWindowSizeCallback(GLFWwindow * pWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0,0, g_windowSize.x, g_windowSize.y);
}
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_game.setkey(key,action);
}
static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

int main(int argc, char** argv)
{
    
    glfwSetErrorCallback(&glfwError);
    /* Initialize the library */
    if (!glfwInit()) {
        cout << "!glfwInit fail" << endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle City", nullptr, nullptr);
    if (pWindow==nullptr)
    {
        cout << "glfwCreateWindow failed!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize OpenGL context" << endl;
        return -1;
    }
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGl version"<< glGetString(GL_VERSION) << endl;

    glClearColor(0, 0, 0, 1);

    
    {
        ResourceManager::setExecutablePath(argv[0]);
        g_game.init();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            g_game.render();
            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
        ResourceManager::unloadAllResources();
    }
    glfwTerminate();
    return 0;
}