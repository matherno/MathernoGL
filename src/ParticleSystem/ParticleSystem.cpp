//
// Created by matt on 24/12/17.
//

#include "ParticleSystem.h"
#include "RenderableParticles.h"

ParticleSystem::ParticleSystem(uint id, bool renderAsPoints) : GameActor(id), renderAsPoints(renderAsPoints)
  {
  setParticleSpawnPoint();
  setParticleDirectionRandom();
  }

void ParticleSystem::onAttached(GameContext* gameContext)
  {
  }

void ParticleSystem::onUpdate(GameContext* gameContext)
  {
  initNewRenderables(gameContext->getRenderContext());
  std::list<uint> emittersToRemove;
  for (ParticleSystemItem& item : *emitters.getList())
    {
    ParticleEmitterPtr& emitter = item.baseEmitter;
    emitter->updateParticles(gameContext->getDeltaTime());
    if (depthTesting && !renderAsPoints)
      emitter->sortParticles(gameContext->getCamera()->getWorldToCamera()->getTransformMatrix());

    if (item.endEmitterCallback)
      {
      if (item.endEmitterCallback())
        {
        emitter->setSpawningState(false);
        item.endEmitterCallback = nullptr;
        item.timeToLive = timeAlive;
        }
      }
    else
      {
      item.timeToLive -= gameContext->getDeltaTime();
      if (item.timeToLive <= 0)
        {
        emittersToRemove.emplace_back(item.id);
        gameContext->getRenderContext()->getRenderableSet()->removeRenderable(item.renderable->getID());
        item.renderable->cleanUp(gameContext->getRenderContext());
        item.renderable.reset();
        }
      else if (item.timeToLive <= timeAlive)
        {
        emitter->setSpawningState(false);
        }
      }
    }

  for (uint id : emittersToRemove)
    emitters.remove(id);
  }

void ParticleSystem::onDetached(GameContext* gameContext)
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (item.renderable)
      {
      gameContext->getRenderContext()->getRenderableSet()->removeRenderable(item.renderable->getID());
      item.renderable->cleanUp(gameContext->getRenderContext());
      }
    }
  emitters.clear();
  }

void ParticleSystem::setGravityAccel(double gravityAccel)
  {
  this->gravityAccel = gravityAccel;
  }

void ParticleSystem::setTimeBetweenSpawns(long timeBetweenSpawns, bool updateExisting /*= false*/)
  {
  this->timeBetweenSpawns = timeBetweenSpawns > 0 ? timeBetweenSpawns : 1;
  if (updateExisting)
    {
    for (ParticleSystemItem& item : *emitters.getList())
      {
      if (item.baseEmitter)
        item.baseEmitter->setTimeBetweenSpawns(timeBetweenSpawns);
      }
    }
  }

void ParticleSystem::setTimeAlive(long timeAlive)
  {
  this->timeAlive = timeAlive;
  }

void ParticleSystem::setParticleSize(double size)
  {
  if (size > 0)
    this->size = size;
  else
    this->size = 1;
  }

void ParticleSystem::setInitVelocity(double velocity)
  {
  this->initVelocity = velocity;
  }

void ParticleSystem::addEmitter(const Vector3D& position, long timeToLive, const Vector3D& colour, double particleSize /*= -1*/)
  {
  addEmitter(position, timeToLive, nullptr, colour, particleSize);
  }

void ParticleSystem::addEmitter(const Vector3D& position, EndEmitterCallback callback, const Vector3D& colour, double particleSize)
  {
  addEmitter(position, 0, callback, colour, particleSize);
  }

void ParticleSystem::addEmitter(const Vector3D& position, long timeToLive, EndEmitterCallback callback, const Vector3D& colour, double particleSize)
  {
  ParticleEmitterPtr emitter(new ParticleEmitter());
  emitter->setGravityAccel(gravityAccel);
  emitter->setInitVelocity(initVelocity);
  emitter->setTimeBetweenSpawns(timeBetweenSpawns);
  emitter->setTimeAlive(timeAlive);
  emitter->setParticleOffsetFunc(particleSpawnFunc);
  emitter->setParticleDirectionFunc(particleDirectionFunc);
  emitter->setPosition(translation + position);
  emitter->setNumTexturesIndices((int)textures.size());

  ParticleSystemItem item;
  item.id = nextEmitterID++;
  item.endEmitterCallback = callback;
  item.timeToLive = timeToLive + timeAlive;   // allows for extra time for particles to die off
  item.baseEmitter = emitter;
  item.colour = colour;
  item.size = particleSize;
  emitters.add(item, item.id);
  }

void ParticleSystem::clearEmitters()
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (item.timeToLive > timeAlive)
      item.timeToLive = timeAlive;
    }
  }

