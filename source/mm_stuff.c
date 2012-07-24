//
// Maxmod for FeOS
// SENSIBLE soundbank loading functions - the ones in mm_soundbank.s
// AND mm_main9.s HORRIBLY SUCKED and HORRIBLY LEAKED MEMORY, and also
// HORRIBLY didn't support passing a custom FILE* in (NECESSARY for FAR
// loading), and even DID ABSOLUTELY NO ERROR CHECKING
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <feos.h>

#include <maxmod9.h>

#define MMCB_SONGREQUEST  0x1A
#define MMCB_SAMPREQUEST  0x1B
#define MMCB_DELETESONG   0x1C
#define MMCB_DELETESAMPLE 0x1D

static FILE* fp;
static void* m;
static void* pMemToFree;
static bool fpOwn;

static int nsamps, nmods;

FEOS_EXPORT void mmUnloadSoundbank()
{
	if (pMemToFree)
	{
		free(pMemToFree);
		pMemToFree = NULL;
	}
	if (fp && fpOwn) fclose(fp);
	fp = NULL;
	m = NULL;
	nsamps = 0;
	nmods = 0;
	fpOwn = false;
}

extern const mm_word _mmChannel;

static bool _commonInit(mm_callback cb)
{
	mm_ds_system sys;
	sys.mod_count    = nmods;
	sys.samp_count   = nsamps;
	sys.mem_bank     = pMemToFree;
	sys.fifo_channel = _mmChannel;
	mmInit(&sys);
	mmSetCustomSoundBankHandler(cb);
	return true;
}

static mm_word fileLoader(mm_word msg, mm_word param);

FEOS_EXPORT bool mmInitDefault( const char* soundbank_file )
{
	return mmInitDefaultHandle(fopen(soundbank_file, "rb"), true);
}

FEOS_EXPORT bool mmInitDefaultHandle(FILE* f, bool bTakeOwnership)
{
	mmUnloadSoundbank();

	if (!f) return false;
	fp = f;
	fpOwn = bTakeOwnership;

	fread(&nsamps, 2, 1, fp);
	fread(&nmods, 2, 1, fp);

	pMemToFree = malloc(4*(nmods+nsamps));
	if (!pMemToFree)
	{
		mmUnloadSoundbank();
		return false;
	}

	return _commonInit(fileLoader);
}

static void* file_loadFile(int id);
static void* file_loadFile(int id);

mm_word fileLoader(mm_word msg, mm_word param)
{
	switch(msg)
	{
		case MMCB_SAMPREQUEST:
			return (mm_word) file_loadFile((int)param);
		case MMCB_SONGREQUEST:
			return (mm_word) file_loadFile((int)param + nsamps);
		case MMCB_DELETESAMPLE:
		case MMCB_DELETESONG:
			free((void*) param);
		default:
			break;
	}
	return 0;
}

void* file_loadFile(int id)
{
	fseek(fp, 12+id*4, SEEK_SET);
	int off = 0;
	fread(&off, sizeof(off), 1, fp);
	fseek(fp, off, SEEK_SET);
	int size = 0;
	fread(&size, sizeof(off), 1, fp);
	fseek(fp, off, SEEK_SET);
	size += 8; // header
	void* mem = malloc(size);
	if (!mem) return NULL;
	fread(mem, 1, size, fp);
	return mem;
}

static mm_word memLoader(mm_word msg, mm_word param);

FEOS_EXPORT bool mmInitDefaultMem( mm_addr soundbank )
{
	mmUnloadSoundbank();
	void* _m = (void*)soundbank;
	if (!_m) return false;
	m = _m;

	nmods = ((u16*)_m)[0];
	nsamps = ((u16*)_m)[1];

	pMemToFree = malloc(4*(nmods+nsamps));
	if (!pMemToFree)
	{
		mmUnloadSoundbank();
		return false;
	}

	return _commonInit(memLoader);
}

mm_word memLoader(mm_word msg, mm_word param)
{
	switch(msg)
	{
		case MMCB_SAMPREQUEST:
		case MMCB_SONGREQUEST:
		{
			int id = (int)param + (msg == MMCB_SONGREQUEST ? nsamps : 0);
			return ((mm_word*)m)[3+id];
		}
	}
	return 0;
}
