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
	
	// Load the module
	mmLoad(MOD_FLATOUTLIES);

	// Load sound effects
	mmLoadEffect(SFX_AMBULANCE);
	mmLoadEffect(SFX_BOOM);

	// Start playing module
	mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP);

	mm_sound_effect ambulance =
	{
		{ SFX_AMBULANCE } ,    // id
		(int)(1.0f * (1<<10)), // rate
		0,                     // handle
		255,                   // volume
		0,                     // panning
	};

	mm_sound_effect boom =
	{
		{ SFX_BOOM } ,         // id
		(int)(1.0f * (1<<10)), // rate
		0,                     // handle
		255,                   // volume
		255,                   // panning
	};

	printf("Maxmod Audio demo\n");
	printf("Hold A for ambulance sound\n");
	printf("Press B for boom sound\n");
	printf("Press START to exit\n");
	
	// Sound effect handle (for cancelling it later)
	mm_sfxhand amb = 0;

	for(;;)
	{	
		swiWaitForVBlank();

		int keys_pressed = keysDown();
		int keys_released = keysUp();

		// Play looping ambulance sound effect out of left speaker if A button is pressed
		if (keys_pressed & KEY_A)
			amb = mmEffectEx(&ambulance);

		// Stop ambulance sound when A button is released
		if (keys_released & KEY_A)
			mmEffectCancel(amb);

		// Play explosion sound effect out of right speaker if B button is pressed
		if (keys_pressed & KEY_B)
			mmEffectEx(&boom);

		// Exit if START button is pressed
		if (keys_pressed & KEY_START)
			break;
	}
	
	// Cleanup
	mmStop();
	mmEffectCancel(amb);
	mmUnload(MOD_FLATOUTLIES);
	mmUnloadEffect(SFX_AMBULANCE);
	mmUnloadEffect(SFX_BOOM);
	mmUnloadSoundbank();

	return 0;
}
