// NOTE(Abi): Modified GUI Program Skeleton from Handmade Network
// TODO(Abi): Add a button to close, make it look nicer
internal void
LinuxMessageBox(const char * Title, const char * Message) {
    // Create connection to the X Server.
    Display* dpy = XOpenDisplay(NULL);
    
    // Create window.
    Window   win = XCreateSimpleWindow(
                                       dpy,
                                       DefaultRootWindow(dpy), // Parent
                                       0, 0,                   // X/Y, often overridden by window manager
                                       400, 100,               // Width/Height
                                       0, 0,                   // Border
                                       WhitePixel(dpy, 0)      // Background
                                       );
    
    // Set window title.
    XStoreName(dpy, win, Title);
    
    // Choose which events we want.
    XSelectInput(dpy, win,
                 SubstructureNotifyMask // Window size changes (ConfigureNotify)
                 | ExposureMask         // Window needs to be repainted (Expose)
                 | PointerMotionMask    // Mouse moves (MotionNotify)
                 | ButtonPressMask
                 | ButtonReleaseMask
                 | KeyPressMask);
    
    // Make window visible and give it focus.
    XMapRaised(dpy, win);
    
    // For window manager close button event.
    Atom win_closed_id = XInternAtom(dpy, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(dpy, win, &win_closed_id, 1);
    
    for (;;) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        
        // WM close button pushed, exit.
        if (ev.type == ClientMessage && ev.xclient.data.l[0] == win_closed_id) {
            break;
        }
        
        // Expose event tells us to repaint (some of) the window.
        if (ev.type == Expose) {
            XDrawString(dpy, win, DefaultGC(dpy, 0), 30, 30, Message, strlen(Message));
        }
    }
    XDestroyWindow(dpy, win);
}


internal void
LinuxError(const char * Title, const char * Message, ...) {
    va_list args;
    va_start(args, Message);
    fprintf(stderr, "%s: ", Title);
    vfprintf(stderr, Message, args);
    
    local char Buffer[1024] = {0};
    
    vsprintf(Buffer, Message, args);
    LinuxMessageBox(Title, Message);
    va_end(args);
}