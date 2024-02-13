#include "cBackend_OpenGL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h> /* temporary */

#include <cm/Core/cWindow.h>
#include <cm/Framework/cVertexLayout.h>

#include <stdio.h>


static GLuint getShaderType_OpenGL( cm::eShaderType _type )
{
	using namespace cm;

	switch ( _type )
	{
	case Shader_Vertex:   return GL_VERTEX_SHADER;   break;
	case Shader_Fragment: return GL_FRAGMENT_SHADER; break;
	case Shader_Geometry: return GL_GEOMETRY_SHADER; break;
	}

	return GL_NONE;
}

static GLuint getBufferTarget_OpenGL( cm::eBufferType _type )
{
	using namespace cm;

	switch ( _type )
	{
	case BufferType_Vertex: return GL_ARRAY_BUFFER;         break;
	case BufferType_Index:  return GL_ELEMENT_ARRAY_BUFFER; break;
	}

	return GL_BUFFER;
}

static GLuint getPrimitive_OpenGL( cm::eDrawMode _mode )
{
	using namespace cm;

	switch ( _mode )
	{
	case DrawMode_Lines:     return GL_LINES;      break;
	case DrawMode_LineLoop:  return GL_LINE_LOOP;  break;
	case DrawMode_LineStrip: return GL_LINE_STRIP; break;
	case DrawMode_Points:    return GL_POINTS;     break;
	case DrawMode_Triangle:  return GL_TRIANGLES;  break;
	}

	return GL_LINES;
}


cm::cBackend_OpenGL::cBackend_OpenGL()
{

}

cm::cBackend_OpenGL::~cBackend_OpenGL()
{

}

void cm::cBackend_OpenGL::create( cWindow& _window )
{
	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
	{
		fprintf( stderr, "Failed to initialize GLAD\n" );
		return;
	}
	glViewport( 0, 0, _window.getWidth(), _window.getHeight() );

	const GLubyte* renderer = glGetString( GL_RENDERER );
	const GLubyte* vendor = glGetString( GL_VENDOR );
	const GLubyte* version = glGetString( GL_VERSION );
	const GLubyte* glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

	GLint major, minor;
	glGetIntegerv( GL_MAJOR_VERSION, &major );
	glGetIntegerv( GL_MINOR_VERSION, &minor );

	printf( "Renderer Backend: OpenGL\n" );
	printf( "   Vendor            : %s\n", vendor );
	printf( "   Renderer          : %s\n", renderer );
	printf( "   Version (string)  : %s\n", version );
	printf( "   Version (integer) : %d.%d\n", major, minor );
	printf( "   GLSL Version      : %s\n", glslVersion );


	// ATTRIBUTES

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	glEnable( GL_PROGRAM_POINT_SIZE );
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
}

void cm::cBackend_OpenGL::clear( unsigned int _color )
{
	float r = ( _color & 0xFF000000 ) / 256.0f;
	float g = ( _color & 0x00FF0000 ) / 256.0f;
	float b = ( _color & 0x0000FF00 ) / 256.0f;
	float a = ( _color & 0x000000FF ) / 256.0f;

	glClearColor( r, g, b, a );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void cm::cBackend_OpenGL::onResize( int _width, int _height )
{
	glViewport( 0, 0, _width, _height );
}

cm::sShader cm::cBackend_OpenGL::createShader( const char* _source, eShaderType _type )
{
	int  success;
	char info_log[ 512 ];
	unsigned int shader;

	shader = glCreateShader( getShaderType_OpenGL( _type ) );

	glShaderSource( shader, 1, &_source, NULL );
	glCompileShader( shader );

	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		glGetShaderInfoLog( shader, 512, NULL, info_log );
		printf( "ERROR::SHADER::COMPILATION_FAILED\n %s \n", info_log );
	}

	return { shader, _type };
}

cm::hShaderProgram cm::cBackend_OpenGL::createShaderProgram()
{
	hShaderProgram program = glCreateProgram();

	return program;
}

cm::sBuffer cm::cBackend_OpenGL::createBuffer( eBufferType _type )
{
	hBuffer buffer = 0;
	glGenBuffers( 1, &buffer );
	glBindBuffer( getBufferTarget_OpenGL( _type ), buffer );

	return { buffer, _type };
}

cm::hVertexArray cm::cBackend_OpenGL::createVertexArray()
{
	hVertexArray vertex_array;
	glGenVertexArrays( 1, &vertex_array );

	return vertex_array;
}

