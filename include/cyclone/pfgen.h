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

    class PartileDrag : public ParticleForceGenerator
    {
        private:
            real k1;
            real k2;
        public:
            PartileDrag(real k1, real k2);
            virtual void updateForce(Particle* particle, real duration)
            {
                Vector3 force;
                particle->getVelocity(&force);

                real dragCoeff = force.magnitude();
                dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

                force.normalize();
                force *= -dragCoeff;
                particle->addForce(force);
            }
    };
    class ParticleSpring : public ParticleForceGenerator
    {
        Particle* other;
        real springConstant;
        real restLength;
        public:
            ParticleSpring(Particle* other, real springConstant, real restLength)
            {

            }
            virtual void updateForce(Particle* particle, real duration)
            {
                Vector3 force;
                particle->getPosition(&force);
                force -= other->getPosition();
                
                real magnitude = force.magnitude();
                magnitude = real_abs(magnitude - restLength);
                magnitude *= springConstant;

                force.normalize();
                force *= -magnitude;
                particle->addForce(force);
            }
    };

    class ParticleAnchoredSpring : public ParticleForceGenerator
    {
        Vector3* anchor;
        real springConstant;
        real restLength;
        public:
            ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength)
            {

            }
            virtual void updateForce(Particle* particle, real duration)
            {
                Vector3 force;
                particle->getPosition(&force);
                force -= *anchor;

                real magnitude = force.magnitude();
                magnitude = real_abs(magnitude - restLength);
                magnitude *= springConstant;

                force.normalize();
                force *= -magnitude;
                particle->addForce(force);
            }
    };
}
