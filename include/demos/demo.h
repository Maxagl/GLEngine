#pragma once

#include <vector>
#include <string>

#include "gl/glew.h"
#include "render/Camera.h"
#include "glfw/glfw3.h"
#include "cyclone/particle.h"
#include "render/ShaderLoader.h"
#include "render/Mesh.h"
#include "render/LightRenderer.h"
#include "cyclone/cyclone.h"


class demo
{
public:
    Camera* camera;
    GLuint shaderProgram;


public:
    virtual const std::string getTitle() = 0;
    virtual void update(float duration) = 0;
    virtual void display() = 0;
    virtual void processInput(GLFWwindow* window) = 0;
    virtual void processMouseButton(GLFWwindow* window, int button, int action, int mods) = 0;
};

class MassAggregateDemo : public demo
{
protected:
    cyclone::ParticleWorld world;
    cyclone::Particle* particleArray;
    cyclone::GroundContacts ground;

};
