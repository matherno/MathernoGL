#pragma once

#include <RenderSystem/RenderSystem.h>
#include "ParticleEmitter.h"

/*
*   
*/

class RenderableParticles : public Renderable
  {
private:
  ParticleEmitterPtr emitter;
  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic particlesBuffer;
  bool renderAsPoints = true;
  Vector3D colour = Vector3D(1, 0, 0);
  double size = 3.0f;
  std::vector<TexturePtr> textures;
  int numTexAtlasRows = 1;
  int numTexAtlasColumns = 1;
  double colourMixFactor = 1;
  bool additiveBlending = false;
  bool depthTesting = false;

public:
  RenderableParticles(uint id, bool renderAsPoints = true);

  void setEmitter(ParticleEmitterPtr emitter);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void renderPoints(RenderContext* renderContext);
  void renderQuads(RenderContext* renderContext);

  void setColour(const Vector3D& colour);
  void setParticleSize(double size);
  void setTextureColourMixFactor(double factor){ colourMixFactor = factor; }
  void setTextureAtlasSize(int numCols, int numRows){ numTexAtlasColumns = numCols, numTexAtlasRows = numRows; }
  void addTextureAtlas(TexturePtr textureAtlas);
  void setAdditiveBlending(bool active){ additiveBlending = active; }
  void setDepthTesting(bool active) { depthTesting = active; }

protected:
  float calcFadeValue(Particle* particle) const;
  };
