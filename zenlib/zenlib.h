#ifndef ZENLIB_H
#define ZENLIB_H

#include "zencore/zencore.h"

#ifdef ZEN2D
#include "zen2d/zen2d.h"
#endif

#ifdef ZEN3D
#include "zen3d/zen3d.h"
#endif

internal void AppInit(void);
internal void AppUpdate(void);
internal void AppHotLoad(void);
internal void AppHotUnload(void);

#endif //ZENLIB_H
