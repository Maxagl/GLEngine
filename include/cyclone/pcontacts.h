#include "cyclone/particle.h"

namespace cyclone
{
    class ParticleContact
    {
    public:
        Particle* particle[2];
        real restitution;
        Vector3 contactNormal;
    protected:
        void resolve(real duration);
        real calculateSeparatingVelocity() const;
    private:
        void resolveVelocity(real duration);
    };
}


