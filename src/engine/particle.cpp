#include<assert.h>
#include "cyclone/core.h"
#include "cyclone/particle.h"

namespace cyclone
{
    void Particle::integrate(real duration)
    {
        if(inverseMass <= 0.0f) return;
        
        assert(duration > 0.0f);

        position.addScaledVector(velocity, duration);

        Vector3 resultingAcc = acceleration;
        resultingAcc.addScaledVector(forceAccum, inverseMass);
        // v = v * d^t + a * t, d是阻尼
        // 但下面的更新v先变化了，不会对后面的阻尼变更有影响吗
        // 测试一下
        velocity.addScaledVector(resultingAcc, duration);

        velocity *= real_pow(damping, duration);
        clearAccumulator();
    }

    void Particle::setMass(const real mass)
    {
        assert(mass != 0);
        Particle::inverseMass = (static_cast<real>(1.0f)) / mass;
    }
    real Particle::getMass() const
    {
        if(inverseMass == 0)
        {
            return REAL_MAX;
        }
        else
        {
            return (static_cast<real>(1.0f) / inverseMass);
        }
    }
    void Particle::setInverseMass(const real inverseMass)
    {
        Particle::inverseMass = inverseMass;
    }
    real Particle::getInverseMass() const
    {
        return inverseMass;
    }
    bool Particle::hasFiniteMass() const
    {
        return inverseMass >= 0.0f;
    }
    void Particle::setDamping(const real damping)
    {
        Particle::damping = damping;
    }
    real Particle::getDamping() const
    {
        return damping;
    }

    void Particle::setPosition(const Vector3& position)
    {
        Particle::position = position;
    }
    void Particle::setPosition(const real x, const real y, const real z)
    {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    void Particle::getPosition(Vector3 *position) const
    {
        *position = Particle::position;
    }
    Vector3 Particle::getPosition() const
    {
        return position;
    }

    void Particle::setVelocity(const Vector3 &velocity)
    {
        Particle::velocity = velocity;
    }
    void Particle::setVelocity(const real x, const real y, const real z)
    {
        velocity.x = x;
        velocity.y = y;
        velocity.z = z;
    }

    void Particle::getVelocity(Vector3* velocity) const
    {
        *velocity = Particle::velocity;
    }

    Vector3 Particle::getVelocity() const
    {
        return velocity;
    }

    void Particle::setAcceleration(const Vector3 &acceleration)
    {
        Particle::acceleration = acceleration;
    }
    void Particle::setAcceleration(const real x, const real y, const real z)
    {
        acceleration.x = x;
        acceleration.y = y;
        acceleration.z = z;
    }

    void Particle::getAcceleration(Vector3 * acceleration)
    {
        *acceleration = Particle::acceleration;
    }
    Vector3 Particle::getAcceleration() const
    {
        return acceleration;
    }

    void Particle::clearAccumulator()
    {
        forceAccum.clear();
    }
    void Particle::addForce(const Vector3& force)
    {
        forceAccum += force;
    }
}
