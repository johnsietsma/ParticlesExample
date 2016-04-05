#pragma once

#include "ParticleEmitter.h"
#include "Vector2.h"
#include "Vector3.h"

#include <SDL.h>

// A colored particle
struct ParticleColored
{
    Vec2 position;
    Vec2 velocity;
    Vec3 color;
};



template<>
void ParticleEmitter<ParticleColored>::EmitParticle(ParticleColored& particle) const
{
    float red = (rand() / (float)RAND_MAX) * 255;
    float green = (rand() / (float)RAND_MAX) * 255;
    float blue = (rand() / (float)RAND_MAX) * 255;
    particle.color = { red, green, blue };
}


template<>
void ParticleEmitter<ParticleColored>::UpdateParticle(ParticleColored& particle, float deltaTime) const
{
    static const Vec3 colorIncrement{ 50, 50, 50 };
    Vec3 c = particle.color + colorIncrement * deltaTime;
    // Wrap the colors around
    if (c.x > 255) c.x = 0;
    if (c.y > 255) c.y = 0;
    if (c.z > 255) c.z = 0;
    particle.color = c;
}


template<>
void ParticleEmitter<ParticleColored>::RenderParticle(SDL_Renderer* pRenderer, const ParticleColored& particle, const SDL_Rect& rect) const
{
    Uint8 red = static_cast<Uint8>(particle.color.x);
    Uint8 green = static_cast<Uint8>(particle.color.y);
    Uint8 blue = static_cast<Uint8>(particle.color.z);

    SDL_SetRenderDrawColor(pRenderer, red, green, blue, 255);
    SDL_RenderFillRect(pRenderer, &rect);
}

