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
    std::vector<LightRenderer*> lines;
    LightRenderer* ball;
    
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

public:
    void fire()
    {
        int i{0};
        for(i; i < ammoRounds; ++i)
        {
            if(ammo[i]->type == UNUSED) break;
        }
        if(i >= 16) return;

        switch(currentShotType)
        {
            case PISTOL:
                ammo[i]->particle.setMass(2.0f);
                ammo[i]->particle.setVelocity(0.0f, 0.0f, 35.f);
                ammo[i]->particle.setAcceleration(0.0f, -1.0f, 0.0f);
                ammo[i]->particle.setDamping(0.99f);
                break;

            case ARTILLERY:
                ammo[i]->particle.setMass(200.0f);
                ammo[i]->particle.setVelocity(0.0f, 30.0f, 35.f);
                ammo[i]->particle.setAcceleration(0.0f, -20.0f, 0.0f);
                ammo[i]->particle.setDamping(0.99f);
                break;

            case FIREBALL:
                ammo[i]->particle.setMass(1.0f);
                ammo[i]->particle.setVelocity(0.0f, 0.0f, 10.f);
                ammo[i]->particle.setAcceleration(0.0f, 0.6f, 0.0f);
                ammo[i]->particle.setDamping(0.99f);
                break;

            case LASER:
                ammo[i]->particle.setMass(0.1f);
                ammo[i]->particle.setVelocity(0.0f, 0.0f, 100.f);
                ammo[i]->particle.setAcceleration(0.0f, 0.6f, 0.0f);
                ammo[i]->particle.setDamping(0.99f);
                break;                  
        }
        ammo[i]->particle.setPosition(0.0f, 1.5f, 0.0f);
        ammo[i]->startTime = glfwGetTime();
        ammo[i]->type = currentShotType;

        ammo[i]->particle.clearAccumulator();
    }

    balldemo() : currentShotType(LASER), lines(20)
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
    virtual void update(float duration)
    {
        if(duration <= 0.0f) return;
        for(AmmoRound* shot : ammo)
        {
            if(shot->type != UNUSED)
            {
                shot->particle.integrate(duration);
                if(shot->particle.getPosition().y < 0.0f || 
                   shot->startTime + 5 < duration || 
                   shot->particle.getPosition().z > 400.f)
                {
                    shot->type = UNUSED;
                }
            }
        }
    }
    virtual void display()
    {
        ShaderLoader shader;
        GLuint flatShaderProgram = shader.CreateProgram("../Shaders/FlatModel.vs", "../Shaders/FlatModel.fs");
        // 发射点的球
        ball = new LightRenderer(MeshType::kSphere, camera);
        ball->setProgram(flatShaderProgram);
        ball->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        // 画几条线
        for(int i{0}; i < lines.size(); ++i)
        {
            lines[i] = new LightRenderer(MeshType::kQuad, camera);
            lines[i]->setProgram(flatShaderProgram);
            lines[i]->setPosition(glm::vec3(i * 30, 0, 0));
        }
    }
};
