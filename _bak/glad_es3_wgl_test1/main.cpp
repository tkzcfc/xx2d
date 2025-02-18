﻿#include "pch.h"
#include "logic.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static const TCHAR window_classname[] = _T("GLAD_ES3_TEST1");
static const TCHAR window_title[] = _T("glad without glfw opengl es3.0 test1");
static const POINT window_location = { CW_USEDEFAULT, 0 };

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create logic code
    auto logic = xx::Make<Logic>();

    // create window
    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = window_classname;

    ATOM wndclass = RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(MAKEINTATOM(wndclass), window_title,
        WS_OVERLAPPEDWINDOW,
        window_location.x, window_location.y,
        logic->w, logic->h,
        NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, _T("Failed to create window!"), window_title, MB_ICONERROR);
        return -1;
    }

    // Get a device context so I can set the pixel format later:
    HDC hdc = GetDC(hWnd);
    if (!hdc) {
        DestroyWindow(hWnd);
        MessageBox(NULL, _T("Failed to get Window's device context!"), window_title, MB_ICONERROR);
        return -1;
    }

    // Set the pixel format for the device context:
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int format = ChoosePixelFormat(hdc, &pfd);
    if (format == 0 || SetPixelFormat(hdc, format, &pfd) == FALSE) {
        ReleaseDC(hWnd, hdc);
        DestroyWindow(hWnd);
        MessageBox(NULL, _T("Failed to set a compatible pixel format!"), window_title, MB_ICONERROR);
        return -1;
    }

    // Create and enable a temporary (helper) opengl context:
    HGLRC temp_context = NULL;
    if (NULL == (temp_context = wglCreateContext(hdc))) {
        ReleaseDC(hWnd, hdc);
        DestroyWindow(hWnd);
        MessageBox(NULL, _T("Failed to create the initial rendering context!"), window_title, MB_ICONERROR);
        return -1;
    }
    wglMakeCurrent(hdc, temp_context);

    // Load WGL Extensions:
    gladLoaderLoadWGL(hdc);

    // Set the desired OpenGL version:
    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,   // Set the MAJOR version of OpenGL to 3
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,   // Set the MINOR version of OpenGL to 3
        WGL_CONTEXT_FLAGS_ARB,
        WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
        0
    };

    // Create the final opengl context and get rid of the temporary one:
    HGLRC opengl_context = NULL;
    if (NULL == (opengl_context = wglCreateContextAttribsARB(hdc, NULL, attributes))) {
        wglDeleteContext(temp_context);
        ReleaseDC(hWnd, hdc);
        DestroyWindow(hWnd);
        MessageBox(NULL, _T("Failed to create the final rendering context!"), window_title, MB_ICONERROR);
        return -1;
    }
    wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
    wglDeleteContext(temp_context); // Delete the temporary OpenGL context
    wglMakeCurrent(hdc, opengl_context);    // Make our OpenGL 3.2 context current

    // Glad Loader!
    if (!gladLoaderLoadGL()) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(opengl_context);
        ReleaseDC(hWnd, hdc);
        DestroyWindow(hWnd);
        MessageBox(NULL, _T("Glad Loader failed!"), window_title, MB_ICONERROR);
        return -1;
    }
    // Show & Update the main window:
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Close vsync
    wglSwapIntervalEXT(0);

    // A typical native Windows game loop:
    logic->Init();
    CheckGLError();
    MSG msg = { };
    while (true) {
        while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT || (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)) goto LabQuit;
        }
        CheckGLError();
        logic->Update();
        CheckGLError();
        SwapBuffers(hdc);
    }
LabQuit:
    logic.Reset();

    // Clean-up:
    if (opengl_context)
        wglDeleteContext(opengl_context);
    if (hdc)
        ReleaseDC(hWnd, hdc);
    if (hWnd)
        DestroyWindow(hWnd);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_QUIT:
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
