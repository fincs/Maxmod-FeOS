// Maxmod FeOS app template

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <feos.h>
#include <maxmod9.h>
#include <far.h>

#include "soundbank.h"

// FAR loading boilerplate

far_t hFar;

FEOSINIT void loadFar()
{
	hFar = FAR_OpenSelf();
}

FEOSFINI void closeFar()
{
	if (hFar) FAR_Close(hFar);
}

// A simple sprite structure
typedef struct
{
	u16* gfx;
	int size;
	int format;
	int rotationIndex;
	int paletteAlpha;
	int x;
	int y;
	int dy;
} MySprite;


// Create 5 sprites, one for each song event used
MySprite sprites[] =
{
	{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 20, 96, 0},
	{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 70, 96, 0},
	{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 120, 96, 0},
	{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 170, 96, 0},
	{0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, 0, 220, 96, 0}
};

mm_word myEventHandler(mm_word msg, mm_word param)
{
	switch (msg)
	{
		case MMCB_SONGMESSAGE:
			sprites[param-1].dy = -8;
			break;
	}

	return 0;
}

// Program entrypoint
int main()
{	
	if (!mmIsARM7Loaded())
	{
		printf("Couldn't load Maxmod!\n");
		return 1;
	}

	// Init Maxmod with default settings (loading from FAR)
	mmInitDefaultFAR(hFar, "/soundbank.bin");
	
	FeOS_DirectMode();

	videoSetMode(MODE_0_2D);
	videoSetModeSub(0);
	vramSetBankA(VRAM_A_MAIN_SPRITE);

	// Initialize the sprite engine with 1D mapping 128 byte boundary
	// aand no external palette support
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	int i;
	for (i = 0; i < 5; i ++)
	{
		// Allocate some space for the sprite graphics
		sprites[i].gfx = oamAllocateGfx(&oamMain, sprites[i].size, sprites[i].format);

		// Fill each sprite with a different index (2 pixels at a time)
		dmaFillHalfWords(((i+1) << 8) | (i+1), sprites[i].gfx, 32*32);
	}

	// Set indexes to different colours
	SPRITE_PALETTE[1] = RGB15(31,0,0);
	SPRITE_PALETTE[2] = RGB15(0,31,0);
	SPRITE_PALETTE[3] = RGB15(0,0,31);
	SPRITE_PALETTE[4] = RGB15(31,0,31);
	SPRITE_PALETTE[5] = RGB15(0,31,31);

	mmSetEventHandler(myEventHandler);
	mmLoad(MOD_EXAMPLE2);
	mmStart(MOD_EXAMPLE2, MM_PLAY_LOOP);

	for(;;)
	{
		swiWaitForVBlank();
		if (keysDown() & KEY_START)
			break;

		for (i = 0; i < 5; i ++)
		{
			// Constantly increase the sprite's y velocity
			sprites[i].dy += 1;
		
			// Update the sprite's y position with its y velocity
			sprites[i].y += sprites[i].dy;
		
			// Clamp the sprite's y position
			if (sprites[i].y<72) sprites[i].y = 72;
			if (sprites[i].y>96) sprites[i].y = 96;
		
			oamSet(&oamMain,                 // Main graphics engine context
			       i,                        // OAM index (0 to 127)  
			       sprites[i].x,             // X and Y pixel location of the sprite
			       sprites[i].y, 			
			       0,                        // Priority, lower renders last (on top)
			       sprites[i].paletteAlpha,  // Palette index 
			       sprites[i].size,
			       sprites[i].format,
			       sprites[i].gfx,           // Pointer to the loaded graphics
			       sprites[i].rotationIndex, // Sprite rotation data  
			       false,                    // Double the size when rotating?
			       false,                    // Hide the sprite?
			       false, false,             // VFlip, HFlip
			       false);                   // Apply mosaic
		}
	}

	mmStop();
	mmUnload(MOD_EXAMPLE2);
	mmUnloadSoundbank();

	FeOS_ConsoleMode();

	return 0;
}
