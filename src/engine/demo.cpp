#include "demos/demo.h"

using namespace cyclone;
MassAggregateDemo::MassAggregateDemo(unsigned int particleCount)
: world(particleCount * 10)
{
    particleArray = new cyclone::Particle[particleCount];
    for(unsigned i{0}; i < particleCount; ++i)
    {
        world.getParticles().push_back(particleArray + i);
    }
    groundContactGenerator.init(&world.getParticles());
    world.getContactGenerators().push_back(&groundContactGenerator);
}