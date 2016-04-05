#pragma once

#include "Vector2.h"

#include <array>
#include <SDL.h>

struct SDL_Renderer;

/*
 * A ParticleEmitter emits particles of a single type.
 * A particle type must have position and velocity members. See Particle.h for a minimal example.
 * New particle types can specialize EmitParticle, UpdateParticle and RenderParticle to achieve different behaviours and effects. 
 * See ParticleColored.h for an example of these template specializations.
 */
template<typename TParticle>
class ParticleEmitter
{
    static constexpr const int ParticleCount = 10000;
    static constexpr const float EmitRate = 5.f;
    static constexpr const float EmitTimeStep = 1 / EmitRate;

public:
    ParticleEmitter(Vec2 startPos) :
        mStartPos(startPos)
    {}

    void Update(float deltaTime);
    void Render(SDL_Renderer* pRenderer) const;

private:
    void Emit(); // Init a particle

    // Functions available for specialization
    void EmitParticle(TParticle& particle) const {}; // Does nothing by default
    void UpdateParticle(TParticle& particle, float deltaTime) const {}; // Does nothing by default
    inline void RenderParticle(SDL_Renderer* pRenderer, const TParticle& particle, const SDL_Rect& rect) const;

    float mEmitTimer = 0; // Tracks the timer between particle emits
    size_t mDeadIndex = 0; // The index of the first dead particle. Lower indices hold live particles
    std::array<TParticle, ParticleCount> mParticles; // The particle buffer
    Vec2 mStartPos; // Were particles will be emitted from
};



/*
 * Update the particle emiiter.
 * Emits new particles and updates the properties of existing particles.
 */
template<typename TParticle>
void ParticleEmitter<TParticle>::Update(float deltaTime)
{
    mEmitTimer += deltaTime;
    while (mEmitTimer > EmitTimeStep) {
        mEmitTimer -= EmitTimeStep;
        Emit();
    }

    for (size_t particleIndex = 0; particleIndex < mDeadIndex; particleIndex++) {
        auto& particle = mParticles[particleIndex];
        particle.position += particle.velocity * deltaTime; // Move the particle
        UpdateParticle(particle, deltaTime); // Update any further particle data
    }
}


/*
 * Render all the particles.
 */
template<typename TParticle>
void ParticleEmitter<TParticle>::Render(SDL_Renderer* pRenderer) const
{
    // Render all the particles red by default
    SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);

    // Set the default rectangle size.
    SDL_Rect particleRect{ 0,0,20,20 };

    for (size_t particleIndex = 0; particleIndex < mDeadIndex; particleIndex++)
    {
        const auto& particle = mParticles[particleIndex];
        particleRect.x = static_cast<int>(particle.position.x);
        particleRect.y = static_cast<int>(particle.position.y);
        RenderParticle(pRenderer, particle, particleRect);
    }
}

/*
 * Emit a single particle.
 */
template<typename TParticle>
void ParticleEmitter<TParticle>::Emit()
{
    float yVelocity = ((rand() / (float)RAND_MAX) * 10) - 5;

    auto& particle = mParticles[mDeadIndex];
    mDeadIndex = (mDeadIndex + 1) % ParticleCount; // Wrap around the particle buffer

    // Set the initial particle data
    particle.position = mStartPos;
    particle.velocity = { 100, yVelocity };

    EmitParticle(particle); // Fill in any futher particle data.
}

/*
 * Render a single particle.
 */
template<typename TParticle>
void ParticleEmitter<TParticle>::RenderParticle(SDL_Renderer* pRenderer, const TParticle& particle, const SDL_Rect& rect) const
{
    // The default render, simply draw a rectangle.
    SDL_RenderFillRect(pRenderer, &rect);
}