void ParticleSystem::initNewRenderables(RenderContext* renderContext)
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (!item.renderable)
      {
      RenderableParticles* particles = new RenderableParticles(renderContext->getNextRenderableID(), renderAsPoints);
      particles->setEmitter(item.baseEmitter);
      particles->setColour(item.colour);
      particles->setParticleSize(item.size > 0 ? item.size : size);
      for (TexturePtr texture : textures)
        particles->addTextureAtlas(texture);
      particles->setTextureColourMixFactor(colourMixFactor);
      particles->setTextureAtlasSize(numTexAtlasColumns, numTexAtlasRows);
      particles->setAdditiveBlending(additiveBlending);
      particles->setDepthTesting(depthTesting);
      particles->initialise(renderContext);
      item.renderable.reset(particles);
      renderContext->getRenderableSet()->addRenderable(item.renderable);
      }
    }
  }

void ParticleSystem::addTextureAtlas(TexturePtr textureAtlas)
  {
  textures.push_back(textureAtlas);
  }

void ParticleSystem::setParticleSpawnPoint()
  {
  particleSpawnFunc = []()->Vector3D
    {
    return Vector3D(0, 0, 0);
    };
  }

void ParticleSystem::setParticleSpawnSphere(double radius)
  {
  using namespace mathernogl;
  particleSpawnFunc = [radius]()->Vector3D
    {
    Vector3D spawnOffset(radius * 5);
    while (spawnOffset.magnitude() > radius)
      spawnOffset.set(RandomGenerator::randomDouble(-radius, radius), RandomGenerator::randomDouble(-radius, radius), RandomGenerator::randomDouble(-radius, radius));
    return spawnOffset;
    };
  }

void ParticleSystem::setParticleSpawnLine(const Vector3D& first, const Vector3D& second)
  {
  using namespace mathernogl;
  particleSpawnFunc = [first, second]()->Vector3D
    {
    double lineOffset = RandomGenerator::randomFloat(0, 1);
    return first + (second - first) * lineOffset;
    };
  }

void ParticleSystem::setParticleSpawnCircle(double radius, const Vector3D& normal)
  {
  //todo: pre generate random samples to reuse over time
  using namespace mathernogl;
  Vector3D w = normal.getUniform();
  Vector3D u = crossProduct(Vector3D(0.0023, 1, 0.0048), w);
  u.makeUniform();
  Vector3D v = crossProduct(u, w);
  particleSpawnFunc = [radius, u, v, w]()->Vector3D
    {
    Vector3D sample(radius * 5);
    while (sample.magnitude() > radius)
      sample.set(RandomGenerator::randomDouble(-radius, radius), RandomGenerator::randomDouble(-radius, radius), 0);
    Vector3D offset = (u * sample.x) + (v * sample.y) + (w * sample.z);
    return offset;
    };
  }

void ParticleSystem::setParticleDirectionRandom()
  {
  using namespace mathernogl;
  particleDirectionFunc = []()->Vector3D
    {
    Vector3D sample(5);
    while (sample.magnitude() > 1)
      sample.set(RandomGenerator::randomDouble(-1, 1), RandomGenerator::randomDouble(-1, 1), RandomGenerator::randomDouble(-1, 1));
    return sample.getUniform();
    };
  }

void ParticleSystem::setParticleDirection(const Vector3D& direction)
  {
  Vector3D dir = direction.getUniform();
  particleDirectionFunc = [dir]()->Vector3D
    {
    return dir;
    };
  }

void ParticleSystem::setParticleDirectionHemisphere(const Vector3D& direction, double exp)
  {
  using namespace mathernogl;
  Vector3D w = direction.getUniform();
  Vector3D u = crossProduct(Vector3D(0.0023, 1, 0.0048), w);
  u.makeUniform();
  Vector3D v = crossProduct(u, w);
  particleDirectionFunc = [exp, u, v, w]()->Vector3D
    {
    Vector3D sample = Vector3D(RandomGenerator::randomFloat(0, 1), RandomGenerator::randomFloat(0, 1), 0);
    double cosPhi = cos(2.0*M_PI*sample.x);
    double sinPhi = sin(2.0*M_PI*sample.x);
    double cosTheta = pow(1.0-sample.y, 1.0/(exp+1.0));
    double sinTheta = sqrt(1.0 - cosTheta*cosTheta);
    Vector3D hemispherePoint(sinTheta*cosPhi, sinTheta*sinPhi, cosTheta);
    Vector3D randomDirection = (u * hemispherePoint.x) + (v * hemispherePoint.y) + (w * hemispherePoint.z);
    return randomDirection.getUniform();
    };
  }

void ParticleSystem::setTranslation(const Vector3D& translation)
  {
  Vector3D offset = translation - this->translation;
  if (offset.x == 0 && offset.y == 0 && offset.z == 0)
    return;

  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (item.baseEmitter)
      {
      item.baseEmitter->translate(offset);
      }
    }
  this->translation = translation;
  }

void ParticleSystem::stopEmitters()
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (item.baseEmitter)
      item.baseEmitter->setSpawningState(false);
    }
  }

void ParticleSystem::startEmitters()
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (item.baseEmitter && item.timeToLive > timeAlive)
      item.baseEmitter->setSpawningState(true);
    }
  }
