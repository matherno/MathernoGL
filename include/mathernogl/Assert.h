#pragma once
//
// Created by matt on 10/06/17.
//

#include <cstring>
#include <stdlib.h>

#include "mathernogl/utils/Logging.h"

namespace mathernogl{

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define FILE_LINE_DESC std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + ", " + std::string(__PRETTY_FUNCTION__)

#ifdef NDEBUG
#define ASSERT(cond, msg)
#define ASSERT_NO_GL_ERROR()
#else
#define ASSERT(cond, msg) if(!(cond)){ mathernogl::logError("Assert failed in " + FILE_LINE_DESC + ". " + msg); abort(); }
#define ASSERT_NO_GL_ERROR() {uint error = glGetError(); ASSERT(error == 0, "OpenGL Error: " + std::to_string(error));}
#endif

}