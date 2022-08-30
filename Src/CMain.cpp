/// \file CMain.cpp
/// \brief Code for the main class CMain.

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

#include <chrono>
#include <ctime>

#include "CMain.h"

#include "Includes.h"
#include "resource.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

#pragma region Constructors and destructors

/// Initialize GDI+, create and initialize the menus.
/// \param hInstance Instance handle.
/// \param hwnd Window handle.

CMain::CMain(const HINSTANCE hInstance, const HWND hwnd):
  m_hInstance(hInstance), m_hWnd(hwnd)
{
  m_gdiplusToken = InitGDIPlus(); //initialize GDI+
  CreateMenus(); //create the menu bar
  CreateTiles(); //create the tile set

  m_pRand = new std::default_random_engine;  //PRNG
  const time_t t = static_cast<std::chrono::seconds>(std::time(0)).count(); //time
  m_pRand->seed(static_cast<UINT>(t)); //sow seed from time

  Generate(eTesselation::Original); //generate a tiled bitmap
} //constructor

/// Clean up and shut down GDI+.

CMain::~CMain(){
  delete m_pRand; //delete the PRNG
  delete m_pBitmap; //delete the bitmap

  for(auto p: m_stdTile) //delete the tiles
    delete p;

  Gdiplus::GdiplusShutdown(m_gdiplusToken); //shut down GDI+
} //destructor

#pragma endregion Constructors and destructors

///////////////////////////////////////////////////////////////////////////////
// Drawing functions

#pragma region Drawing functions

/// Draw the bitmap to the window client area, scaled down if necessary. This
/// function should only be called in response to a `WM_PAINT` message.

void CMain::OnPaint(){  
  PAINTSTRUCT ps; //paint structure
  HDC hdc = BeginPaint(m_hWnd, &ps); //device context
  Gdiplus::Graphics graphics(hdc); //GDI+ graphics object
  graphics.Clear(Gdiplus::Color::White); //clear to white

  //get bitmap width and height
  
  const int nBitmapWidth = m_pBitmap->GetWidth(); 
  const int nBitmapHeight = m_pBitmap->GetHeight(); 

  //get client rectangle

  RECT rectClient; //for client rectangle
  GetClientRect(m_hWnd, &rectClient); //get client rectangle
  const int nClientWidth  = rectClient.right - rectClient.left; //client width
  const int nClientHeight = rectClient.bottom - rectClient.top; //client height

  //compute destination rectangle

  const int nDestSide = min(nClientWidth, nClientHeight); //dest width and ht

  const int width  = min(nDestSide, nBitmapWidth); //dest rect width
  const int height = min(nDestSide, nBitmapHeight);  //dest rect height

  const int x = max(0, nClientWidth  - width)/2; //x margin
  const int y = max(0, nClientHeight - height)/2; //y margin

  Gdiplus::Rect rectDest(x, y, width, height); //destination rectangle

  //draw image to destination rectangle then clean up
  
  graphics.DrawImage(m_pBitmap, rectDest);

  EndPaint(m_hWnd, &ps); //this must be done last
} //OnPaint

/// Create bitmap and set all pixels to white.
/// \param w Bitmap width in pixels.
/// \param h Bitmap height in pixels.

void CMain::CreateBitmap(int w, int h){
  delete m_pBitmap; //safety
  m_pBitmap = new Gdiplus::Bitmap(w, h); //create bitmap
} //CreateBitmap

#pragma endregion Drawing functions

///////////////////////////////////////////////////////////////////////////////
// Menu functions

#pragma region Menu functions

/// Add the menus to the menu bar. 

void CMain::CreateMenus(){
  m_hMenuBar = CreateMenu(); //create menu bar

  CreateFileMenu(m_hMenuBar); //attach File menu to menu bar
  m_hTessMenu = CreateTesselationMenu(m_hMenuBar); //attach Tesselation menu to menu bar
  CreateHelpMenu(m_hMenuBar); //attach Help menu to menu bar

  SetMenu(m_hWnd, m_hMenuBar); //attach menu bar to app window
} //CreateMenus

#pragma endregion Menu functions

///////////////////////////////////////////////////////////////////////////////
// Generation functions

#pragma region Generation functions

/// Generate the tile set into `m_stdTile`. The first tile is from an image
/// file and the rest are generated from it using rotations and flips.
/// The image is loaded from the resource `IDB_BITMAP1`.