cm::sTexture2D cm::cBackend_OpenGL::createTexture()
{
	sTexture2D texture{ 0,0,0,0 };
	glGenTextures( 1, &texture.handle );

	return texture;
}

void cm::cBackend_OpenGL::attachShader( hShaderProgram& _program, sShader& _shader )
{
	glAttachShader( _program, _shader.handle );
}

void cm::cBackend_OpenGL::linkShaderProgram( hShaderProgram& _program )
{
	int  success;
	char info_log[ 512 ];

	glLinkProgram( _program );

	glGetProgramiv( _program, GL_LINK_STATUS, &success );
	if ( !success )
	{
		glGetProgramInfoLog( _program, 512, NULL, info_log );
		printf( "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n %s \n", info_log );
	}
}

void cm::cBackend_OpenGL::generateTexture( sTexture2D _texture, unsigned char* _data )
{
	if ( !_data )
	{
		printf( "No texture data.\n" );
		return;
	}

	glBindTexture( GL_TEXTURE_2D, _texture.handle );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// filtering, TODO: implement as flag
	// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	GLint format = GL_RGBA;
	switch ( _texture.num_channels )
	{
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	}

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _texture.width, _texture.height, 0, format, GL_UNSIGNED_BYTE, _data );
	glGenerateMipmap( GL_TEXTURE_2D );
}

void cm::cBackend_OpenGL::useShaderProgram( hShaderProgram _program )
{
	glUseProgram( _program );
}

void cm::cBackend_OpenGL::bufferData( sBuffer& _buffer, void* _data, size_t _size )
{
	int target = getBufferTarget_OpenGL( _buffer.type );
	if ( !target )
		return;

	glBufferData( target, _size, _data, GL_DYNAMIC_DRAW ); /* move usage to buffer object */

}

void cm::cBackend_OpenGL::bindVertexLayout( cVertexLayout& _layout )
{
	const auto& elements = _layout.getElements();
	unsigned int offset = 0;

	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
		const auto& element = elements[ i ];

		GLenum type = GL_UNSIGNED_BYTE;

		switch ( element.type )
		{
		case cVertexLayout::Float:  type = GL_FLOAT;         break;
		case cVertexLayout::Uint:   type = GL_UNSIGNED_INT;  break;
		case cVertexLayout::Byte:   type = GL_UNSIGNED_BYTE; break;
		case cVertexLayout::Double: type = GL_DOUBLE;        break;
		}

		glEnableVertexAttribArray( i );
		glVertexAttribPointer( i, element.count, type, false, _layout.getStride(), (const void*)offset );
		offset += element.count * (unsigned int)element.size;
	}
}

void cm::cBackend_OpenGL::bindVertexArray( hVertexArray _vertex_array )
{
	glBindVertexArray( _vertex_array );
}

void cm::cBackend_OpenGL::bindTexture2D( hTexture _texture )
{
	glBindTexture( GL_TEXTURE_2D, _texture );
}

void cm::cBackend_OpenGL::drawArrays( unsigned int _vertex_count, eDrawMode _mode )
{
	glDrawArrays( getPrimitive_OpenGL( _mode ), 0, _vertex_count );
}

void cm::cBackend_OpenGL::drawElements( unsigned int _index_count, eDrawMode _mode )
{
	glDrawElements( getPrimitive_OpenGL( _mode ), _index_count, GL_UNSIGNED_INT, 0 );
}

int cm::cBackend_OpenGL::getUniformLocation( hShaderProgram _shader, const char* _uniform )
{
	return glGetUniformLocation( _shader, _uniform );;
}

void cm::cBackend_OpenGL::setUniformMat4f( int _location, float* _matrix_ptr )
{
	glUniformMatrix4fv( _location, 1, GL_FALSE, _matrix_ptr );
}

void cm::cBackend_OpenGL::setUniformFloat( int _location, float _float )
{
	glUniform1f( _location, _float );
}

void cm::cBackend_OpenGL::setUniformVec4f( int _location, wv::cVector4<float> _vector )
{
	glUniform4f( _location, _vector.x, _vector.y, _vector.z, _vector.w );
}

void cm::cBackend_OpenGL::setUniformVec4d( int _location, wv::cVector4<double> _vector )
{
	glUniform4d( _location, _vector.x, _vector.y, _vector.z, _vector.w );
}
