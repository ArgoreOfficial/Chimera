#pragma once

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace cm
{
	class cWindow
	{
	public:
		 cWindow();
		~cWindow();

		int  create ( unsigned int _width, unsigned int _height, const char* _title );
		void destroy( void );

		void processInput( void );
		void display  ( void );

		void onResize( int _width, int _height );

		GLFWwindow* const getWindowObject( void ) { return m_window_object; }

		unsigned int getWidth   ( void ) { return m_width; }
		unsigned int getHeight  ( void ) { return m_height; }
		double       getTime    ( void ) { return glfwGetTime(); }
		bool         shouldClose( void ) { return glfwWindowShouldClose( m_window_object ); }
		float        getAspect  ( void );

		void setTitle( const char* _title ) { glfwSetWindowTitle( m_window_object, _title ); }
		void setVSync( bool _state )        { glfwSwapInterval( _state ); }
	private:

		GLFWwindow* m_window_object = nullptr;
		unsigned int m_width = 0;
		unsigned int m_height = 0;

	};
}

