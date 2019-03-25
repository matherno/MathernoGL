//
// Created by matt on 27/03/17.
//


#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
  {

  }

void ParticleEmitter::createParticle(const Vector3D& initPosOffset, const Vector3D& velocity, long timeAlive)
  {
  Particle* particle = new Particle();
  particle->position = position + initPosOffset;
  particle->velocity = velocity;
  particle->timeToLive = timeAlive;
  particle->texIndex = numTextureIndices > 0 ? mathernogl::RandomGenerator::randomInt(0, numTextureIndices - 1) : 0;
  particles.emplace_back(particle);
  }

void ParticleEmitter::updateParticles(long deltaTime)
  {
  currentTime += deltaTime;
  while (currentTime - lastSpawnTime > timeBetweenSpawns)
    {
    if (isSpawning)
      {
      Vector3D offset(0);
      if (particleOffsetFunc)
        {
        offset = particleOffsetFunc();
        }
      Vector3D direction;
      if (particleDirectionFunc)
        direction = particleDirectionFunc();
      else
        direction = randomGenerator.getNormal();
      createParticle(offset, direction * initVelocity, timeAlive);
      }
    lastSpawnTime += timeBetweenSpawns;
    }

  auto iter = particles.begin();
  while (iter != particles.end())
    {
    Particle* particle = iter->get();
    particle->timeToLive -= deltaTime;
    particle->velocity.y -= gravityAccel * deltaTime;
    particle->position += particle->velocity * (int) deltaTime;
    if (particle->timeToLive <= 0)
      iter = particles.erase(iter);
    else
      iter++;
    }
  }

double ParticleEmitter::getGravityAccel() const
  {
  return gravityAccel;
  }

void ParticleEmitter::setGravityAccel(double gravityAccel)
  {
  ParticleEmitter::gravityAccel = gravityAccel;
  }

long ParticleEmitter::getTimeBetweenSpawns() const
  {
  return timeBetweenSpawns;
  }

void ParticleEmitter::setTimeBetweenSpawns(long timeBetweenSpawns)
  {
  ParticleEmitter::timeBetweenSpawns = timeBetweenSpawns;
  }

long ParticleEmitter::getTimeAlive() const
  {
  return timeAlive;
  }

void ParticleEmitter::setTimeAlive(long timeAlive)
  {
  ParticleEmitter::timeAlive = timeAlive;
  }

void ParticleEmitter::sortParticles(const Matrix4* worldToCamera)
  {
  //todo could try different algorithm if (when...) need performance -> insertion sort
  particles.sort([worldToCamera](const ParticlePtr& lhs, const ParticlePtr& rhs)->bool
                   {
                   Vector3D lhCamPos = lhs->position * *worldToCamera;
                   Vector3D rhCamPos = rhs->position * *worldToCamera;
                   return lhCamPos.z < rhCamPos.z;
                   });
  }
