/*
* Copyright � 2012-2013 Graham Sellers
*
* This code is part of the OpenGL SuperBible, 6th Edition.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice (including the next
* paragraph) shall be included in all copies or substantial portions of the
* Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

// Header file modified by Yang Liu
// lykavin@hotmail.com

#ifndef __SB7_H__
#define __SB7_H__

//#ifdef WIN32
//#pragma once
//#define _CRT_SECURE_NO_WARNINGS 1
//
//#define WIN32_LEAN_AND_MEAN 1
//#include <Windows.h>
//#else
//#include <unistd.h>
//#define Sleep(t) sleep(t)
//#endif
#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>

#include <GL/glew.h>

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>

namespace sb7
{

	class application
	{
	private:
		static void debug_callback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei lsength,
			const GLchar* message,
			GLvoid* userParam) {

			using namespace std;
			cout << "---------------------opengl-callback-start------------" << endl;
			cout << "message: " << message << endl;
			cout << "type: ";
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				cout << "ERROR";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				cout << "DEPRECATED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				cout << "UNDEFINED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				cout << "PORTABILITY";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				cout << "PERFORMANCE";
				break;
			case GL_DEBUG_TYPE_OTHER:
				cout << "OTHER";
				break;
			}
			cout << endl;

			cout << "id: " << id << endl;
			cout << "severity: ";
			switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:
				cout << "LOW";
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				cout << "MEDIUM";
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				cout << "HIGH";
				break;
			}
			cout << endl;
			cout << "---------------------opengl-callback-end--------------" << endl;
		}

	private:
		double startupTime = 0.0;

	public:
		application() {}
		virtual ~application() {}
		virtual void run(sb7::application* the_app);
	

		virtual void init()
		{
			strcpy(info.title, "OpenGL Playground Application");
			info.windowWidth = 1024;
			info.windowHeight = 768;
#ifdef __APPLE__
			info.majorVersion = 3;
			info.minorVersion = 2;
#else
			info.majorVersion = 3;
			info.minorVersion = 3;
#endif
			info.samples = 0;
			info.flags.all = 0;
			info.flags.cursor = 1;
#ifdef _DEBUG
			info.flags.debug = 1;
#endif
		}

		virtual void startup()
		{

		}

		virtual void render(double currentTime)
		{

		}

		virtual void shutdown()
		{

		}

		void setWindowTitle(const char * title)
		{
			glfwSetWindowTitle(window, title);
		}

		virtual void onResize(int w, int h)
		{
			info.windowWidth = w;
			info.windowHeight = h;
		}

		virtual void onKey(int key, int action)
		{

		}

		virtual void onMouseButton(int button, int action)
		{

		}

		virtual void onMouseMove(int x, int y)
		{

		}

		virtual void onMouseWheel(int pos)
		{

		}

		virtual void onDebugMessage(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message)
		{
#ifdef _WIN32
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
#else
			std::cout << message;
			std::cout << endl;
#endif /* _WIN32 */
		}

		void getMousePosition(int& x, int& y)
		{
			double dx, dy;
			glfwGetCursorPos(window, &dx, &dy);

			x = static_cast<int>(floor(dx));
			y = static_cast<int>(floor(dy));
		}

	public:
		struct APPINFO
		{
			char title[128];
			int windowWidth;
			int windowHeight;
			int majorVersion;
			int minorVersion;
			int samples;
			union
			{
				struct
				{
					unsigned int    fullscreen : 1;
					unsigned int    vsync : 1;
					unsigned int    cursor : 1;
					unsigned int    stereo : 1;
					unsigned int    debug : 1;
					unsigned int    robust : 1;
				};
				unsigned int        all;
			} flags;
		};

	protected:
		APPINFO     info;
		static      sb7::application * appli;
		GLFWwindow* window;

		static void glfw_onResize(GLFWwindow* window, int w, int h)
		{
			appli->onResize(w, h);
		}

		static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			appli->onKey(key, action);
		}

		static void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
		{
			appli->onMouseButton(button, action);
		}

		static void glfw_onMouseMove(GLFWwindow* window, double x, double y)
		{
			appli->onMouseMove(static_cast<int>(x), static_cast<int>(y));
		}

		static void glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
		{
			appli->onMouseWheel(static_cast<int>(yoffset));
		}

		void setVsync(bool enable)
		{
			info.flags.vsync = enable ? 1 : 0;
			glfwSwapInterval((int)info.flags.vsync);
		}

	};

};



#define DECLARE_MAIN(a)                             \
int main(int argc, const char ** argv)              \
{                                                   \
    a *app = new a;                                 \
    app->run(app);                                  \
    delete app;                                     \
    return 0;                                       \
}

#endif

/* __SB7_H__ */

