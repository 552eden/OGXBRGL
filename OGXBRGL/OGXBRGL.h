#pragma once

#ifndef SUNRISE2_H
#define SUNRISE2_H
#include "stdafx.h"
DWORD HvxSetState(DWORD mode);

DWORD HvxGetVersions(DWORD key, __int64 type);

extern BOOL bIsDevkit;
extern BOOL bAllowRetailPlayers;
extern BOOL bIgnoreTrueskill;

#endif