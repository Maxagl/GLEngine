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


class demo
{
public:
    Camera* camera;
    GLuint shaderProgram;
private:
    virtual const std::string getTitle() = 0;
    virtual void update(float duration) = 0;
    virtual void display() = 0;
};
