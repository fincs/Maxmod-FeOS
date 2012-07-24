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

enum
{
	rv_delay_left = 500, // milliseconds
	rv_delay_right = 520,
	rv_rate = 32768, // Hertz
	rv_format = 16 // 16-bit
};

void setupReverb()
{
	//---------------------------------------------------------
	// Enable reverb system
	//---------------------------------------------------------
	mmReverbEnable();
	
	//---------------------------------------------------------
	// Calculate amount required and allocate memory
	// for both reverb channels
	//---------------------------------------------------------
	void* rv_buffer_left;
	void* rv_buffer_right;

	int rv_size_left;
	int rv_size_right;
	rv_size_left = mmReverbBufferSize(rv_format, rv_rate, rv_delay_left);
	rv_size_right = mmReverbBufferSize(rv_format, rv_rate, rv_delay_right);
	
	rv_buffer_left  = calloc(rv_size_left, 4);
	rv_buffer_right = calloc(rv_size_right, 4);
	
	//---------------------------------------------------------
	// Configure reverb settings
	//---------------------------------------------------------
	mm_reverb_cfg config;
	
	config.flags = MMRF_MEMORY | MMRF_DELAY | MMRF_FEEDBACK |
	               MMRF_PANNING | MMRF_DRYLEFT | MMRF_DRYRIGHT |
	               MMRF_RATE | MMRF_16BITLEFT | MMRF_16BITRIGHT |
	               MMRF_INVERSEPAN | MMRF_BOTH;

	config.memory = rv_buffer_left;   // Set memory target (for left)
	config.delay = rv_size_left;      // Set delay (for left)
	config.feedback = 1024;           // Set feedback to 50% (for both)
	config.panning = 0;               // Set panning to 0% (and inversed (100%) for right channel)
	config.rate = 16777216 / rv_rate; // Set sampling rate for both channels
	mmReverbConfigure(&config);       // Run configuration...

	//---------------------------------------------------------
	// Configure remaining parameters (right channel memory
	// and delay)
	//---------------------------------------------------------
	config.flags = MMRF_MEMORY | MMRF_DELAY | MMRF_RIGHT;
	
	config.delay = rv_size_right;
	config.memory = rv_buffer_right;
	
	mmReverbConfigure(&config);
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

	// Setup and configure the reverb system
	setupReverb();

	//---------------------------------------------------------
	// Load and play test module
	//---------------------------------------------------------
	mmLoad(MOD_TEMPEST_ZEN_BOWLING);
	mmStart(MOD_TEMPEST_ZEN_BOWLING, MM_PLAY_LOOP);
	
	printf("Maxmod Reverb Example\nPress A to toggle\nPress START to exit\n");

	bool bReverb = false;

	for(;;)
	{
		swiWaitForVBlank();
		int keys = keysDown();

		if (keys & KEY_A)
		{
			bReverb = !bReverb;
			if (bReverb)
			{
				// Start reverb
				mmReverbStart(MMRC_BOTH);

				// Lower module volume & tempo
				// This creates a cool atmosphere
				mmSetModuleVolume(550);
				mmSetModuleTempo(800);
			}else
			{
				// Stop reverb and restore volume and tempo
				mmReverbStop(MMRC_BOTH);
				mmSetModuleVolume(0x400);
				mmSetModuleTempo(0x400);
			}
		}

		if (keys & KEY_START)
			break;
	}

	mmReverbStop(MMRC_BOTH);
	mmStop();
	mmUnload(MOD_TEMPEST_ZEN_BOWLING);
	mmUnloadSoundbank();

	return 0;
}
