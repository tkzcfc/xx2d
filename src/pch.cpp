﻿#include "pch.h"

// todo: mouse, touch, keyboard, joystick support
// todo: node( with matrix )

// todo: more texture format support?
// todo: Program manager? ready for shader switch
// todo: draw instance for particle system?
// todo: texture array support?
// todo: spine? webassembly? lua? 




// some code snippets put here

/*


//#ifdef OPEN_MP_NUM_THREADS
//	omp_set_num_threads(OPEN_MP_NUM_THREADS);
//	omp_set_dynamic(0);
//#endif

	int n = (int)ss.size();
#ifdef OPEN_MP_NUM_THREADS
#pragma omp parallel for
#endif
	for (int i = 0; i < n; ++i) {





glfwSetWindowUserPointer(wnd, ptr)
glfwGetWindowUserPointer(window)

//glfwDefaultWindowHints();
//glfwxSetParent((HWND)0);
//glfwWindowHint(GLFW_RED_BITS, 8);
//glfwWindowHint(GLFW_GREEN_BITS, 8);
//glfwWindowHint(GLFW_BLUE_BITS, 8);
//glfwWindowHint(GLFW_ALPHA_BITS, 8);
//glfwWindowHint(GLFW_DEPTH_BITS, 24);
//glfwWindowHint(GLFW_STENCIL_BITS, 8);
//glfwWindowHint(GLFW_SAMPLES, 0);
//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // GLFW_OPENGL_ES_API
//glfwWindowHint(GLFW_AUX_BUFFERS, 0);
//glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
//glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
//glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
//glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
//glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//	glfwSetJoystickCallback(nullptr);


//int realW = 0, realH = 0;
//glfwGetWindowSize(_mainWindow, &realW, &realH);
//if (realW != neededWidth) {
//	rect.size.width = realW / _frameZoomFactor;
//}
//if (realH != neededHeight) {
//	rect.size.height = realH / _frameZoomFactor;
//}

//glfwSetMouseButtonCallback(wnd, GLFWEventHandler::onGLFWMouseCallBack);
//glfwSetCursorPosCallback(wnd, GLFWEventHandler::onGLFWMouseMoveCallBack);
//glfwSetScrollCallback(wnd, GLFWEventHandler::onGLFWMouseScrollCallback);
//glfwSetCharCallback(wnd, GLFWEventHandler::onGLFWCharCallback);
////glfwSetKeyCallback(wnd, GLFWEventHandler::onGLFWKeyCallback);
//glfwSetWindowPosCallback(wnd, GLFWEventHandler::onGLFWWindowPosCallback);
////glfwSetFramebufferSizeCallback(wnd, GLFWEventHandler::onGLFWframebuffersize);
//glfwSetWindowSizeCallback(wnd, GLFWEventHandler::onGLFWWindowSizeFunCallback);
//glfwSetWindowIconifyCallback(wnd, GLFWEventHandler::onGLFWWindowIconifyCallback);
//glfwSetWindowFocusCallback(wnd, GLFWEventHandler::onGLFWWindowFocusCallback);

//if (!gladLoadGLES2(glfwGetProcAddress))

//GLint numExt = 0;
//glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
//bool supportASTC = GLAD_GL_KHR_texture_compression_astc_hdr && GLAD_GL_KHR_texture_compression_astc_ldr;
//bool supportDXT = GLAD_GL_EXT_texture_compression_s3tc;  // Texture compression: DXT
//bool supportETC2 = GLAD_GL_ARB_ES3_compatibility;        // Texture compression: ETC2/EAC
//std::cout << glGetString(GL_VERSION) << std::endl;



glEnable
glDisable

GL_BLEND
GL_CULL_FACE
GL_DEPTH_TEST
GL_DITHER
GL_POLYGON_OFFSET_FILL
GL_PRIMITIVE_RESTART_FIXED_INDEX
GL_RASTERIZER_DISCARD
GL_SAMPLE_ALPHA_TO_COVERAGE
GL_SAMPLE_COVERAGE
GL_SCISSOR_TEST
GL_STENCIL_TEST

known issue：if enable depth test，will be alpha bug( can be fix by discard )
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_ALWAYS);//glDepthFunc(GL_LEQUAL);
glDepthMask(GL_TRUE);






glMapPersistent optimize glBufferData 

GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
GLbitfield createFlags = mapFlags | GL_MAP_DYNAMIC_STORAGE_BIT;
mDestHead = 0;
mBuffSize = 3 *maxVerts *kVertexSizeBytes;
glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
glBufferStorage(GL_ARRAY_BUFFER，mBuffSize，nullptr，createFlags);
mVertexDataPtr = glMapBufferRange(GL_ARRAY_BUFFER，0, mBuffSize, mapFlags);








https://vulkan-tutorial.com/
https://github.com/SaschaWillems/Vulkan
https://github.com/KhronosGroup/Vulkan-Samples
https://github.com/vulkanprogrammingguide/examples



*/
