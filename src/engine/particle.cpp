#include<assert.h>
#include "cyclone/particle.h"

using namespace cyclone;


void Particle::integrate(real duration)
{
    if(inverseMass <= 0.0f) return;
    
    assert(duration > 0.0f);

    position.addScaleVector(velocity, duration);

    Vector3 resultingAcc = acceleration;
    resultingAcc.addScaleVector(forceAccum, inverseMass);
    // v = v * d^t + a * t, d是阻尼
    // 但下面的更新v先变化了，不会对后面的阻尼变更有影响吗
    // 测试一下
    velocity.addScaleVector(resultingAcc, duration);

    velocity *= real_pow(damping, duration);
    clearAccumulator();
}