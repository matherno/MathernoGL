#pragma once

#include <GameSystem/GameSystem.h>
#include "ParticleEmitter.h"
#include "utils/MappedList.h"

/*
*   
*/

typedef std::function<bool()> EndEmitterCallback;

struct ParticleSystemItem
  {
  uint id;
  ParticleEmitterPtr baseEmitter;
  RenderablePtr renderable;
  long timeToLive;
  EndEmitterCallback endEmitterCallback;
  Vector3D colour;
  double size;
  };

class ParticleSystem : public GameActor
  {
private:
  mathernogl::MappedList<ParticleSystemItem> emitters;
  double gravityAccel = 0.00001;
  long timeBetweenSpawns = 30;
  long timeAlive = 1000;
  double size = 5.0f;
  double initVelocity = 0.008;
  uint nextEmitterID = 0;
  bool renderAsPoints;
  std::vector<TexturePtr> textures;
  int numTexAtlasRows = 1;
  int numTexAtlasColumns = 1;
  double colourMixFactor = 1;
  ParticleInitFunction particleSpawnFunc;
  ParticleInitFunction particleDirectionFunc;
  Vector3D translation;
  bool additiveBlending = false;
  bool depthTesting = true;

public:
  ParticleSystem(uint id, bool renderAsPoints);

  void setTranslation(const Vector3D& translation);
  Vector3D getTranslation() const { return translation; }
  void addEmitter(const Vector3D& position, long timeToLive, const Vector3D& colour = Vector3D(1), double particleSize = -1);
  void addEmitter(const Vector3D& position, EndEmitterCallback callback, const Vector3D& colour = Vector3D(1), double particleSize = -1);
  void clearEmitters();
  void stopEmitters();
  void startEmitters();
  void setGravityAccel(double gravityAccel);
  void setTimeBetweenSpawns(long timeBetweenSpawns, bool updateExisting = false);
  void setTimeAlive(long timeAlive);
  void setParticleSize(double size);
  void setInitVelocity(double velocity);
  void setAdditiveBlending(bool active) { additiveBlending = active; }
  void setDepthTesting(bool active) { depthTesting = active; }

  void setTextureColourMixFactor(double factor){ colourMixFactor = factor; }
  void setTextureAtlasSize(int numCols, int numRows){ numTexAtlasColumns = numCols, numTexAtlasRows = numRows; }
  void addTextureAtlas(TexturePtr textureAtlas);

  void setParticleSpawnPoint();
  void setParticleSpawnSphere(double radius);
  void setParticleSpawnLine(const Vector3D& first, const Vector3D& second);
  void setParticleSpawnCircle(double radius, const Vector3D& normal);

  void setParticleDirectionRandom();
  void setParticleDirection(const Vector3D& direction);
  void setParticleDirectionHemisphere(const Vector3D& direction, double exp);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void addEmitter(const Vector3D& position, long timeToLive, EndEmitterCallback callback, const Vector3D& colour, double particleSize);
  void initNewRenderables(RenderContext* renderContext);
  };

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;