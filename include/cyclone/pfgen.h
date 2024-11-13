#pragma once
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

    class ParticleBungee : public ParticleForceGenerator
    {
        Particle* other;
        real springConstant;
        real restLength;
    public:
        ParticleBungee(Particle* other, real springConstant, real restLength)
        {};
        virtual void updateForce(Particle* particle, real duration)
        {
            Vector3 force;
            particle->getPosition();
            force -= other->getPosition();

            real magnitude = force.magnitude();
            if(magnitude <= restLength) return;
            magnitude = springConstant * (restLength - magnitude);
            
            force.normalize();
            force *= -magnitude;
            particle->addForce(force);
        }
    };

    class ParticleBuoyancy : public ParticleForceGenerator
    {
        real maxDepth;
        real volume;

        real waterHeight;
        real liquidDensity;

    public:
        ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.f)
        {
        }
        virtual void updateForce(Particle* particle, real duration)
        {
            real depth = particle->getPosition().y;
            if(depth >= waterHeight + maxDepth) return;
            
            Vector3 force(0, 0, 0);
            if(depth <= waterHeight - maxDepth)
            {
                force.y = liquidDensity * volume;
                particle->addForce(force);
                return;
            }
            force.y = liquidDensity * volume * (depth - maxDepth - waterHeight) / 2 * maxDepth;

            particle->addForce(force);
        }
    };

    #define real_sin sinf
    #define real_cos cosf
    #define real_exp expf

    class ParticleFakeSpring : public ParticleForceGenerator
    {
        Vector3* anchor;
        real springConstant;
        real damping;
    public:
        ParticleFakeSpring(Vector3* anchor, real springConstant, real damping)
        {

        }
        virtual void updateForce(Particle* particle, real duration)
        {
            if(!particle->hasFiniteMass()) return;
            
            Vector3 position;
            particle->getPosition(&position);
            position -= *anchor;

            real gamma = 0.5f * real_sqrt(4 * springConstant - damping * damping);
            if(gamma == 0.0f) return;
            Vector3 c = position * (damping / (2.0f * gamma)) + particle->getVelocity() * (1.0f / gamma);

            Vector3 target = position * real_cos(gamma * duration) + c * real_sin(gamma * duration);
            target *= real_exp(-0.5f * duration * damping);

            Vector3 accel = (target - position) * (1.0f / duration * duration) - particle->getVelocity() * duration;
            particle->addForce(accel * particle->getMass());
        }
    };
}



