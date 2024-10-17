#include "cyclone/pcontacts.h"

using namespace cyclone;

void ParticleContact::resolve(real duration)
{
    resolveVelocity(duration);
}

real ParticleContact::calculateSeparatingVelocity() const
{
    Vector3 relativeVelocity = particle[0]->getVelocity();
    if(particle[1]) relativeVelocity -=  particle[1]->getVelocity();
    return relativeVelocity * contactNormal;
}
