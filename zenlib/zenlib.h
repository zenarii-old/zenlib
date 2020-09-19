#ifndef ZENLIB_H
#define ZENLIB_H

#include "zencore/zencore.h"

#ifdef ZEN2D
#include "zen2d/zen2d.h"
#endif

#ifndef USE_OPENGL
#define USE_OPENGL 1
#endif

internal void AppInit(void);
internal void AppUpdate(void);
internal void AppHotLoad(void);
internal void AppHotUnload(void);

#endif //ZENLIB_H
