#include "cyclone/particle.h"

namespace cyclone
{
    class ParticleContact
    {
    public:
        Particle* particle[2];
        real restitution;
        Vector3 contactNormal;
        real penetration;
        
    protected:
        void resolve(real duration);
        real calculateSeparatingVelocity() const;
        void resolveInterpenetration(real duraiton);
        void resolve(real duration);
    private:
        void resolveVelocity(real duration);
    };
}


