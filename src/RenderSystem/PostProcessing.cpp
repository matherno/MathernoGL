//
// Created by matt on 26/05/18.
//

#include "PostProcessing.h"

/*
 *  PostProcSimpleBase
 */

PostProcSimpleBase::PostProcSimpleBase(uint id, int stepOrder, const string& fragmentShaderFilePath)
      : PostProcStepHandler(id, stepOrder), fragmentShaderFilePath(fragmentShaderFilePath)
  {}

void PostProcSimpleBase::initialise(RenderContext* renderContext)
  {
  using namespace mathernogl;
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/ScreenQuadVS.glsl"), Shader(GL_FRAGMENT_SHADER, fragmentShaderFilePath) };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);

  GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(
    {
      -1, -1,
      1, -1,
      1, 1,
      -1, -1,
      1, 1,
      -1, 1,
    });
  screenQuadVBO.init();
  screenQuadVBO.bind();
  screenQuadVBO.linkBufferAsFloats(buffer, 2, 0, false);
  screenQuadVBO.unbind();
  buffer.cleanUp();
  }

void PostProcSimpleBase::cleanUp(RenderContext* renderContext)
  {
  screenQuadVBO.cleanUp();
  }

void PostProcSimpleBase::render(RenderContext* renderContext, FrameBufferPtr screenFBO)
  {
  renderContext->activateShaderProgram(shaderProgram);
  shaderProgram->setVarInt("inColourTexture", renderContext->bindTexture(screenFBO->getColourTexture()), true);
  shaderProgram->setVarInt("inDepthTexture", renderContext->bindTexture(screenFBO->getDepthTexture()), true);
  screenQuadVBO.bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  }