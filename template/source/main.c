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
	
	printf("START=exit\n");

	for(;;)
	{
		swiWaitForVBlank();
		int keys = keysDown();

		if (keys & KEY_START)
			break;
	}

	mmUnloadSoundbank();

	return 0;
}
