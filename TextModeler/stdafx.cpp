
// stdafx.cpp : source file that includes just the standard includes
// TextAnalyzer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


HRESULT gl_GetBitFont(CDC *cDCPtr, LOGFONT *LFont, CString strContent, BITMAP *bitmap, BITMAPINFO *m_pBmpInfo)
{
	CBitmap MemCBitmap,* OldMapPtr;
	CDC		MemCDC;

	CRect drawRect(0, 0, 0, 0);

	// -------------- calc drawRect -----------------------
	CFont font;
	//	VERIFY(font.CreateFontIndirect(LFont));
	font.CreateFontIndirect(LFont);
	CFont* def_font = cDCPtr->SelectObject(&font);

	cDCPtr->SetBkMode(OPAQUE);
	cDCPtr->DrawText(strContent, drawRect, DT_LEFT | DT_NOPREFIX | DT_CALCRECT);
	if( LFont->lfItalic )
		drawRect.right += (6*GetDeviceCaps(cDCPtr->m_hDC, LOGPIXELSY))/72;

	cDCPtr->SelectObject(def_font);
	font.DeleteObject();

	//--------------- create bitmap ---------------------------
	MemCBitmap.CreateCompatibleBitmap(cDCPtr, drawRect.Width(), drawRect.Height());
	MemCDC.CreateCompatibleDC(NULL);

	OldMapPtr = MemCDC.SelectObject(&MemCBitmap);

	// ---------- draw ---------------------
	font.CreateFontIndirect(LFont);
	def_font = MemCDC.SelectObject(&font);
	MemCDC.FillSolidRect(drawRect, RGB(0,0,0));
	MemCDC.SetTextColor(RGB(255,255,255));
	MemCDC.DrawText(strContent, drawRect, DT_LEFT | DT_NOPREFIX);

	//---------------------- Get BitMap ------------------------------
	void* pre_ptr;
	pre_ptr = bitmap->bmBits;
	//	int pre_size = bitmap.bmHeight*bitmap.bmWidthBytes;

	MemCBitmap.GetBitmap(bitmap);
	bitmap->bmBitsPixel = 1;  // 1 bit

	div_t over = div((int)(bitmap->bmWidth*bitmap->bmBitsPixel), 8);
	bitmap->bmWidthBytes = over.quot;
	if( over.rem ) bitmap->bmWidthBytes++;
	bitmap->bmWidthBytes = ((bitmap->bmWidthBytes + 3)&(~3));

	//	bitmap.bmWidthBytes = (bitmap.bmWidth*bitmap.bmBitsPixel)/8;
	//	if( (bitmap.bmWidth*bitmap.bmBitsPixel)%8 ) bitmap.bmWidthBytes++;
	//	bitmap.bmWidthBytes = ((bitmap.bmWidthBytes + 3)&(~3));

	//	bitmap.bmWidthBytes = ((bitmap.bmWidth*bitmap.bmBitsPixel/8 + 3)&(~3));	// 1 byte

	if( pre_ptr )
	{
		bitmap->bmBits = realloc(pre_ptr, bitmap->bmHeight * bitmap->bmWidthBytes);
		//bitmap.bmBits = LocalReAlloc((HLOCAL)pre_ptr, bitmap.bmHeight*bitmap.bmWidthBytes, LMEM_MOVEABLE);
	}
	else
	{
		bitmap->bmBits = malloc(bitmap->bmHeight * bitmap->bmWidthBytes);
		//bitmap.bmBits = LocalAlloc(LPTR, bitmap.bmHeight*bitmap.bmWidthBytes);
		//bitmap.bmBits = (unsigned char*)HeapAlloc(GetProcessHeap(), 0, bitmap.bmHeight*bitmap.bmWidthBytes);
	}

	memset(m_pBmpInfo, 0, sizeof(BITMAPINFOHEADER));
	BITMAPINFOHEADER* bmHedPtr = (BITMAPINFOHEADER*)m_pBmpInfo;

	bmHedPtr->biSize = sizeof(BITMAPINFOHEADER);
	bmHedPtr->biWidth = bitmap->bmWidth;
	bmHedPtr->biHeight = bitmap->bmHeight;
	bmHedPtr->biPlanes = bitmap->bmPlanes;
	bmHedPtr->biBitCount = bitmap->bmBitsPixel;

	int iR = GetDIBits( MemCDC.m_hDC, 
		(HBITMAP)MemCBitmap.GetSafeHandle(),	// source CBitmap
		0L,
		bitmap->bmHeight,
		(LPBYTE)bitmap->bmBits,
		(LPBITMAPINFO)m_pBmpInfo,
		(DWORD)DIB_RGB_COLORS);


	MemCDC.SelectObject(OldMapPtr);
	MemCDC.SelectObject(def_font);
	//	MemCDC.DeleteDC();
	//	MemCBitmap.DeleteObject();
	//	font.DeleteObject();

	return S_OK;
}

HPALETTE gl_GetPalette(HDC inhDC)
{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
	// Range for each color entry (7,7,and 3)

	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(inhDC);
	DescribePixelFormat(inhDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;	

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	// Loop through all the palette entries
	for(i = 0; i < nColors; i++)
	{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
	}

	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(inhDC,hRetPal,FALSE);
	RealizePalette(inhDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
}
