#include "cyclone/particle.h"
#include "cyclone/pcontacts.h"

namespace cyclone
{
    class ParticleLink
    {
    public:
        Particle* particle[2];
    protected:
        real currentLength() const;
    public:
        virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const = 0;
    };
    
    class ParticleCable : public ParticleLink
    {
    public:
        real maxLength;
        real restitution;
    
    public:
        virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const;
    };

    real ParticleLink::currentLength() const
    {
        Vector3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
        return relativePos.magnitude();
    }

    unsigned ParticleCable::fillContact(ParticleContact* contact, unsigned limit) const
    {
        real length = currentLength();
        if(length < maxLength)
        {
            return 0;
        }
        contact->particle[0] = particle[0];
        contact->particle[1] = particle[1];

        Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
        normal.normalize();
        contact->contactNormal = normal;
        contact->penetration = length - maxLength;
        contact->restitution = restitution;

        return 1;
    }

    class ParticleRod : public ParticleLink
    {
    public:
        real length;
    public:
        real currentLength() const;

        virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const;
    };

    unsigned ParticleRod::fillContact(ParticleContact* contact, unsigned limit) const
    {
        real currentLen = currentLength();

        if(currentLen == length) return 0;
        
        contact->particle[0] = particle[0];
        contact->particle[1] = particle[1];

        Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
        normal.normalize();

        if(currentLen > length)
        {
            contact->contactNormal = normal;
            contact->penetration = currentLen - length;
        }
        else
        {
            contact->contactNormal = normal * -1;
            contact->penetration = length - currentLen;
        }
        contact->restitution = 0;
        return 1;
    }

    class ParticleConstraint : public ParticleContactGenerator
    {
    public:
        Particle* particle;
        Vector3 anchor;
    
    protected:
        real currentLength() const;
    
    public:
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
    };

    class ParticleCableConstraint : public ParticleContactGenerator
    {
    public:
        real maxLength;
        real restitution;
    public:
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
    };
    class ParticleRodConstraint : public ParticleConstraint
    {
    public:
        real length;
    
    public:
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
    };
}