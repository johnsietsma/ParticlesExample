#pragma once

#include "Vector2.h"

#include <array>
#include <assert.h>
#include <SDL.h>

struct SDL_Renderer;

template<typename TParticle>
class ParticleEmitter
{
    static constexpr const int ParticleCount = 10000;
    static constexpr const float EmitRate = 5.f;
    static constexpr const float mEmitTimeStep = 1 / EmitRate;

public:
    ParticleEmitter(Vec2 startPos) :
        mStartPos(startPos)
    {}

    void Update(float deltaTime);
    void Render(SDL_Renderer* pRenderer) const;

private:
    void Emit();

    void EmitParticle(TParticle& particle) const {};
    void UpdateParticle(TParticle& particle, float deltaTime) const {};
    inline void RenderParticle(SDL_Renderer* pRenderer, const TParticle& particle, const SDL_Rect& rect) const;

    float mEmitTimer = 0;

    size_t mDeadIndex = 0;
    std::array<TParticle, ParticleCount> mParticles;

    Vec2 mStartPos;
};



template<typename TParticle>
void ParticleEmitter<TParticle>::Update(float deltaTime)
{
    mEmitTimer += deltaTime;
    while (mEmitTimer > mEmitTimeStep) {
        mEmitTimer -= mEmitTimeStep;
        Emit();
    }

    for (size_t particleIndex = 0; particleIndex < mDeadIndex; particleIndex++) {
        auto& particle = mParticles[particleIndex];
        particle.position += particle.velocity * deltaTime;
        UpdateParticle(particle, deltaTime);
        mParticles[particleIndex] = particle;
    }
}


template<typename TParticle>
void ParticleEmitter<TParticle>::Render(SDL_Renderer* pRenderer) const
{
    // Render all the particles red by default
    SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);

    SDL_Rect particleRect{ 0,0,20,20 };

    for (size_t particleIndex = 0; particleIndex < mDeadIndex; particleIndex++)
    {
        const auto& particle = mParticles[particleIndex];
        particleRect.x = static_cast<int>(particle.position.x);
        particleRect.y = static_cast<int>(particle.position.y);
        RenderParticle(pRenderer, particle, particleRect);
    }
}

template<typename TParticle>
void ParticleEmitter<TParticle>::Emit()
{
    auto& particle = mParticles[mDeadIndex];
    mDeadIndex = (mDeadIndex + 1) % ParticleCount;
    particle.position = mStartPos;
    particle.velocity = { 100,0 };
    EmitParticle(particle);
}

template<typename TParticle>
void ParticleEmitter<TParticle>::RenderParticle(SDL_Renderer* pRenderer, const TParticle& particle, const SDL_Rect& rect) const
{
    SDL_RenderFillRect(pRenderer, &rect);
}