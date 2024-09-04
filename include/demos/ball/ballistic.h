#include "gl/glew.h"
#include "render/Camera.h"
#include "glfw/glfw3.h"
#include "cyclone/particle.h"
#include "render/ShaderLoader.h"
#include "render/Mesh.h"
#include "render/LightRenderer.h"


class balldemo
{
private:
    Camera* camera;
    LightRenderer* light;

private:
    enum ShotType
    {
        UNUSED = 0,
        PISTOL,
        ARTILLERY,
        FIREBALL,
        LASER
    };

    struct AmmoRound
    {
        cyclone::Particle particle;
        ShotType type;
        unsigned startTime;

        // 就是这里，不知道怎么写
        void render()
        {

        }
    };

    void initGame()
    {
        glEnable(GL_DEPTH_TEST);
        ShaderLoader shader;
        GLuint flatShaderProgram = shader.CreateProgram("../Shaders/FlatModel.vs", "../Shaders/FlatModel.fs");
        camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
        light = new LightRenderer(MeshType::kSphere, camera);
        light->setProgram(flatShaderProgram);
        light->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    }

};
