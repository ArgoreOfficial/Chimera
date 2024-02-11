#pragma once

/*
 *
 * Shader.h
 * 
 * Type definitions for shader handles
 * 
 */

namespace cm
{
	typedef unsigned int hShader;
	typedef unsigned int hShaderProgram;

	enum eShaderType
	{
		Shader_Vertex, 
		Shader_Fragment,
		Shader_Geometry,
		Shader_Compute /* not implemented */
	};

	struct sShader
	{
		const hShader handle;
		const eShaderType type;
	};
}
