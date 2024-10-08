#pragma once

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "render/Mesh.h"
#include "render/ShaderLoader.h"
#include "render/Camera.h"

class LightRenderer
{
private:
    Camera* camera;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    glm::vec3 position;
    glm::vec3 color;
    glm::mat4 trans;
    GLuint vbo, ebo, vao, texture, program;
    MeshType mtype;


public:
    LightRenderer(MeshType MeshType, Camera* camera, glm::mat4 tansform = glm::mat4(1.0));
    ~LightRenderer();

    void draw(const int width, const int height);

    void setPosition(glm::vec3 _position);
    void setColor(glm::vec3 _color);
    void setProgram(GLuint program);
    void setTransform(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), 
                                 float rotateRadian = 0.0f, glm::vec3 rotateAxix = glm::vec3(1.0f, 0.0f, 0.0f));

    glm::vec3 getPosition();
    glm::vec3 getColor();

};