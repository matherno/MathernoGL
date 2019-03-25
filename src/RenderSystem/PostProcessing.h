#pragma once

#include "RenderSystem.h"

/*
*   
*/

class PostProcSimpleBase : public PostProcStepHandler
  {
private:
  string fragmentShaderFilePath;

protected:
  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray screenQuadVBO;

public:
  PostProcSimpleBase(uint id, int stepOrder, const string& fragmentShaderFilePath);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext, FrameBufferPtr screenFBO) override;
  };