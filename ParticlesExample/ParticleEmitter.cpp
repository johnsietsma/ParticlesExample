#include "ParticleEmitter.h"

#include <SDL.h>
#include <iostream>

void ParticleEmitter::Update(float deltaTime)
{
    mEmitTimer += deltaTime;
    while (mEmitTimer > mEmitTimeStep) {
        mEmitTimer -= mEmitTimeStep;
        Emit();
    }

    for (size_t particleIndex = 0; particleIndex < mDeadIndex; particleIndex++) {
        auto& particle = mParticles[particleIndex];
        particle.position += particle.velocity * deltaTime;
        mParticles[particleIndex] = particle;
    }
}

void ParticleEmitter::Render( SDL_Renderer* pRenderer )
{
    // Render all the particles red
    SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);

    SDL_Rect particleRect{ 0,0,20,20 };
    for (size_t particleIndex = 0; particleIndex < mDeadIndex; particleIndex++)
    {
        const auto& particle = mParticles[particleIndex];
        particleRect.x = static_cast<int>(particle.position.x);
        particleRect.y = static_cast<int>(particle.position.y);
        SDL_RenderFillRect(pRenderer, &particleRect);
    }
}

void ParticleEmitter::Emit()
{
    auto& newParticle = mParticles[mDeadIndex];
    mDeadIndex = (mDeadIndex + 1) % ParticleCount;
    newParticle.position = { 10, 768 / 2 };
    newParticle.velocity = { 100,0 };
}
