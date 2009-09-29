// ------------------------------------------------------------------
// Elf 2D engine - a game development framework
// ------------------------------------------------------------------
// Collection of extra features built on top of the framework
// ------------------------------------------------------------------

#ifndef __ELF_EXTRA_H_INCLUDED__
#define __ELF_EXTRA_H_INCLUDED__

#include "Elf.h"

extern "C"
{
	// Anti-aliased lines
	// Draws 1 pixel thick lines using a gradient texture
	//
	// Expects states:
	// 1. Alpha blending - enabled.
	// 2. Alpha blending modes - ElfBM_SrcAlpha, ElfBM_InvSrcAlpha.
	//
	// Modifies states:
	// 1. Texture and texture wrapping.
	// 2. elfQuads texture region.
	// 3. elfQuads colour.

	Elf2D_EXPORTED void elfExtra_linesBegin();
	Elf2D_EXPORTED void elfExtra_linesColour(unsigned long colour);
	Elf2D_EXPORTED void elfExtra_linesDraw(const float& fromX, const float& fromY, const float& toX, const float& toY);
	Elf2D_EXPORTED void elfExtra_linesEnd();

	// Variable-width fonts
	// Integrated from http://www.angelcode.com/products/bmfont/
	//
	// Notes:
	// 1. Only one font 'page' is supported. 8 bit texture is suitable.
	// 
	// Expects states:
	// 1. Alpha blending - enabled.
	// 2. Alpha blending modes - ElfBM_SrcAlpha, ElfBM_InvSrcAlpha.
	//
	// Modifies states:
	// 1. Texture and texture wrapping.
	// 2. elfQuads texture region.
	// 3. elfQuads colour.

	Elf2D_EXPORTED bool elfFont_load(const char* id, const char* fileName, int textureId);
	Elf2D_EXPORTED int  elfFont_getId(const char* id);
	Elf2D_EXPORTED void elfFont_unload(int id);
	Elf2D_EXPORTED void elfFont_bind(int id);

	// Text alignment modes
	enum ElfTextAlignmentMode
	{
		ElfTAM_Left		= 0,
		ElfTAM_Center	= 1,
		ElfTAM_Right	= 2,
		ElfTAM_Justify	= 3
	};

	// The following functions work on the font that's currently bound
	Elf2D_EXPORTED float elfFont_getTextWidth(const char* text);
	Elf2D_EXPORTED float elfFont_getBottomOffset();
	Elf2D_EXPORTED float elfFont_getTopOffset();

	Elf2D_EXPORTED void  elfFont_setHeight(float h);
	Elf2D_EXPORTED void  elfFont_colour(unsigned long colour);

	Elf2D_EXPORTED void  elfFont_draw(float x, float y, const char* text);
	Elf2D_EXPORTED void  elfFont_drawML(float x, float y, const char* text);
	Elf2D_EXPORTED void  elfFont_drawBox(float x, float y, float width, const char* text, ElfTextAlignmentMode mode);
};

#endif // __ELF2D_H_INCLUDED__
