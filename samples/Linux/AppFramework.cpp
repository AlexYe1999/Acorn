#include"AppFramework.hpp"

int AppFramework::Run(IApplication* pApp, int argc, char* argv[]){
    xcb_connection_t        *pConnect;
    xcb_screen_t            *pScreen;
    xcb_window_t            window;
    xcb_gcontext_t          foreground;
    xcb_gcontext_t          background;
    xcb_generic_event_t     *pEvent;
    xcb_colormap_t          colormap;
    uint32_t                mask = 0;
    uint32_t                values[3];
    bool                    isQuit = false;

    /* establish connection to X server */
    pConnect = xcb_connect(nullptr, nullptr);

    /* get the first screen and root window*/
    pScreen = xcb_setup_roots_iterator(xcb_get_setup(pConnect)).data;
    window = pScreen->root;

    /* create black (foreground) graphic context */
    foreground = xcb_generate_id(pConnect);
    mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    values[0] = pScreen->black_pixel;
    values[1] = 0;
    xcb_create_gc(pConnect, foreground, window, mask, values);

    /* create white (background) graphic context */
    background = xcb_generate_id(pConnect);
    mask = XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    values[0] = pScreen->white_pixel;
    values[1] = 0;
    xcb_create_gc(pConnect, background, window, mask, values);

    /* create window */
    window = xcb_generate_id(pConnect);
    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = pScreen->white_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;

    xcb_create_window(  
        pConnect,                               /* connection */
        XCB_COPY_FROM_PARENT,                   /* depth */
        window,                                 /* window ID */
        pScreen->root,                          /* parent window */
        100, 20,                                /* x, y */
        pApp->GetWidth(), pApp->GetHeight(),    /* width, height */
        10,                                     /* boarder width */
        XCB_WINDOW_CLASS_INPUT_OUTPUT,          /* class */
        pScreen->root_visual,                   /* visual */
        mask, values                            /* masks */
    );                         

    /* set the title of the window */
    xcb_change_property(
        pConnect, XCB_PROP_MODE_REPLACE, window,
        XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
        strlen(pApp->GetTitle().c_str()), pApp->GetTitle().c_str()
    );

    /* set the title of the window icon */
    xcb_change_property(
        pConnect, XCB_PROP_MODE_REPLACE, window,
        XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8,
        strlen(pApp->GetTitle().c_str()), pApp->GetTitle().c_str()
    );      

    /* map the window on the screen */
    xcb_map_window(pConnect, window);

    xcb_flush(pConnect);

    pApp->OnInit();

    while((pEvent = xcb_wait_for_event(pConnect)) && !isQuit) {
        switch(pEvent->response_type & ~0x80) {
            case XCB_EXPOSE:
                static xcb_point_t line[2] = {
                    {20, 20}, {50, 50}
                };

                xcb_poly_line(
                    pConnect,                   /* The connection to the X server */
                    XCB_COORD_MODE_ORIGIN,      /* Coordinate mode, usually set to XCB_COORD_MODE_ORIGIN */
                    window,                     /* The drawable on which we want to draw the line(s) */
                    foreground,                 /* The Graphic Context we use to draw the line(s) */
                    2,                          /* The number of points in the polygonal line */
                    line                        /* An array of points */
                );   

                xcb_flush(pConnect);
                break;
            case XCB_KEY_PRESS:
                isQuit = true;
                break;
        }
        free(pEvent);
    }

    return 0;
}
