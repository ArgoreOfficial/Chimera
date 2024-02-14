#pragma once

/*
 *
 * Shader.h
 * 
 * Type definitions for shader handles
 * 
 */

#include <string>

namespace cm
{
	namespace Shader
	{
		typedef unsigned int hShader;
		typedef unsigned int hShaderProgram;

		enum eShaderType
		{
			ShaderType_Vertex, 
			ShaderType_Fragment,
			ShaderType_Geometry,
			ShaderType_Compute /* not implemented */
		};

		struct sShader
		{
			const hShader handle;
			const eShaderType type;
		};

		struct sShaderUniform
		{
			std::string name;
			int location;
			int type;
		};
	}
}
