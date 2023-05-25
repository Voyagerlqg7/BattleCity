#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
using namespace std;


int g_windowSizeX = 640;
int g_windowSizeY = 480;
void glfwWindowSizeCallback(GLFWwindow * pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0,0, g_windowSizeX, g_windowSizeY);
}
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}
int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        cout << "!glfwInit fail" << endl;
        return -1;
    }
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!pWindow)
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

    glClearColor(1, 1, 0, 1);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}