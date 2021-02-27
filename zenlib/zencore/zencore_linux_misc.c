// NOTE(Abi): Modified GUI Program Skeleton from Handmade Network
// TODO(Abi): Add a button to close, make it look nicer
// TODO(Abi): Maybe just use the inbuilt renderer, also fix the windowing issues,
// in fact, once i have a dev terminal i won't need this bc it will either dump to inbuilt console or linux one
internal void
LinuxMessageBox(const char * Title, const char * Message) {
    // Create connection to the X Server.
    Display* dpy = XDisplay;//XOpenDisplay(NULL);
    
    
#define MSG_BOX_MAX_LINES 128
#define MAX_CHARACTERS_PER_LINE 64
    char * Text[MSG_BOX_MAX_LINES] = {0};
    i32 Line = 0;
    // NOTE(Abi): string preprocess, don't need to free strings as program immediately exits
    // TODO(Abi): may work badly with newlines
    for(i32 idx = 0; (idx < StringLength(Message)) && (Line < MSG_BOX_MAX_LINES); ++Line) {
        if(idx + MAX_CHARACTERS_PER_LINE > StringLength(Message)) {
            Text[Line] = strndup(Message + idx, StringLength(Message) - idx);
            break;
        }
        i32 End = idx + MAX_CHARACTERS_PER_LINE;
        while(Message[End] != ' ') { End--; };
        Text[Line] = strndup(Message + idx, (End - idx));
        idx = End + 1;
    }
    
    Window   win = XCreateSimpleWindow(dpy,
                                       DefaultRootWindow(dpy),
                                       0, 0,                  
                                       435, 50 + Line * 16,              
                                       0, 0,                  
                                       WhitePixel(dpy, 0));
    
    XStoreName(dpy, win, Title);
    
    XSelectInput(dpy, win,
                 SubstructureNotifyMask // Window size changes (ConfigureNotify)
                 | ExposureMask         // Window needs to be repainted (Expose)
                 | PointerMotionMask    // Mouse moves (MotionNotify)
                 | ButtonPressMask
                 | ButtonReleaseMask
                 | KeyPressMask);
    
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
            for(i32 i = 0; i <= Line; ++i) {
                XDrawString(dpy, win, DefaultGC(dpy, 0), 30, 30 + (i * 16), Text[i], strlen(Text[i]));
            }
        }
    }
    
    XUnmapWindow(dpy, win);
    XDestroyWindow(dpy, win);
}


internal void
LinuxError(const char * Title, const char * Message) {
    // TODO(Abi): Not completely happy with this being commented out
    //fprintf(stderr, "%s: %s\n", Title, Message);
    LinuxMessageBox(Title, Message);
}

internal void *
LinuxHeapAlloc(u32 SizeInBytes) {
    void * Memory = malloc(SizeInBytes);
    Assert(Memory != 0);
    return Memory;
}

internal void
LinuxHeapFree(void * Memory) {
    //b8 Success = munmap(Memory, SizeInBytes);
    //Assert((Success == 0) && "Freed memory successfully");
    free(Memory);
}