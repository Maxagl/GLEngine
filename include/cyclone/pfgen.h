#include "cyclone/particle.h"
#include <vector>

namespace cyclone
{
    class ParticleForceGenerator
    {
    public:
        virtual void updateForce(Particle* particle, real duration) = 0;
    };

    class ParticleForceRegistry
    {
    protected:
        struct  ParticleForceRegistration
        {
            Particle* Particle;
            ParticleForceGenerator* fg;
        };

        using Registry = std::vector<ParticleForceRegistration>;
        Registry registration;
    
    public:
        void add(Particle* particle, ParticleForceGenerator* fg);
        void remove(Particle* particle, ParticleForceGenerator* fg);
        void clear();
        void updateForces(real duration)
        {
            Registry::iterator i = registration.begin();
            for(; i != registration.end(); ++i)
            {
                i->fg->updateForce(i->Particle, duration);
            }
        }
    };

    class ParticleGravatiy : public ParticleForceGenerator
    {
        Vector3 gravity{};
        public:
            ParticleGravatiy(const Vector3& gravity);
            virtual void updateForce(Particle* particle, real duration)
            {
                if(!particle->hasFiniteMass()) return;
                particle->addForce(gravity* particle->getMass());
            }
    };
}
