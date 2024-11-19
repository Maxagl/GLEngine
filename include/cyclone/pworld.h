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

        Particles particles;
        ContactGenerators contactGenerators;

        unsigned maxContacts;
    
    public:
        ParticleWorld(unsigned maxContacts, unsigned iterations=0);
        unsigned generateContacts();
        void integrate(real duration);
        void runPhysics(real duration);
        void startFrame();
        Particles& getParticles();
        ContactGenerators& getContactGenerators();
        ParticleForceRegistry& getForceRegistry();

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
