#pragma once
#include "cyclone/particle.h"
#include "cyclone/pfgen.h"
#include "cyclone/pcontacts.h"

namespace cyclone
{
    class ParticleWorld
    {
    public:
        struct ParticleRegistration
        {
            Particle* particle;
            ParticleRegistration* next;
        };
        ParticleRegistration* firstParticle;
        using Particles = std::vector<Particle*>;
        using ContactGenerators = std::vector<ParticleContactGenerator*>;
    public:
        ParticleWorld(unsigned maxContacts, unsigned iterations=0);
        void startFrame();
    
    public:
        ParticleForceRegistry registry;

        ParticleContactResolver resolver;
        bool calculateIterations;

        struct ContactGenRegistration
        {
            ParticleContactGenerator* gen;
            ContactGenRegistration* next;
        };
        ContactGenRegistration* firstContactGen;
        ParticleContact* contacts;
        unsigned maxContacts;
    
    public:
        unsigned generateContacts();
        void integrate(real duration);
        void runPhysics(real duration);
    };

    class GroundContacts : public ParticleContactGenerator
    {
    private:
        ParticleWorld::Particles* particles;
    public:
        void init(ParticleWorld::Particles* particles);
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
    };
}
