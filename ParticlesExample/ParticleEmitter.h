#pragma once

#include "Particle.h"

#include <array>

struct SDL_Renderer;

class ParticleEmitter
{
    static constexpr const int ParticleCount = 10000;
    static constexpr const float EmitRate = 5.f;
    static constexpr const float mEmitTimeStep = 1 / EmitRate;

public:
    void Update(float deltaTime);
    void Render(SDL_Renderer* pRenderer);

private:
    void Emit();

    float mEmitTimer = 0;

    size_t mDeadIndex = 0;
    std::array<Particle, ParticleCount> mParticles;
};
