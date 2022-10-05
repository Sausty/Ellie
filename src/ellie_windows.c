#include "ellie_common.h"
#include "ellie_platform.h"
#include "ellie.h"

#include <stdio.h>

#include "glad/glad.h"

#ifdef ELLIE_WINDOWS

typedef DWORD (*PFN_CHOOSE_PIXEL_FORMAT)(HDC dc, PIXELFORMATDESCRIPTOR* pfd);
typedef void (*PFN_SET_PIXEL_FORMAT)(HDC hdc, DWORD pixel_format, PIXELFORMATDESCRIPTOR* pfd);
typedef void (*PFN_SWAP_BUFFERS)(HDC hdc);
PFN_CHOOSE_PIXEL_FORMAT ChoosePixelFormatStub;
PFN_SET_PIXEL_FORMAT SetPixelFormatStub;
PFN_SWAP_BUFFERS SwapBuffersStub;

typedef HGLRC (*PFN_WGL_CREATE_CONTEXT)(HDC hdc);
typedef void (*PFN_WGL_MAKE_CURRENT)(HDC hdc, HGLRC hglrc);
typedef void (*PFN_WGL_DELETE_CONTEXT)(HGLRC hglrc);
PFN_WGL_CREATE_CONTEXT wglCreateContextStub;
PFN_WGL_MAKE_CURRENT wglMakeCurrentStub;
PFN_WGL_DELETE_CONTEXT wglDeleteContextStub;

typedef struct windows_state {
    dynamic_library GDI;
    dynamic_library OpenGL;
    
    u32 Width;
    u32 Height;

    u32 MouseX;
    u32 MouseY;

    b8 MouseButtons[MouseButton_Max];
} windows_state;

internal windows_state State;

void OpenDynamicLib(dynamic_library* Library, const char* Path)
{
    Library->Handle = LoadLibraryA(Path);
}

void CloseDynamicLib(dynamic_library* Library)
{
    FreeLibrary(Library->Handle);
}

void* LoadFunction(dynamic_library* Library, const char* Name)
{
    return GetProcAddress(Library->Handle, Name);
}

b8 IsMouseButtonPressed(mouse_button Button)
{
    return State.MouseButtons[Button];
}

hmm_vec2 GetMousePosition()
{
    return HMM_Vec2(State.MouseX, State.MouseY);
}

char* PlatformReadFile(const char* Path, i32* OutputSize)
{
    FILE* File = fopen(Path, "r");
    if (!File)
    {
        printf("Failed to open file: %s\n", Path);
        fclose(File);
        return NULL;
    }

    long CurrentPos = ftell(File);
    fseek(File, 0, SEEK_END);
    long Size = ftell(File);
    fseek(File, CurrentPos, SEEK_SET);

    char* Buffer = malloc(Size + 1);
    fread(Buffer, Size, sizeof(char), File);
    Buffer[Size] = '\0';
    *OutputSize = Size;
    return Buffer;
}

void LoadLibs()
{
    OpenDynamicLib(&State.GDI, "gdi32.dll");
    OpenDynamicLib(&State.OpenGL, "opengl32.dll");

    ChoosePixelFormatStub = LoadFunction(&State.GDI, "ChoosePixelFormat");
    SetPixelFormatStub = LoadFunction(&State.GDI, "SetPixelFormat");
    SwapBuffersStub = LoadFunction(&State.GDI, "SwapBuffers");

    wglCreateContextStub = LoadFunction(&State.OpenGL, "wglCreateContext");
    wglMakeCurrentStub = LoadFunction(&State.OpenGL, "wglMakeCurrent");
    wglDeleteContextStub = LoadFunction(&State.OpenGL, "wglDeleteContext");
}

LRESULT CALLBACK EventCallback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int main()
{
    LoadLibs();
    if (!gladLoadGL())
        return -1;

    HICON icon = LoadIcon(GetModuleHandle(NULL), IDI_APPLICATION);
    
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = EventCallback;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "WindowClass";

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
    }

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;
    
    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;
    
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    State.Width += border_rect.right - border_rect.left;
    State.Height += border_rect.bottom - border_rect.top;

    HWND window = CreateWindowExA(window_ex_style, "WindowClass", "Ellie", window_style, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, wc.hInstance, 0);
    if (!window) {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }
    HDC dc = GetDC(window);

    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;  
    
    i32 pixel_format = ChoosePixelFormatStub(dc, &pfd);
    if (pixel_format == 0)
        MessageBoxA(NULL, "Failed to choose window pixel format!", "Error", MB_ICONEXCLAMATION | MB_OK);
    
    SetPixelFormatStub(dc, pixel_format, &pfd);
    
    HGLRC hglrc = wglCreateContextStub(dc);
    wglMakeCurrentStub(dc, hglrc);

    b8 should_activate = true;  
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(window, show_window_command_flags);

    b8 Quit = false;
    while (!Quit)
    {
        MSG msg;
        while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            switch (msg.message)
            {
                case WM_CLOSE:
                {
                    Quit = true;
                } break;
            }
        }

        GameUpdate(0.0f);
        SwapBuffersStub(dc);
    }

    GameExit();
    wglDeleteContextStub(hglrc);
    ReleaseDC(window, dc);
    CloseDynamicLib(&State.OpenGL);
    CloseDynamicLib(&State.GDI);
    return 0;
}

#endif