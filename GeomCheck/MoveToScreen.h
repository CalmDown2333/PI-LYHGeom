#ifndef MOVE_TO_SCREEN_H
#define MOVE_TO_SCREEN_H

#include <iostream>
#include <afxwin.h>
#include "ProWindows.h"
#include "ProMdl.h"
#include "ProObjects.h"

//#define _WIN32_WINNT 0x0401

void UserSolidToScreen(ProSolid solid,ProPoint3d solid_point,ProPoint3d screen_point);

void MoveToScreenCenter(ProMdl p_mdl, ProPoint3d sp);

#endif