#include <math.h>
#include <vector>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "render/camera.h"
#include "render/Mesh.h"
#include "render/LightRenderer.h"
#include "render/ShaderLoader.h"
#include "demos/ball/ballistic.h"

void renderScene();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void processMouseButton(GLFWwindow* window, int button, int action, int mods);
void initGame();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera* camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

LightRenderer* light;
balldemo* demo;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Engine Demo", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, processMouseButton);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewInit();
    initGame();
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame- lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    glfwTerminate();

    delete camera;
    delete light;
}

void initGame()
{
    glEnable(GL_DEPTH_TEST);
    ShaderLoader shader;
    GLuint flatShaderProgram = shader.CreateProgram("../Shaders/FlatModel.vs", "../Shaders/FlatModel.fs");
    camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
    light = new LightRenderer(MeshType::kQuad, camera);
    light->setProgram(flatShaderProgram);
    light->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    demo = new balldemo();

}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 204.0f / 255.0f, 1.0f);
    light->draw(800, 600);
    demo->update(deltaTime);
    demo->display();
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {   
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {   
        camera->ProcessKeyboard(LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {   
        camera->ProcessKeyboard(RIGHT, deltaTime);
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;
    
    camera->ProcessMouseMovement(xoffset, yoffset);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void processMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        demo->fire();
    }
}