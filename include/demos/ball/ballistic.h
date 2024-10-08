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
#include "demos/demo.h"


class balldemo : public demo
{
private:
    std::vector<LightRenderer*> lines;
    LightRenderer* ball;
    
public:
    enum ShotType
    {
        UNUSED = 0,
        PISTOL,
        ARTILLERY,
        FIREBALL,
        LASER
    };
    ShotType currentShotType;
private:
    struct AmmoRound
    {
        cyclone::Particle particle;
        ShotType type;
        unsigned startTime;
        LightRenderer* light;
        AmmoRound(Camera* camera, GLuint shaderProgram)
        {
            ShaderLoader shader;
            light = new LightRenderer(MeshType::kSphere, camera);
            light->setProgram(shaderProgram);
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

    balldemo(GLuint shaderProgram, Camera* outcamera) : currentShotType(LASER), lines(20)
    {
        this->shaderProgram = shaderProgram;
        this->camera = outcamera;
        ammo.resize(ammoRounds);

        ball = new LightRenderer(MeshType::kSphere, camera);
        ball->setProgram(shaderProgram);
        ball->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
        for(int i{0}; i < ammoRounds; ++i)
        {
            ammo[i] = new AmmoRound(camera, shaderProgram);
            ammo[i]->type = UNUSED;
        }
        // 发射点的球
        // 画几条线
        for(int i{0}; i < lines.size(); ++i)
        {
            lines[i] = new LightRenderer(MeshType::kQuad, camera);
            lines[i]->setProgram(shaderProgram);
            lines[i]->setPosition(glm::vec3(0, 0, i * 20));
            lines[i]->setTransform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(10.0f, 0.1f, 1.0f));
        }
    }

    const std::string getTitle() override
    {
        return "Cyclone > Ballistic Demo";
    }
    void update(float duration) override
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
    void display() override
    {
        ball->draw(800, 600);
        for(int i{0}; i < lines.size(); ++i)
        {
            lines[i]->draw(800, 600);
        }
        for(int i{0}; i < ammoRounds; ++i)
        {
            if(ammo[i]->type != UNUSED)
            {
                ammo[i]->render();
            }
        }
    }
    void processInput(GLFWwindow* window) override
    {
        // set fire type
        if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        {   
            this->currentShotType = ShotType::PISTOL;
        }
        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {   
            this->currentShotType = ShotType::ARTILLERY;
        }
        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {   
            this->currentShotType = ShotType::FIREBALL;
        }
        if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {   
            this->currentShotType = ShotType::LASER;
        }
    }

    void processMouseButton(GLFWwindow* window, int button, int action, int mods) override
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            this->fire();
        }
    }
};
