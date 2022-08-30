/// \file WindowsHelpers.h
/// \brief Interface for some helpful Windows-specific functions.
///
/// These platform-dependent functions are hidden away so that the faint-of-heart
/// don't have to see them if they're offended by them. 

// MIT License
//
// Copyright (c) 2022 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifndef __WINDOWSHELPERS_H__
#define __WINDOWSHELPERS_H__

#include "Includes.h"

///////////////////////////////////////////////////////////////////////////////
// Menu IDs

#pragma region Menu IDs

#define IDM_FILE_SAVE 1 ///< Menu id for Save.
#define IDM_FILE_QUIT 2 ///< Menu id for Quit.

#define IDM_TESS_ORIGINAL 3 ///< Menu id for origonal tesselation.
#define IDM_TESS_RANDOM 4 ///< Menu id for random tesselation.

#define IDM_HELP_HELP  5 ///< Menu id for display help.
#define IDM_HELP_ABOUT 6 ///< Menu id for display About info.

#pragma endregion Menu IDs

///////////////////////////////////////////////////////////////////////////////
// Helper functions

#pragma region Helper functions

void InitWindow(HINSTANCE, INT, WNDPROC); ///< Initialize window.
ULONG_PTR InitGDIPlus(); ///< Initialize GDI+.

HRESULT SaveBitmap(HWND, const std::wstring&, Gdiplus::Bitmap*);

#pragma endregion Helper functions

///////////////////////////////////////////////////////////////////////////////
// Menu functions

#pragma region Menu functions

void CreateFileMenu(HMENU hParent); ///< Create `File` menu.
HMENU CreateTesselationMenu(HMENU hParent); ///< Create `Tesselation` menu.
void CreateHelpMenu(HMENU hParent); ///< Create `Help` menu.

#pragma endregion Menu functions

#endif //__WINDOWSHELPERS_H__
