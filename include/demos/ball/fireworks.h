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

class Firework : public cyclone::Particle, public demo
{
public:
    unsigned type;
    cyclone::real age;

public:
    void initFireworkRules()
    {
    }
};

class FireworkRule
{
public:
    struct Payload
    {
        unsigned type;
        unsigned count;
        void set(unsigned type, unsigned count)
        {
            Payload::type = type;
            Payload::count == count;
        }
    };
public:
    unsigned type;
    cyclone::real minAge;
    cyclone::real maxAge;
    cyclone::Vector3 minVelocity;
    cyclone::Vector3 maxVelocity;
    cyclone::real damping;
    unsigned payloadCount;
    Payload* payloads;

public:
    FireworkRule() : payloadCount(0), payloads(nullptr)
    {

    }
    ~FireworkRule()
    {
        if(payloads != nullptr) delete[] payloads;
    }
    void init(unsigned payloadCount)
    {
        this->payloadCount = payloadCount;
        payloads = new Payload[payloadCount];
    }

    void setParameters(unsigned type, cyclone::real minAge, cyclone::real maxAge,
                       const cyclone::Vector3& minVelocity, const cyclone::Vector3& maxVelocity,
                       cyclone::real damping)
    {
        this->type = type;
        this->minAge = minAge;
        this->maxAge = maxAge;
        this->minVelocity = minVelocity;
        this->maxVelocity = maxVelocity;
        this->damping = damping;
    }

    void create(Firework* firework, const Firework* parent = nullptr) const
    {
        firework->type = type;
    }
};