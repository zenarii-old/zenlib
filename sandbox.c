// NOTE(Abi): Headers
#include "zenlib/zenlib.h"

// NOTE(Abi): Implementations
#include "zenlib/zenlib.c"

struct core {
    i32 Pad;
    //font Font;
};

internal void
AppInit() {
    fprintf(stderr, "[App] Loaded\n");
    
    Platform->Core = MemoryArenaAlloc(&Platform->PermenantArena, sizeof(core));
}

internal void
AppUpdate() {
#ifdef ZEN3D
    // TODO(Abi): figure out why i can't just set this once, is it bc i ditch the shsader?
    matrix4x4 I = ScaleMatrix(1.f, 1.f, 1.f);
    GLuint ProjLoc = glGetUniformLocation(Zen3D->Shaders[0], "Projection");
    glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, &I.Elements[0][0]);
    matrix4x4 T = TranslationMatrix(0.25, -0.25, 0);
    GLuint ViewLoc = glGetUniformLocation(Zen3D->Shaders[0], "View");
    glUniformMatrix4fv(ViewLoc, 1, GL_TRUE, &T.Elements[0][0]);
    PrintMatrix4x4(MultM4M4(T, I));
    
    {
        v3 p0, p1, p2, p3;
        p0 = v3(-.5f,  .5f, 0);
        p1 = v3(.5f,   .5f, 0);
        p2 = v3(.5f,  -.5f, 0);
        p3 = v3(-.5f, -.5f, 0);
        v4 Colour = v4(1.f, 0.f, 0.f, 1.f);
        Zen3DPushQuad(p0, p1, p2, p3, Colour);
    }
    {
        v3 p0, p1, p2, p3;
        p0 = v3(0.f, 1.f, -0.5);
        p1 = v3(1.f, 1.f, -0.5);
        p2 = v3(1.f, 0.f, -0.5);
        p3 = v3(0.f, 0.f, -0.5);
        v4 Colour = v4(1.f, 1.f, 0.f, 1.f);
        Zen3DPushQuad(p0, p1, p2, p3, Colour);
    }
#endif
#ifdef ZEN2D
    Zen2DPushRect(v4(100, 100, 100, 100), v4(1.f, 0.f, 0.f, 1.f));
#endif
    if(ZenKeyDown(ZKEY_ESCAPE)) ZenPlatformQuit();
}

internal void
AppHotLoad() {
    fprintf(stderr, "[App] Hot Loaded\n");
}

internal void
AppHotUnload() {
    fprintf(stderr, "[App] Hot Unloaded\n");
}