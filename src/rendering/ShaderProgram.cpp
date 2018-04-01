#include <mathernogl/rendering/ShaderProgram.h>
#include <mathernogl/maths/Vector4D.h>

namespace mathernogl {



void ShaderProgram::init(const std::vector<Shader> &shaders) {
	std::vector<unsigned int> glShaderIDs;
	try {
        for(const Shader& shader : shaders){
            std::string shaderSrc = readTextFile(shader.filePath);
            glShaderIDs.push_back(createShader(shader.glTypeID, shaderSrc));
        }
        this->glID = createProgram(glShaderIDs);
        for(GLuint id : glShaderIDs){
            glDeleteShader(id);
        }
	}
	catch (const std::runtime_error& e) {
		std::string errorMsg = e.what();
		throw std::runtime_error("Failed to load shader program. " + errorMsg);
	}
}

void ShaderProgram::init(const std::string& vsFilePath, const std::string& fsFilePath) {
    init({Shader(GL_VERTEX_SHADER, vsFilePath), Shader(GL_FRAGMENT_SHADER, fsFilePath)});
}

void ShaderProgram::init(const std::string &vsFilePath, const std::string &tcsFilePath, const std::string &tesFilePath, const std::string &fsFilePath) {
    init({Shader(GL_VERTEX_SHADER, vsFilePath), Shader(GL_TESS_CONTROL_SHADER, tcsFilePath), Shader(GL_TESS_EVALUATION_SHADER, tesFilePath), Shader(GL_FRAGMENT_SHADER, fsFilePath)});
}

void ShaderProgram::initFromSrc(const std::string& vertexShaderCode, const std::string& fragmentShaderCode){
  std::vector<unsigned int> glShaderIDs;
  try {
    glShaderIDs.push_back(createShader(GL_VERTEX_SHADER, vertexShaderCode));
    glShaderIDs.push_back(createShader(GL_FRAGMENT_SHADER, fragmentShaderCode));
    this->glID = createProgram(glShaderIDs);
    for(GLuint id : glShaderIDs){
      glDeleteShader(id);
    }
  }
  catch (const std::runtime_error& e) {
    std::string errorMsg = e.what();
    throw std::runtime_error("Failed to load shader program. " + errorMsg);
  }
}

unsigned int ShaderProgram::createShader(GLuint glType, const std::string& shaderSrc) {
	//load and compile shader into openGL
	GLuint glShaderID = glCreateShader(glType);
	const char *shaderFileString = shaderSrc.c_str();
	glShaderSource(glShaderID, 1, &shaderFileString, nullptr);
	glCompileShader(glShaderID);

	//check compilation status
	GLint compilationStatus;
	glGetShaderiv(glShaderID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(glShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(glShaderID, infoLogLength, nullptr, infoLog);
		std::string errorLogContents = "Unknown";
		switch (glType) {
            case GL_VERTEX_SHADER:
                errorLogContents = "Vertex";
                break;
            case GL_TESS_CONTROL_SHADER:
                errorLogContents = "Tessellation Control";
                break;
            case GL_TESS_EVALUATION_SHADER:
                errorLogContents = "Tessellation Evaluation";
                break;
            case GL_GEOMETRY_SHADER:
                errorLogContents = "Geometry";
                break;
            case GL_FRAGMENT_SHADER:
                errorLogContents = "Fragment";
                break;
		}
		errorLogContents += " Shader compile error log: \n";
		errorLogContents += infoLog;
		delete[] infoLog;

		throw std::runtime_error("Couldn't compile shader. \n" + errorLogContents);
	}

	return glShaderID;
}

unsigned int ShaderProgram::createProgram(const std::vector<unsigned int>& glShaderIDs) {
	//attach and link shaders to program
	GLuint programID = glCreateProgram();
	for (GLuint id : glShaderIDs) {
		glAttachShader(programID, id);
	}
	glLinkProgram(programID);

	//check linking status
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	std::string errorLogContents;
	if (linkStatus == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(programID, infoLogLength, nullptr, infoLog);
		errorLogContents = "Shader program linking error: \n";
		errorLogContents += infoLog;
		delete[] infoLog;
	}

	//detach all shaders
	for (GLuint id : glShaderIDs) {
		glDetachShader(programID, id);
	}
	if (linkStatus == GL_FALSE) {
		throw std::runtime_error("Couldn't link shader program. \n" + errorLogContents);
	}
	return programID;
}

void ShaderProgram::cleanUp() {
	glDeleteProgram(glID);
	glID = 0;
}

//must enable shader program before performing this operation
void ShaderProgram::setVarFloat(const std::string& name, float value, bool optional) {
  if (optional && !hasUniformVariable(name))
    return;
	glUniform1f(getUniformLocation(name), value);
}

//must enable shader program before performing this operation
void ShaderProgram::setVarInt(const std::string& name, int value, bool optional) {
  if (optional && !hasUniformVariable(name))
    return;
	glUniform1i(getUniformLocation(name), value);
}

std::string constructArrayName(const std::string& name, int index){
  return name + "[" + std::to_string(index) + "]";
}

//must enable shader program before performing this operation
void ShaderProgram::setVarIntArray(const std::string& name, int index, int value, bool optional){
  if (optional && !hasUniformVariable(name))
    return;
  glUniform1i(getUniformLocation(constructArrayName(name, index)), value);
}

//must enable shader program before performing this operation
void ShaderProgram::setVarVec2(const std::string &name, const Vector2D &value, bool optional) {
  if (optional && !hasUniformVariable(name))
    return;
  glUniform2f(getUniformLocation(name), value.x, value.y);
}

//must enable shader program before performing this operation
void ShaderProgram::setVarVec3(const std::string &name, const Vector3D& value, bool optional) {
  if (optional && !hasUniformVariable(name))
    return;
  glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

//must enable shader program before performing this operation
void ShaderProgram::setVarVec4(const std::string& name, const Vector4D& value, bool optional) {
  if (optional && !hasUniformVariable(name))
    return;
  glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

  //must enable shader program before performing this operation
void ShaderProgram::setVarMat4(const std::string &name, const Matrix4& value, bool optional) {
  if (optional && !hasUniformVariable(name))
    return;
  const double* dmatrix = value.getCArray();
  float matrix[16];
  std::copy(dmatrix, dmatrix + 16, matrix);
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, matrix);
}

void ShaderProgram::enable() const {
	glUseProgram(glID);
}

void ShaderProgram::disable() const {
	glUseProgram(0);
}

bool ShaderProgram::hasUniformVariable(const std::string& name) const {
  return glGetUniformLocation(glID, name.c_str()) >= 0;
}

int ShaderProgram::getUniformLocation(const std::string& name) {
	int glUniformID = glGetUniformLocation(glID, name.c_str());
    if (glUniformID < 0) {
        throw std::runtime_error("Couldn't set shader program variable of name: " + name);
    }
    return glUniformID;
}

}
