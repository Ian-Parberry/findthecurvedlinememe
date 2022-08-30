/// \file CMain.h
/// \brief Interface for the main class CMain.

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

#ifndef __CMAIN_H__
#define __CMAIN_H__

#include <vector>
#include <random>
#include <string>

#include "Includes.h"
#include "WindowsHelpers.h"

/// \brief The main class.
///
/// The interface between I/O from Windows (input from the drop-down menus,
/// output to the client area of the window) and the GDI+ graphics interface.

class CMain{
  public:

    /// \brief Tesselation enumerated type.
    ///
    /// An enumeration type for the tesselation style.

    enum class eTesselation{
      Original, Random
    }; //eTesselation

  private:
    HINSTANCE m_hInstance = nullptr; ///< Instance handle. 
    HWND m_hWnd = nullptr; ///< Window handle.    
    ULONG_PTR m_gdiplusToken = 0; ///< GDI+ token.
    HMENU m_hMenuBar = nullptr; ///< Handle to the menu bar.
    HMENU m_hTessMenu = nullptr; ///< Handle to tesselation menu.

    Gdiplus::Bitmap* m_pBitmap = nullptr; ///< Pointer to a bitmap image.
    std::vector<Gdiplus::Bitmap*> m_stdTile; ///< List of tile images.
    std::default_random_engine* m_pRand = nullptr; ///< Pointer to PRNG.

    void CreateMenus(); ///< Create menus.
    void CreateBitmap(int, int); ///< Create the bitmap.
    void CreateTiles(); ///< Create the tiles.
    void CheckMenuItems(const eTesselation); ///< Check menu items.

    UINT RandomTileIndex(); ///< Get a pseudo-random tile index.

  public:
    CMain(const HINSTANCE, const HWND); ///< Constructor.
    ~CMain(); ///< Destructor.

    void Generate(const eTesselation); ///< Generate tiled image.

    void OnPaint(); ///< Paint the client area of the window.
    Gdiplus::Bitmap* GetBitmap() const; ///< Get pointer to bitmap.
}; //CMain

#endif //__CMAIN_H__