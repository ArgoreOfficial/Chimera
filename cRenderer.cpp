#include "cRenderer.h"

#include <cm/backends/cBackend_OpenGL.h>
#include <cm/backends/cBackend_D3D11.h>

cm::cRenderer::cRenderer( void ):
	m_backend{ nullptr }
{

}

cm::cRenderer::~cRenderer( void )
{

}

void cm::cRenderer::create( cWindow& _window, cRenderer::eBackendType _backend )
{
	switch ( _backend )
	{
	case OpenGL:
		m_backend = new cBackend_OpenGL();
		break;
	case Direct3D11:
		m_backend = new cBackend_D3D11();
		break;
	default:
		m_backend = new cBackend_OpenGL();
	}

	m_backend->create( _window );

	createDefaultShader();
}

cm::sShader cm::cRenderer::createShader( const char* _source, eShaderType _type )
{
    return m_backend->createShader( _source, _type );
}

void cm::cRenderer::onResize( int _width, int _height )
{
	m_backend->onResize( _width, _height );
}

void cm::cRenderer::clear( unsigned int _color )
{
	m_backend->clear( _color );
}

void cm::cRenderer::beginFrame( void )
{
	m_backend->beginFrame();
}

void cm::cRenderer::endFrame( void )
{
	m_backend->endFrame();
}

void cm::cRenderer::createDefaultShader( void )
{
	sShader vertex_shader   = m_backend->createShader( m_shader_default_vert, eShaderType::Shader_Vertex );
	sShader fragment_shader = m_backend->createShader( m_shader_default_frag, eShaderType::Shader_Fragment );
	
	m_shader_default = m_backend->createShaderProgram();
	
	m_backend->attachShader( m_shader_default, vertex_shader );
	m_backend->attachShader( m_shader_default, fragment_shader );

	m_backend->linkShaderProgram( m_shader_default );
}
