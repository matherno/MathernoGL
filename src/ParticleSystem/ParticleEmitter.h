#pragma once
//
// Created by matt on 27/03/17.
//

#include <algorithm>
#include <list>

#include <RenderSystem/RenderSystem.h>


struct Particle
  {
  Vector3D position;
  Vector3D velocity;
  long timeToLive;
  int texIndex;
  };
typedef std::unique_ptr<Particle> ParticlePtr;

typedef std::function<Vector3D()> ParticleInitFunction;

class ParticleEmitter
  {
private:
  std::list<ParticlePtr> particles;
  mathernogl::RandomGenerator randomGenerator;
  Vector3D position;
  long currentTime = 0;
  long lastSpawnTime = 0;
  double gravityAccel = 0.001;
  long timeBetweenSpawns = 1000;
  long timeAlive = 10000;
  double initVelocity = 0.05;
  bool isSpawning = true;
  ParticleInitFunction particleOffsetFunc;
  ParticleInitFunction particleDirectionFunc;
  int numTextureIndices = 0;

public:
  ParticleEmitter();

  void setPosition(const Vector3D& position){ this->position = position; }
  void translate(const Vector3D& translate){ this->position += translate; }
  double getGravityAccel() const;
  void setGravityAccel(double gravityAccel);
  long getTimeBetweenSpawns() const;
  void setTimeBetweenSpawns(long timeBetweenSpawns);
  long getTimeAlive() const;
  void setTimeAlive(long timeAlive);
  void createParticle(const Vector3D& initPosOffset, const Vector3D& velocity, long timeAlive);
  void updateParticles(long deltaTime);
  void setInitVelocity(double velocity){ initVelocity = velocity; }
  int count() { return (int)particles.size(); }
  void setSpawningState(bool isSpawning){ this->isSpawning = isSpawning; }
  void setNumTexturesIndices(int numTextures){ numTextureIndices = numTextures; }

  void setParticleOffsetFunc(ParticleInitFunction func){ particleOffsetFunc = func; }
  void setParticleDirectionFunc(ParticleInitFunction func){ particleDirectionFunc = func; }

  void sortParticles(const Matrix4* worldToCamera);

  const std::list<ParticlePtr>::const_iterator begin() const { return particles.cbegin(); }
  const std::list<ParticlePtr>::const_iterator end() const { return particles.cend(); }
  };

typedef std::shared_ptr<ParticleEmitter> ParticleEmitterPtr;


