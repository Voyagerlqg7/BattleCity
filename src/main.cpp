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
#include"Renderer/Renderer.h"
#include"Game/Game.h"
using namespace std;


glm::ivec2 g_windowSize(13*16,14*16);
unique_ptr<Game> g_game= make_unique<Game>(g_windowSize);


void glfwWindowSizeCallback(GLFWwindow * pWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;

    const float map_aspect_ratio = 13.f / 14.f;
    unsigned int viewPortWidth = g_windowSize.x;
    unsigned int viewPortHeight = g_windowSize.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;

    if (g_windowSize.x / g_windowSize.y > map_aspect_ratio) {
        viewPortWidth = g_windowSize.y * map_aspect_ratio;
        viewPortLeftOffset = (g_windowSize.x - viewPortWidth) / 2;
    }
    else {
        viewPortHeight = g_windowSize.x / map_aspect_ratio;
        viewPortBottomOffset = (g_windowSize.y - viewPortHeight) / 2;
    }
    RenderEngine::Renderer::setViewPort(viewPortWidth,viewPortHeight, viewPortLeftOffset,viewPortBottomOffset);
}
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_game->setkey(key,action);
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
    cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << endl;
    cout << "OpenGl version"<< RenderEngine::Renderer::getVersionStr() << endl;
    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    
    {
        ResourceManager::setExecutablePath(argv[0]);
        g_game->init();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            RenderEngine::Renderer::Clear();
            g_game->render();
            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
        g_game = nullptr;
        ResourceManager::unloadAllResources();
    }
    glfwTerminate();
    return 0;
}