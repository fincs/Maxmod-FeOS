#include <nds.h>

// Sound hardware cleanup functions

void _soundOn()
{
	register int i;

	// Power sound on
	powerOn(POWER_SOUND);
	writePowerManagement(PM_CONTROL_REG, (readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE) | PM_SOUND_AMP);
	REG_SOUNDCNT = 127 | SOUND_ENABLE;

	// Turn all channels off
	for (i = 0; i < 16; i ++)
		SCHANNEL_CR(i) = 0;
}

void _soundOff()
{
	register int i;

	// Turn all channels off
	for (i = 0; i < 16; i ++)
		SCHANNEL_CR(i) = 0;

	// Power sound off
	//REG_SOUNDCNT &= ~SOUND_ENABLE;
	REG_SOUNDCNT = 0;
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_AMP ) | PM_SOUND_MUTE );
	powerOff(POWER_SOUND);
}
