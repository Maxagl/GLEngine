#include <math.h>
#include <vector>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "render/camera.h"
#include "render/Mesh.h"
#include "render/LightRenderer.h"
#include "render/ShaderLoader.h"

void renderScene();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
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

std::vector<LightRenderer*> lights(10, nullptr);

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Engine Demo", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
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
    light = new LightRenderer(MeshType::kSphere, camera);
    light->setProgram(flatShaderProgram);
    light->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    for(int i{0}; i < 10; ++i)
    {
        lights[i] = new LightRenderer(MeshType::kSphere, camera);
        lights[i]->setProgram(flatShaderProgram);
        lights[i]->setPosition(glm::vec3(0.0f, 0.0f, -i * 2));
    }
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 204.0f / 255.0f, 1.0f);
    // light->draw(800, 600);
    for(LightRenderer* l : lights)
    {
        float ypos = sin(glfwGetTime()) * 2;
        l->setPosition(glm::vec3(l->getPosition().x, ypos, l->getPosition().z));
        l->draw(800, 600);
    }

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