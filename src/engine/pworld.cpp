#include "cyclone/pworld.h"

namespace cyclone
{

    ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations=0)
    : resolver(iterations), maxContacts(maxContacts)
    {
        contacts = new ParticleContact[maxContacts];
        calculateIterations = (iterations == 0);
    }
    void ParticleWorld::startFrame()
    {
        ParticleRegistration* reg = firstParticle;
        while(reg)
        {
            reg->particle->clearAccumulator();
            reg = reg->next;
        }
    }

    unsigned ParticleWorld::generateContacts()
    {
        unsigned limit = maxContacts;
        ParticleContact* nextContact = contacts;

        ContactGenRegistration* reg = firstContactGen;
        while(reg)
        {
            unsigned used = reg->gen->addContact(nextContact, limit);
            limit -= used;
            nextContact += used;
            if(limit <= 0) break;
            reg = reg->next;
        }
        return maxContacts - limit;
    }

    void ParticleWorld::integrate(real duration)
    {
        ParticleRegistration* reg = firstParticle;
        while (reg)
        {
            reg->particle->integrate(duration);
            reg = reg->next;
        }
    }

    void ParticleWorld::runPhysics(real duration)
    {
        registry.updateForces(duration);
        integrate(duration);

        unsigned usedContacts = generateContacts();
        if(calculateIterations) resolver.setIterations(usedContacts * 2);
        resolver.resolveContacts(contacts, usedContacts, duration);
    }
    
    ParticleWorld::Particles& ParticleWorld::getParticles()
    {
        return particles;
    }

    ParticleWorld::ContactGenerators& ParticleWorld::getContactGenerators()
    {
        return contactGenerators;
    }

    ParticleForceRegistry& ParticleWorld::getForceRegistry()
    {
        return registry;
    }

    void GroundContacts::init(ParticleWorld::Particles* particles)
    {
        GroundContacts::particles = particles;
    }

    unsigned GroundContacts::addContact(cyclone::ParticleContact* contact, unsigned limit) const
    {
        unsigned count = 0;
        for(ParticleWorld::Particles::iterator p = particles->begin(); p != particles->end(); ++p)
        {
            cyclone::real y = (*p)->getPosition().y;
            if(y < 0.0f)
            {
                contact->contactNormal = Vector3::UP;
                contact->particle[0] = *p;
                contact->particle[1] = nullptr;
                contact->penetration = -y;
                contact->restitution = 0.2;
                contact++;
                count++;
            }
            if(count >= limit) return count;
        }
        return count;
    }
}



