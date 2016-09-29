/*
 * Loads and manages a shader
 */

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace vtk {

//forward declaration
class ShaderUniform;

class Shader {
public:
	Shader();
    bool activate(); //Must be called to use shader

    //Load shader, type 0 = Vert, 1 = Frag, 2 = Geometry
    bool loadShaderFiles(const std::string& vertShader,
	                     const std::string& fragShader,
					     const std::string& geomShader = "");

    
	GLuint loadShader(const std::string& code, const GLenum& shaderType);
	ShaderUniform getUniform(const std::string& name);

protected:
	GLuint mShaderID;

};


//uniform setter
class ShaderUniform {
public:
	ShaderUniform(const GLint& uniformID);
	void set(const glm::mat4& data);
	void set(const glm::vec4& data);
	void set(const glm::vec3& data);
protected:
	GLint mUniformID;
};

}