void CMain::CreateTiles(){  
  Gdiplus::Image* pImage = Gdiplus::Bitmap::FromResource(m_hInstance,
    MAKEINTRESOURCEW(IDB_BITMAP1));

  const UINT w = pImage->GetWidth(); //tile width
  const UINT h = pImage->GetHeight(); //tile height

  for(UINT i=0; i<8; i++){
    Gdiplus::Bitmap* pBitmap = new Gdiplus::Bitmap(w, h);
    Gdiplus::Graphics graphics(pBitmap); //for drawing tiles in bitmap
    graphics.DrawImage(pImage, 0, 0);

    pBitmap->RotateFlip((Gdiplus::RotateFlipType)i); //rotate and flip

    m_stdTile.push_back(pBitmap);
  } //for
} //CreateTiles

/// Get a pseudo-random tile index. Assumes that `m_pRand` points to a
/// pseudo-random number generator that has been seeded.
/// \return A pseudo-random tile index.

UINT CMain::RandomTileIndex(){
  const UINT nMax = (UINT)m_stdTile.size() - 1; //maximum tile index
  return std::uniform_int_distribution<UINT>(0, nMax)(*m_pRand);
} //RandomTileIndex

/// Set the check marks for the current tesselation style.
/// \param tesselation Tesselation style.

void CMain::CheckMenuItems(const eTesselation tesselation){
  switch(tesselation){
    case eTesselation::Original:
      CheckMenuItem(m_hTessMenu, IDM_TESS_ORIGINAL, MF_CHECKED);
      CheckMenuItem(m_hTessMenu, IDM_TESS_RANDOM,   MF_UNCHECKED);
      break;

    case eTesselation::Random:
      CheckMenuItem(m_hTessMenu, IDM_TESS_ORIGINAL, MF_UNCHECKED);
      CheckMenuItem(m_hTessMenu, IDM_TESS_RANDOM,   MF_CHECKED);
      break;
  } //switch
} //CheckMenuItems

/// Generate a tiling into the bitmap pointed to by `m_pBitmap` using the
/// tile set in `m_stdTile`. The original tesselation is hard-coded, and
/// a random tesselation is obtained by calling `CMain::RandomTileIndex()`
/// for each tile.
/// \param tesselation Tesselation style.

void CMain::Generate(const eTesselation tesselation){  
  static const UINT nOriginal[8][8]{ //tile indices in original image
    {0, 1, 5, 4, 5, 6, 0, 3},
    {1, 2, 6, 5, 6, 7, 3, 2},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 2, 3, 0, 3, 0, 3, 0},
    {5, 6, 0, 3, 0, 1, 5, 4},
    {6, 7, 3, 2, 1, 2, 6, 5},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {3, 0, 3, 0, 1, 2, 3, 0}
  }; //nOriginal

  CheckMenuItems(tesselation);

  //generate tiled image

  const UINT w = m_stdTile[0]->GetWidth(); //tile width
  const UINT h = m_stdTile[0]->GetHeight(); //tile height

  const UINT nSize = 8; //number of tiles per side of bitmap

  CreateBitmap(w*nSize, h*nSize); //create bitmap
  Gdiplus::Graphics graphics(m_pBitmap); //for drawing tiles in bitmap
  graphics.Clear(Gdiplus::Color(143, 158, 104)); //fill with green

  int y = 0; //vertical tile coordinate

  for(UINT i=0; i<nSize; i++){ //vertical tile index
    int x = 0; //horizontal tile coordinate

    for(UINT j=0; j<nSize; j++){ //horizontal tile index
      UINT nTileIndex = 0; //tile index for current tile
      
      switch(tesselation){   
        case eTesselation::Original: nTileIndex = nOriginal[i][j]; break;
        case eTesselation::Random:   nTileIndex = RandomTileIndex(); break;
      } //switch
      
      graphics.DrawImage(m_stdTile[nTileIndex], x, y); //draw tile
      x += w; //move right to next tile
    } //for

    y += h; //move down to next tile
  } //for
} //Generate

#pragma endregion Generation functions

///////////////////////////////////////////////////////////////////////////////
// Reader functions

#pragma region Reader functions

/// Reader function for the bitmap pointer `m_pBitmap`.
/// \return The bitmap pointer `m_pBitmap`.

Gdiplus::Bitmap* CMain::GetBitmap() const{
  return m_pBitmap;
} //GetBitmap

#pragma endregion Reader functions