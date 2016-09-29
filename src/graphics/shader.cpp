/*
 * Shader source
 */
#include <glm/gtc/type_ptr.hpp>

#include "graphics/shader.h"
#include "graphics/glstate.h"
#include "spdlog/spdlog.h"
#include "fileutils.h"
#include "loadShader.h"

namespace vtk {

Shader::Shader() {
	mShaderID = 0;
}

bool Shader::activate() {
	if (mShaderID == 0){
		spdlog::get("general")->warn("Attempted to load unloaded shader");
		return false;
	}
	gls::setShader(mShaderID);
	//glUseProgram(mShaderID);
	return true;
}


bool Shader::loadShaderFiles(const std::string& vertShader,
					         const std::string& fragShader,
					         const std::string& geomShader)
{
	auto log = spdlog::get("general");
	bool hasGeomShader = geomShader != "";
	// IDs
	GLuint vShaderID;
	GLuint fShaderID;
	GLuint gShaderID;

	auto vShaderCode = file::loadFileIntoString(vertShader);
	auto fShaderCode = file::loadFileIntoString(fragShader);

	std::string gShaderCode; //only load geometry shader if path is provided
	if (hasGeomShader) {
		gShaderCode = file::loadFileIntoString(geomShader);
	}

    log->debug("Compiling shader: {}", vertShader);
	vShaderID = loadShader(vShaderCode, GL_VERTEX_SHADER);

    log->debug("Compiling shader: {}", fragShader);
	fShaderID = loadShader(fShaderCode, GL_FRAGMENT_SHADER);
	
    // Link the program
    mShaderID = glCreateProgram();
    glAttachShader(mShaderID, vShaderID);
    glAttachShader(mShaderID, fShaderID);
    glLinkProgram(mShaderID);

	GLint result;
	int infoLogLength;

    // Check the program
    glGetProgramiv(mShaderID, GL_LINK_STATUS, &result);
    glGetProgramiv(mShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> ProgramErrorMessage(std::max(infoLogLength, int(1)) );
    glGetProgramInfoLog(mShaderID, infoLogLength, NULL, &ProgramErrorMessage[0]);
	log->debug("Linking errors: {}", &ProgramErrorMessage[0]);
 
    glDeleteShader(vShaderID);
    glDeleteShader(fShaderID);	

	return true;
}

GLuint Shader::loadShader(const std::string& code, const GLenum& shaderType) {

	GLuint id = glCreateShader(shaderType);
	GLint result = GL_FALSE;
	int infoLogLength;

    char const * sourcePointer = code.c_str();
    glShaderSource(id, 1, &sourcePointer , NULL);
    glCompileShader(id);

    // Check Shader
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> shaderErrorMessage(std::max(infoLogLength, int(1)));
    glGetShaderInfoLog(id, infoLogLength, NULL, &shaderErrorMessage[0]);
	spdlog::get("general")->debug("Shader errors: {}", &shaderErrorMessage[0]);
	return id;
}


ShaderUniform Shader::getUniform(const std::string& name) {
	return(ShaderUniform(glGetAttribLocation(mShaderID, name.c_str())));
}

ShaderUniform::ShaderUniform(const GLint& uniformID) :
	mUniformID(uniformID)
{

}
void ShaderUniform::set(const glm::mat4& data) {
	glUniformMatrix4fv(mUniformID, 1, GL_FALSE, glm::value_ptr(data)); 
}

void ShaderUniform::set(const glm::vec4& data) {
	glUniform4fv(mUniformID, 1, glm::value_ptr(data));
}

void ShaderUniform::set(const glm::vec3& data) {
	glUniform3fv(mUniformID, 1, glm::value_ptr(data));
}

//namespace vtk
}
