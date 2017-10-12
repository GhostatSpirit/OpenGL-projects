#pragma once
#include "sb7.h"

namespace sb7 {

	sb7::application * sb7::application::appli;

	void sb7::application::run(sb7::application* the_app)
	{
		bool running = true;

		appli = the_app;

		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			return;
		}

		init();

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
		if (info.flags.debug)
#endif /* _DEBUG */
		{
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		}
		if (info.flags.robust)
		{
			glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
		}
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, info.samples);
		glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
		//        if (info.flags.fullscreen)
		//        {
		//            if (info.windowWidth == 0 || info.windowHeight == 0)
		//            {
		//                GLFWvidmode mode;
		//                glfwGetDesktopMode(&mode);
		//                info.windowWidth = mode.Width;
		//                info.windowHeight = mode.Height;
		//            }
		//
		//            glfwOpenWindow(info.windowWidth, info.windowHeight, 8, 8, 8, 0, 32, 0, GLFW_FULLSCREEN);
		//            glfwSwapInterval((int)info.flags.vsync);
		//        }
		//        else
		{
			window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
			if (!window)
			{
				fprintf(stderr, "Failed to open window\n");
				return;
			}
		}

		glfwMakeContextCurrent(window);

		glfwSetWindowSizeCallback(window, glfw_onResize);
		glfwSetKeyCallback(window, glfw_onKey);
		glfwSetMouseButtonCallback(window, glfw_onMouseButton);
		glfwSetCursorPosCallback(window, glfw_onMouseMove);
		glfwSetScrollCallback(window, glfw_onMouseWheel);
		if (!info.flags.cursor)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		// info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			exit(-1);
		}

#ifdef _DEBUG
		fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
		fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
		fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

		if (info.flags.debug)
		{
			if (glewIsSupported("GL_VERSION_4_3"))
			{
				glDebugMessageCallback((GLDEBUGPROC)debug_callback, this);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			}
			else if (glewGetExtension("GL_ARB_debug_output"))
			{
				glDebugMessageCallbackARB((GLDEBUGPROC)debug_callback, this);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			}
		}

		startup();
		startupTime = glfwGetTime();

		do
		{
			render(glfwGetTime() - startupTime);

			glfwSwapBuffers(window);
			glfwPollEvents();

			running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
			running &= (glfwWindowShouldClose(window) != GL_TRUE);
		} while (running);

		shutdown();

		glfwDestroyWindow(window);
		glfwTerminate();

	}
}