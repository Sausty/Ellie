#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>  // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>  // sudo apt-get install libxkbcommon-x11-dev libx11-xcb-dev
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#include <GL/glx.h>
#include "gl_loader.h"

#include <stdio.h>

#include "common.h"
#include "game.h"
#include "platform.h"

typedef struct x11_state {
    Display* Display;
    xcb_connection_t* Connection;
    xcb_window_t Window;
    xcb_screen_t* Screen;
    xcb_atom_t Protocols;
    xcb_atom_t Delete;
    GLXContext Context;

    u32 Width;
    u32 Height;
} x11_state;

internal x11_state State;

char* ReadFile(const char* Path, i32* OutputSize)
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

int main()
{
    // Load GL
    LoadGL();
    
    // Open the display
    State.Display = XOpenDisplay(NULL);
    if (!State.Display)
    {
        printf("Failed to open display!");
        return -1;
    }
    
    //Turn on auto repeat
    XAutoRepeatOn(State.Display);
    // Get connection
    State.Connection = XGetXCBConnection(State.Display);

    if (xcb_connection_has_error(State.Connection))
    {
        XCloseDisplay(State.Display);
        printf("Failed to connect to X server via XCB");
        return -1;
    }

    // Get setup
    const struct xcb_setup_t* Setup = xcb_get_setup(State.Connection);
    
    // Find XCB screen
    xcb_screen_iterator_t Iterator = xcb_setup_roots_iterator(Setup);
    i32 Screen = 0;
    for (i32 ScreenNumber = Screen; ScreenNumber > 0; ScreenNumber--)
    {
        xcb_screen_next(&Iterator);
    }
    State.Screen = Iterator.data;

    // Generate OpenGL context
    i32 Attribs[] =
    {
        GLX_X_RENDERABLE, true,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        GLX_DOUBLEBUFFER, true,
        0
    };

    GLXFBConfig* FramebufferConfig = NULL;
    i32 NumConfigs = 0;
    FramebufferConfig = glXChooseFBConfig(State.Display, Screen, Attribs, &NumConfigs);
    if (!FramebufferConfig || NumConfigs == 0)
    {
        printf("Failed to get GLX config!");
        XAutoRepeatOn(State.Display);
        XCloseDisplay(State.Display);
        return -1;
    }

    i32 VisualID = 0;
    GLXFBConfig Config = FramebufferConfig[0];
    glXGetFBConfigAttrib(State.Display, Config, GLX_VISUAL_ID, &VisualID);

    State.Context = glXCreateNewContext(State.Display, Config, GLX_RGBA_TYPE, 0, true);
    if (!State.Context)
    {
        printf("Failed to create GLX context!");
        XAutoRepeatOn(State.Display);
        XCloseDisplay(State.Display);
        return -1;
    }

    xcb_colormap_t Colormap = xcb_generate_id(State.Connection);
    xcb_create_colormap(State.Connection, XCB_COLORMAP_ALLOC_NONE, Colormap, State.Screen->root, VisualID);
    
    // Generate window
    State.Window = xcb_generate_id(State.Connection);

    // Register event types
    u32 EventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    // Listener events
    u32 EventListeners = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                       XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                       XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    // Values that will be sent to the xcb connection
    u32 ValueList[] = { State.Screen->black_pixel, EventListeners };
    
    // Create the window
    xcb_create_window(State.Connection, XCB_COPY_FROM_PARENT, State.Window, State.Screen->root, 0, 0, 1280, 720, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, VisualID, EventMask, ValueList);
    GLXWindow GLWindow = glXCreateWindow(State.Display, Config, State.Window, 0);
    if (!GLWindow)
    {
        printf("Failed to create GLX window!\n");
        glXDestroyContext(State.Display, State.Context);
        XCloseDisplay(State.Display);
        return -1;
    }

    if (!glXMakeContextCurrent(State.Display, (GLXDrawable)GLWindow, (GLXDrawable)GLWindow, State.Context))
    {
        xcb_destroy_window(State.Connection, State.Window);
        glXDestroyContext(State.Display, State.Context);
        XCloseDisplay(State.Display);
        return -1;
    }
    
    // Change the title
    xcb_change_property(State.Connection, XCB_PROP_MODE_REPLACE, State.Window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, 5, "Ellie");

    // Tell the server to notify when the window manager attempts to destroy a window
    xcb_intern_atom_cookie_t DeleteCookie = xcb_intern_atom(State.Connection, 0, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");
    xcb_intern_atom_cookie_t ProtocolsCookie = xcb_intern_atom(State.Connection, 0, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* DeleteReply = xcb_intern_atom_reply(State.Connection, DeleteCookie, NULL);
    xcb_intern_atom_reply_t* ProtocolsReply = xcb_intern_atom_reply(State.Connection, ProtocolsCookie, NULL);
    State.Protocols = ProtocolsReply->atom;
    State.Delete = DeleteReply->atom;

    xcb_change_property(State.Connection, XCB_PROP_MODE_REPLACE, State.Window, ProtocolsReply->atom, 4, 32, 1, &DeleteReply->atom);

    // Map the window
    xcb_map_window(State.Connection, State.Window);
    
    // Flush the stream
    i32 StreamResult = xcb_flush(State.Connection);
    if (StreamResult <= 0)
    {
        printf("There was en error when flushing the stream: %d", StreamResult);
        XAutoRepeatOn(State.Display);
        XCloseDisplay(State.Display);
    }
    
    // Init the game
    GameInit();

    // Update
    xcb_generic_event_t* Event;
    xcb_client_message_event_t* CustomClientMessage;
    b8 Quit = false;
    while (!Quit)
    {
        while ((Event = xcb_poll_for_event(State.Connection)))
        {
            switch (Event->response_type & ~0x80)
            {
            case XCB_CLIENT_MESSAGE:
            {
                CustomClientMessage = (xcb_client_message_event_t*)Event;

                // Window close
                if (CustomClientMessage->data.data32[0] == State.Delete)
                {
                    Quit = true;
                }
            } break;
            case XCB_CONFIGURE_NOTIFY:
            {
                xcb_configure_notify_event_t* ConfigureEvent = (xcb_configure_notify_event_t*)Event;

                State.Width = ConfigureEvent->width;
                State.Height = ConfigureEvent->height;
                GameResize(State.Width, State.Height);
            } break;
            default:
            {
                break;
            }
            }
        }

        gl.Viewport(0, 0, State.Width, State.Height);
        // Update the game
        GameUpdate(0.0f);
        
        glXSwapBuffers(State.Display, (GLXDrawable)GLWindow);
    }
    
    // Exit the game
    GameExit();

    // Cleanup
    glXDestroyWindow(State.Display, GLWindow);
    glXDestroyContext(State.Display, State.Context);
    xcb_destroy_window(State.Connection, State.Window);
    XCloseDisplay(State.Display);
    FreeGL();
    printf("Successfully exited the program.\n");
    
    return 0;
}
