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

void ParticleContact::resolveVelocity(real duration)
{
    real separatingVelocity = calculateSeparatingVelocity();
    if(separatingVelocity > 0)
    {
        return;
    }

    real newSepVelocity = -separatingVelocity * restitution;

    Vector3 accCauseVelocity = particle[0]->getAcceleration();
    real accCauseSepVelocity = accCauseVelocity * contactNormal * duration;

    if(accCauseSepVelocity < 0)
    {
        newSepVelocity += restitution * accCauseSepVelocity;
        if(newSepVelocity < 0) newSepVelocity = 0;
    }  


    real deltaVelocity = newSepVelocity - separatingVelocity;

    real totalInverseMass = particle[0]->getInverseMass();
    if(particle[1]) totalInverseMass += particle[1]->getInverseMass();
    if(totalInverseMass <= 0) return;

    real impulse = deltaVelocity / totalInverseMass;

    Vector3 impulsePerIMass = contactNormal * impulse;

    particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerIMass * particle[0]->getInverseMass());

    if(particle[1])
    {
        particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerIMass * -particle[1]->getInverseMass());
    }
}

void ParticleContact::resolve(real duraiton)
{
    resolveVelocity(duraiton);
    resolveInterpenetration(duraiton);
}

void ParticleContact::resolveInterpenetration(real duraiton)
{
    if(penetration <= 0) return;

    // 计算总质量
    real totalInverseMass = particle[0]->getInverseMass();
    if(particle[1]) totalInverseMass += particle[1]->getInverseMass();

    if(totalInverseMass <= 0) return;
    // 单位质量需要移动的距离
    Vector3 movePerIMass = contactNormal * (-penetration / totalInverseMass);
    // 按照质量分配移动距离
    particle[0]->setPosition(particle[0]->getPosition() + movePerIMass * particle[0]->getInverseMass());
    if(particle[1])
    {
        particle[1]->setPosition(particle[1]->getPosition() + movePerIMass * particle[1]->getInverseMass());
    }
}
