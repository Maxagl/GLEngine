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


class balldemo
{
private:
    Camera* camera;
    
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
        LightRenderer* light;
        AmmoRound(Camera* camera)
        {
            ShaderLoader shader;
            GLuint flatShaderProgram = shader.CreateProgram("../Shaders/FlatModel.vs", "../Shaders/FlatModel.fs");
            light = new LightRenderer(MeshType::kSphere, camera);
            light->setProgram(flatShaderProgram);
            light->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        }
        // 就是这里，不知道怎么写
        void render()
        {
            cyclone::Vector3 position;
            particle.getPosition(&position);
            light->setPosition(glm::vec3({position.x, position.y, position.z}));
            light->draw(800, 600);
        }
    };

    int ammoRounds = 16;
    // 下面的这种写法是不被允许的在类里面，会被看成是函数
    //std::vector<AmmoRound*> ammo(199, nullptr);
    std::vector<AmmoRound*> ammo{};
    ShotType currentShotType;

    void fire()
    {
        for(int i{0}; i < ammoRounds; ++i)
        {
            if(ammo[i]->type == UNUSED) break;
        }
    }

public:
    balldemo() : currentShotType(LASER)
    {
        ammo.resize(ammoRounds);
        camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
        for(int i{0}; i < ammoRounds; ++i)
        {
            ammo[i]->type = UNUSED;
        }
    }

    virtual const std::string getTitle()
    {
        return "Cyclone > Ballistic Demo";
    }
    virtual void update()
    {

    }
    virtual void display()
    {

    }
    virtual void mouse(int butto, int state, int x, int y)
    {

    }
    virtual void key(unsigned char key)
    {
    }


};
