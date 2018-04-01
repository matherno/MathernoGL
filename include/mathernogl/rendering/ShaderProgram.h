#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>

#include <mathernogl/utils/FileIO.h>
#include <mathernogl/utils/Logging.h>
#include <mathernogl/maths/Matrix4.h>
#include <mathernogl/maths/Vector3D.h>
#include <mathernogl/maths/Vector4D.h>

namespace mathernogl {

struct Shader{
    GLuint glTypeID;
    std::string filePath;
    Shader(GLuint glTypeID, const std::string &filePath) : glTypeID(glTypeID), filePath(filePath) {}
};

class ShaderProgram {
private:
	unsigned int glID = 0;

public:
	void init(const std::string& vsFilePath, const std::string& fsFilePath);
	void init(const std::string& vsFilePath, const std::string& tcsFilePath, const std::string& tesFilePath, const std::string& fsFilePath);
	void init(const std::vector<Shader>& shaders);
	void initFromSrc(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
	void cleanUp();
	void setVarFloat(const std::string& name, float value, bool optional = false);
	void setVarInt(const std::string& name, int value, bool optional = false);
	void setVarIntArray(const std::string& name, int index, int value, bool optional = false);
	void setVarVec2(const std::string& name, const Vector2D &value, bool optional = false);
	void setVarVec3(const std::string& name, const Vector3D &value, bool optional = false);
	void setVarVec4(const std::string& name, const Vector4D &value, bool optional = false);
	void setVarMat4(const std::string& name, const Matrix4 &value, bool optional = false);
	void enable() const;
	void disable() const;
  bool hasUniformVariable(const std::string& name) const;
  unsigned int getGLID() const { return glID; }

private:
	unsigned int createShader(GLuint glType, const std::string& shaderSrc);
	unsigned int createProgram(const std::vector<unsigned int>& glShaderIDs);
	int getUniformLocation(const std::string& name);
};

}

