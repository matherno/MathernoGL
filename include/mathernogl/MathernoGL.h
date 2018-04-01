#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <mathernogl/data/GPUBuffer.h>
#include <mathernogl/data/GPUBufferStatic.h>
#include <mathernogl/data/VertexArray.h>

#include <mathernogl/world/CameraController.h>

#include <mathernogl/image/ImageData.h>
#include <mathernogl/image/Image.h>

#include <mathernogl/texture/Texture.h>
#include <mathernogl/texture/TextureFactory.h>
#include <mathernogl/texture/TextureBindingsManager.h>

#include <mathernogl/rendering/ShaderProgram.h>
#include <mathernogl/rendering/ScreenTextureRenderer.h>

#include <mathernogl/systemio/Window.h>
#include <mathernogl/systemio/InputHandler.h>

#include <mathernogl/utils/FileIO.h>
#include <mathernogl/utils/Logging.h>
#include <mathernogl/utils/ImageImporter.h>
#include <mathernogl/utils/GLUtils.h>
#include <mathernogl/utils/MeshUtils.h>
#include <mathernogl/utils/MappedList.h>
#include <mathernogl/utils/Misc.h>
#include <mathernogl/utils/FontImporter.h>

#include <mathernogl/maths/Maths.h>

#include <mathernogl/Assert.h>
#include <mathernogl/Types.h>

#include <tiny_obj_loader.h>
