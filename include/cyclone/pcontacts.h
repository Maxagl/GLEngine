#include "cyclone/particle.h"

namespace cyclone
{
    class ParticleContact
    {
    public:
        real restitution;
        Vector3 contactNormal;
        real penetration;
        Particle* particle[2];

    public:
        void resolve(real duration);
        real calculateSeparatingVelocity() const;
        void resolveInterpenetration(real duraiton);
        void resolve(real duration);

    private:
        void resolveVelocity(real duration);
    };

    class ParticleContactResolver
    {
    protected:
        unsigned iterations;
        unsigned iterationsUsed;
    public:
        ParticleContactResolver(unsigned iteration);
        void setIterations(unsigned iterations);
        void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
    };

    class ParticleContactGenerator
    {
    public:
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
    };
}



