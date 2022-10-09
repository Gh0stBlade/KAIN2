#include "Game/CORE.H"
#include "MCARDX.H"
#include "Game/PSX/MAIN.H"
#include "Game/GAMELOOP.H"
#include "Game/MCARD/MEMCARD.H"
#include "Game/MENU/MENUDEFS.H"
#include "Game/LOCAL/LOCALSTR.H"
#include <assert.h>

extern long sub_80163DDC(void* data /*s0*/);
extern long sub_80163DC8(void* data /*s0*/);
extern long sub_80164598(void* data /*s0*/, long val, long flag);
extern long sub_801645B8(void* data /*s0*/, long val, long flag);
extern long sub_801C7F78(void* data /*s0*/, long val, long flag);
extern long sub_80163EB0(void* data /*s0*/);
extern long sub_801645E0(void* data /*s0*/, long val, long flag);
extern long sub_801638CC();
extern void sub_80163D38(struct mcpsx_t* mcpsx, int err);

int dword_801691B0[3] = { 2, 0, 0 };
int dword_80168FD4[10] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4 };

unsigned short charTable[] = {
	0x8140, 0x8149, 0x8168, 0x8194, 0x8190, 0x8193, 0x8195, 0x8166, 0x8169, 0x816A,
	0x8196, 0x817B, 0x8143, 0x817C, 0x8144, 0x815E, 0x8146, 0x8147, 0x8171, 0x8181,
	0x8172, 0x8148, 0x8197, 0x816D, 0x818F, 0x816E, 0x814F, 0x8151, 0x8165, 0x816F, 
	0x8162, 0x8170, 0x8150
};

struct CharTable
{
	unsigned short low;
	unsigned short high;
};

struct CharTable charTable2[] = {
	{ 0x824F, 0x0030 },
	{ 0x8260, 0x0041 },
	{ 0x8281, 0x0061 },
};


const char* gameIdentifier[] = {
	"BASLUS-00708",
	"BESLES-02024",
	"BESLES-02025",
	"BESLES-02026",
	"BESLES-02027",
};


long sub_80164E34(struct mcmenu_t* mcmenu, long flag, long flag2, long index)
{
	struct GameTracker* gt = GAMELOOP_GetGT();//t0, s0
	gt->gameFlags |= 0x20000000;
	
	memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_accessing_card));

	if (flag == fsm_ready)
	{
		mcmenu->state.fsm = (enum fsm_t)flag;
	}

	return -1;
}

void sub_801C86B8(struct mcpsx_directory_t* mcdir, int slot)
{
	language_t lang = localstr_get_language();//s0

	memset(mcdir, 0, sizeof(struct mcpsx_directory_t));
	memcpy(&mcdir->name[0], gameIdentifier[lang], 12);
	sprintf(&mcdir->name[12], "kain %c", slot + 97);
	printf("SetName: %s\n", mcdir->name);
}

unsigned short sub_801C82FC(char c)
{
	//a2 = c
	int v1 = 0;
	int a0 = 0;
	//a1 = c
	if (c < 0x20)
	{
		return 0;
	}

	if (c < 0x30)
	{
		int a0 = 0x3F;
		if (a0 == 0)
		{
			a0 += 0x1F;
			assert(FALSE);//unimpl
		}
		else
		{
			return charTable[c - 0x3F];
		}
	}

	if (c < 0x3A)
	{
		int a0 = 0x1F;
		assert(FALSE);//unimpl
	}

	if (c < 0x5B)
	{
		v1 = 1;

		if (a0 == 0)
		{
			a0 = 0x1F;
			return c + charTable2[v1].low - charTable2[v1].high;
		}
		else
		{
			a0 = 0x1F;
			assert(FALSE);//unimpl
		}
	}

	if (c < 0x61)
	{
		assert(FALSE);//unimpl
	}

	if (c < 0x7B)
	{
		v1 = 2;
		if (a0 == 0)
		{
			a0 = 0x1F;
			return c + charTable2[v1].low - charTable2[v1].high;
		}
		else
		{
			a0 = 0x1F;
			assert(FALSE);//unimpl
		}
	}

	if (c < 0x7F)
	{
		assert(FALSE);//unimpl
	}

	return 0;
}

void sub_801C83E0(const char* name, char* buff)
{
	short* b = (short*)buff;

	while (*name != 0)
	{
		unsigned short v0 = sub_801C82FC(*name++);
		unsigned short v1 = (v0 & 0xFF00) >> 8;
		
		v0 &= 0xFF;
		v0 <<= 8;
		v1 |= v0;
		*b++ = v1;
	}

	*b = 0;
}

void sub_801C8624(void* buff, const char* name)
{
	char* v0 = ((char*)buff) + 4;

	sub_801C83E0(name, v0);
}

void sub_801C87C4(void* buff, const char* name)
{
	sub_801C8624(buff, name);
}

void sub_801C73F8(struct mcard_t* mcard, struct mcpsx_directory_t* dir, int nblocks, void* buffer, int nbytes)
{
	mcard->state.fsm = fsmcard_create;
	mcard->params.filename = dir->name;
	mcard->params.nblocks = nblocks;
	mcard->params.buffer = buffer;
	mcard->params.nbytes = nbytes;
}

long sub_801653F0(struct mcmenu_t* mcmenu, long flag, long flag2, long index)
{
	memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_saving));

	memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_done));

	if (flag == 4)
	{
		mcmenu->state.fsm = fsm_save_complete;
	}

	return -1;
}

long sub_801C8108(struct mcmenu_t* mcmenu, long flag, long flag2, long index)
{
	sub_801C86B8(&mcmenu->params.directory[mcmenu->params.nfiles], mcmenu->state.slot);

	char buff[32];
	sprintf(buff, "Legacy Of Kain: Soul Reaver %c", mcmenu->state.slot + 0x41);

	sub_801C87C4(mcmenu->params.buffer, buff);

	sub_801C73F8(mcmenu->mcard, &mcmenu->params.directory[mcmenu->params.nfiles], mcmenu->params.nblocks, mcmenu->params.buffer, mcmenu->params.nbytes);

	memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_saving));
	memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_done));
	
	GAMELOOP_GetGT()->gameFlags |= 0x20000800;

	mcmenu->state.fsm = fsm_saving;

	return -1;
}

long sub_80165154(struct mcmenu_t* mcmenu, long flag, long flag2, long index)
{
	memcard_item(mcmenu->opaque, NULL, 0, 0, " ");

	memcard_start(mcmenu->opaque);

	return -1;
}

extern long sub_80164E7C(struct mcmenu_t* mcmenu, long flag, long flag2, long index);

//0x801638CC
long (*mcardxFuncTable3[12])(struct mcmenu_t*, long, long, long) = {

	NULL,
	NULL,
	NULL,
	sub_80164E34,
	sub_80164E7C,
	NULL,
	sub_80165154,
	NULL,
	NULL,
	NULL,
	sub_801653F0,
	sub_801C8108,
};

//0x8016384C
long (*mcardxFuncTable2[12])(void*) = {

	sub_80163DC8,
	sub_80163DDC,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	sub_80163EB0,
	NULL,
	NULL,
	NULL,
};

//0x801638CC

const long dword_80163879 = 0x2A;

long (*mcardxFuncTable[12])(void*, long, long) = {

	NULL,
	NULL,
	NULL,
	sub_80164598,
	sub_801645B8,
	sub_801645E0,
	NULL,
	NULL,
	NULL,
	sub_801C7F78,
	NULL,
	NULL,
};

int sub_801C7630(void* opaque, long param, menu_ctrl_t ctrl)
{
	//s1 = param
	//s0 = ctrl
	struct mcmenu_t* mcmenu;
	
	mcmenu = (struct mcmenu_t*)gt2mcmenu(opaque);

	if (ctrl == menu_ctrl_engage)
	{
		mcmenu->state.fsm = (fsm_t)param;
		return 1;
	}
	else if (ctrl == menu_ctrl_cancel)
	{
		memcard_pop(mcmenu->opaque);
		GAMELOOP_GetGT()->gameFlags &= 0xDFFFFFFF;
		return 1;
	}

	return 0;
}

int sub_801C75C4(void* opaque, long param, menu_ctrl_t ctrl)
{
	struct mcmenu_t* mcmenu;

	mcmenu = (struct mcmenu_t*)gt2mcmenu(opaque);

	if ((unsigned int)ctrl - 5 < menu_ctrl_down)
	{
		memcard_pop(mcmenu->opaque);

		GAMELOOP_GetGT()->gameFlags &= 0xDFFFFFFF;

		return 1;
	}

	return 0;
}

long sub_80164E7C(struct mcmenu_t* mcmenu, long flag, long flag2, long index)
{
	int s0 = 0;
	//sp+0x3C = 0;
	//v0 = mcmenu->params.nfiles
	int s4 = 0;
	//s5 = mcmenu->params.nblocks;

	if (mcmenu->params.nfiles > 0)
	{
		assert(FALSE);//unimpl
	}

	if (s4 > 0)
	{
		if (mcmenu->params.nblocks == mcmenu->params.nblocks + 0xF)
		{
			assert(FALSE);//unimpl
		}

	}
	if (mcmenu->params.nblocks >= 16)
	{
		assert(FALSE);
	}

	if (flag2 != 0)
	{
		mcmenu->state.fsm = fsm_start;

		if (mcmenu->state.fsm != mcmenu->state.fsm_prev)
		{
			index = -1;
		}

		if (mcmenu->state.fsm < fsm_num_items)
		{
			mcardxFuncTable3[mcmenu->state.fsm](mcmenu, 1, 0, index);
		}
	}
	else
	{
		memcard_item(mcmenu->opaque, NULL, NULL, 4, localstr_get(LOCALSTR_choose_save_file));
		memcard_item(mcmenu->opaque, sub_801C7630, 11, 0, localstr_get(LOCALSTR_create_file));

		if (index < 0)
		{
			index = 1;
		}

		memcard_item(mcmenu->opaque, sub_801C75C4, 0, 0, localstr_get(LOCALSTR_go_back));

		return index;
	}

	return -1;
}

void sub_80163D50(struct mcpsx_t* mcpsx, long val)
{
	if (val != 0)
	{
		mcpsx->state.sync = sync_busy;
	}
}

long sub_801642B8(long channel, struct mcpsx_directory_t* directory, long* nfilesptr)
{
	struct DIRENTRY dirEnt;

	long ret = MemCardGetDirentry(channel, "*", &dirEnt, nfilesptr, 0, 15);

	if (ret == 0)
	{
		if (ret < nfilesptr[0])
		{
			assert(FALSE);
		}
	}

	return nfilesptr[0];
}

void sub_80163D64(struct mcpsx_t* mcpsx, long err)
{
	sub_80163D38(mcpsx, err);
}

long sub_80163EB0(void* data /*s0*/)
{
	struct mcpsx_t* mcpsx = (struct mcpsx_t*)data;

	sub_80163D64(mcpsx, sub_801642B8(mcpsx->params.channel, mcpsx->params.directory, mcpsx->params.nfilesptr));
	
	return 0;
}

long sub_80163DC8(void* data /*s0*/)
{
	struct mcpsx_t* mcpsx = (struct mcpsx_t*)data;
	return MemCardAccept(mcpsx->params.channel);
}

long sub_80163DDC(void* data /*s0*/)
{
	struct mcpsx_t* mcpsx = (struct mcpsx_t*)data;
	long ret = MemCardAccept(mcpsx->params.channel);

	sub_80163D50(mcpsx, ret);

	return ret;
}

long sub_801638CC(void* data /*s0*/)
{
	struct mcpsx_t* mcpsx = (struct mcpsx_t*)data;
	long ret = MemCardAccept(mcpsx->params.channel);

	sub_80163D50(mcpsx, ret);

	return ret;
}

long sub_80163D84(struct mcpsx_t* mcpsx)
{
	//s0 = mcpsx
	if (mcpsx->state.func - 1 < 9)
	{
		return mcardxFuncTable2[mcpsx->state.func - 1]((struct mcard_t*)mcpsx);
	}
}

void sub_80163D38(struct mcpsx_t* mcpsx, int err)
{
	mcpsx->state.func = func_none;
	mcpsx->state.sync = sync_idle;
	mcpsx->state.err = (mcpsx_err_t)err;
	mcpsx->state.observed = 0;
}

long sub_80163EE0(struct mcpsx_t* mcpsx)
{
	long cmds;
	long result;
	long ret;

	ret = MemCardSync(1, &cmds, &result);
	mcpsx->state.sync = (mcpsx_sync_t)ret;

	if(ret == 1)
	{
		sub_80163D38(mcpsx, result);

	}

	return ret;
}

long sub_80163F28(struct mcpsx_t* mcpsx)
{
	if (mcpsx->state.observed != 0)
	{
		if (mcpsx->state.sync != sync_busy)
		{
			if (mcpsx->state.sync <= 0)
			{
				return 1;
			}

			if (mcpsx->state.sync == sync_func)
			{
				sub_80163D84(mcpsx);
			}
		}
		else
		{
			sub_80163EE0(mcpsx);
		}
	}

	return 0;
}

void sub_80163FB8(struct mcpsx_t* mcpsx, mcpsx_func_t func)
{
	mcpsx->state.func = func;
	mcpsx->state.sync = sync_func;
}

long sub_80164008(struct mcpsx_t* mcpsx)
{
	long ret = sub_80163F28(mcpsx);

	if (ret == 0)
	{
		return 0;
	}
	else
	{
		sub_80163FB8(mcpsx, func_accept);
		return 1;
	}
}

long sub_80163FC8(struct mcpsx_t* mcpsx)
{
	//s0 = mcpsx
	if (sub_80163F28(mcpsx) == 0)
	{
		return 0;
	}
	else
	{
		sub_80163FB8(mcpsx, func_exist);
		return 1;
	}
}

long sub_80164258(struct mcpsx_t* mcpsx, struct mcpsx_directory_t* directory, long* nfilesptr)
{
	//s0 = mcpsx
	//s1 = directory
	//s2 = nfilesptr
	if (sub_80163F28(mcpsx) == 0)
	{
		return 0;
	}
	else
	{
		sub_80163FB8(mcpsx, func_directory);
	}

	mcpsx->params.directory = directory;
	mcpsx->params.nfilesptr = nfilesptr;

}

long sub_801645E0(void* data /*s0*/, long val, long flag)
{
	struct mcard_t* mcard = (struct mcard_t*)data;//s0

	if (val == 0)
	{
		mcard->state.status = mcard_status_ready;
	}

	if (val == 3)
	{
		mcard->state.fsm = fsmcard_new_card;
	}

	if (flag != 0)
	{
		sub_80163FC8(mcard->mcpsx);
	}

	return mcard->state.status;
}

long sub_801645B8(void* data /*s0*/, long val, long flag)
{
	struct mcard_t* mcard = (struct mcard_t*)data;//s0
	long ret = sub_80164258(mcard->mcpsx, mcard->params.directory, mcard->params.nfilesptr);

	if (ret != 0)
	{
		mcard->state.fsm = fsmcard_ready;

		if (ret != 0)
		{
			sub_80163FC8(mcard->mcpsx);
		}
	}


	return mcard->state.status;
}

void sub_801C7428(struct mcard_t* mcard, struct mcpsx_directory_t* dir, void* buffer, int nbytes)
{
	mcard->state.fsm = fsmcard_directory;
	mcard->params.directory = dir;
	mcard->params.filename = (char*)buffer;
	mcard->params.nbytes = nbytes;
}

long sub_801C7F78(void* data /*s0*/, long val, long flag)
{
	struct mcard_t* mcard = (struct mcard_t*)data;
	struct mcmenu_t* mcmenu = gameTrackerX.memcard->mcmenu;

	sub_801C86B8(&mcmenu->params.directory[mcmenu->params.nfiles], mcmenu->state.slot);

	char buff[32];

	sprintf(buff, "Legacy Of Kain: Soul Reaver %c", flag + 0x41);

	sub_801C87C4(mcmenu->params.buffer, buff);

	memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_saving));
	memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_done));

	memcard_save(mcmenu->opaque);

	GAMELOOP_GetGT()->gameFlags |= 0x20000800;

	sub_801C7428(mcmenu->mcard, &mcmenu->params.directory[mcmenu->state.slot], mcmenu->params.buffer, mcmenu->params.nbytes);

	mcmenu->state.fsm = fsm_saving;

	return -1;
}

long sub_80164598(void* data /*s0*/, long val, long flag)
{
	struct mcard_t* mcard = (struct mcard_t*)data;
	struct mcpsx_t* mcpsx;//a0
	
	mcpsx = mcard->mcpsx;

	long ret = sub_80164008(mcpsx);

	if (ret == 0)
	{
		return mcpsx->state.sync;
	}
	else
	{
		mcard->state.fsm = fsmcard_directory;

		if (ret != 0)
		{
			sub_80163FC8(mcpsx);
		}

		return mcard->state.status;
	}
}

void sub_801A039C(struct mcard_t* card)
{
	card->state.err = mcpsx_err_new_card;
	card->state.fsm = fsmcard_new_card;
	card->state.status = mcard_status_new_card;
	card->state.not_exists = 0;
}

void sub_8019FC00(struct mcpsx_t* mcpsx, struct mcard_t* card)
{
	memset(mcpsx, 0, sizeof(struct mcpsx_t));
	PadStopCom();
	MemCardInit(1);//801A3860
	PadStartCom();

	mcpsx->state.mode = mode_initialized;
	mcpsx->state.sync = sync_idle;
	mcpsx->state.func = func_none;
	mcpsx->state.err = mcpsx_err_busy;
	mcpsx->state.observed = 0;
	mcpsx->params.channel = 0;
	mcpsx->params.filename = 0;
	mcpsx->opaque = card;
}

void sub_801A03D4(struct mcard_t* card, struct mcmenu_t* mcmenu, struct mcpsx_directory_t* directory, long* nfiles)
{
	memset(card, 0, sizeof(struct mcard_t));

	card->mcpsx = (struct mcpsx_t*)((struct mcard_t*)card + 1);
	card->opaque = mcmenu;

	card->params.directory = directory;
	card->params.nfilesptr = nfiles;

	sub_8019FC00(card->mcpsx, card);
	sub_801A039C(card);
}

int sub_801A17A8(void* buffer, int* a1, int nbytes, int a3, void* var_10, int* var_14)
{
	//s3 = buffer
	//s0 = a1
	//s2 = nbytes
	//s4 = a3

	memset(buffer, 0, (a3 << 7) + 256);
	return 0;//temp
}

static int unk_8019F934 = 32;
static int unk_801A512C = 0;

int sub_801A1ADC(void* buffer, int nbytes)
{
	return sub_801A17A8(buffer, &unk_8019F934, nbytes, 1, NULL, &unk_801A512C);
}

void MCARDX_initialize(struct mcmenu_t* mcmenu, struct memcard_t* memcard, int nblocks)//0x801A07D4
{
	memset(mcmenu, 0, sizeof(struct mcmenu_t));

	mcmenu->state.status = mcard_status_new_card;
	mcmenu->state.fsm = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;
	mcmenu->state.running = 0;
	mcmenu->params.nblocks = nblocks;
	mcmenu->mcard = (struct mcard_t*)(mcmenu + 1);
	mcmenu->opaque = memcard;

	sub_801A03D4(mcmenu->mcard, mcmenu, mcmenu->params.directory, &mcmenu->params.nfiles);
}

int MCARDX_set_buffer(struct mcmenu_t* mcmenu, void* buffer, int nbytes)
{
	mcmenu->params.buffer = buffer;
	mcmenu->params.nbytes = nbytes;

	return sub_801A1ADC(buffer, mcmenu->params.nblocks);
}

void sub_80163CD0(struct mcpsx_t* mcpsx)
{
	MemCardStart();

	sub_80163C8C(mcpsx);

	mcpsx->state.mode = mode_running;
}

void sub_8016445C(struct mcard_t* mcard)
{
	sub_80163CD0(mcard->mcpsx);

	sub_80164380(mcard);
}

void sub_80164380(struct mcard_t* mcard)//80164380
{
	mcard->state.err = mcpsx_err_new_card;
	mcard->state.fsm = fsmcard_new_card;
	mcard->state.status = mcard_status_new_card;
	mcard->state.not_exists = 0;
}

void sub_80163D04(struct mcpsx_t* mcpsx)
{
	sub_80163C8C(mcpsx);

	MemCardStop();

	mcpsx->state.mode = mode_initialized;
}

void sub_80164990(struct mcard_t* mcmenu)
{
	sub_80163D04(mcmenu->mcpsx);
}

void MCARDX_end(struct mcmenu_t* mcmenu)//801648A4
{
	//s0 = mcmenu
	sub_80164990(mcmenu->mcard);

	mcmenu->state.running = 0;

	return;
}

void MCARDX_begin(struct mcmenu_t* mcmenu)//80164860
{
	sub_8016445C(mcmenu->mcard);

	mcmenu->state.status = mcard_status_new_card;
	mcmenu->state.fsm = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;
	mcmenu->state.running = 1;
}

void sub_80163C8C(struct mcpsx_t* mcpsx)
{
	long cmds;
	long result;
	MemCardSync(0, &cmds, &result);

	mcpsx->state.sync = sync_idle;
	mcpsx->state.func = func_none;
	mcpsx->state.err = mcpsx_err_busy;
}

long sub_80163F94(struct mcpsx_t* mcpsx, int index)
{
	if (mcpsx->state.sync != sync_idle)
	{
		return 8;
	}

	mcpsx->state.observed = 1;
	return mcpsx->state.err;
}


long sub_801644B4(struct mcard_t* mcard, int index)
{
	long ret;//a0, s1
	//s0 = mcard
	ret = sub_80163F94(mcard->mcpsx, index);
	//s1 = 1
	//v1 = mcard->err
	if (ret != mcard->state.err && ret != 0 && ret < 8)
	{
		//v1 = ret << 1
		//v1 += ret
		//v1 <<= 2

		//Seemingly loads the memorycard buffer here!
		assert(FALSE);
	}

	if (mcard->state.fsm >= fsmcard_format)
	{
		mcard->state.status = mcard_status_accessing;
	}

	if (mcard->state.fsm < fsm_save_complete)
	{
		ret = mcardxFuncTable[mcard->state.fsm](mcard, ret, 1);

		return ret;//? see below!

		//0x801645A4
		if (ret != 0)
		{
			mcard->state.fsm = fsmcard_directory;
		}
		//0x80164708

	}
	else
	{
		//0x801646E8
		assert(FALSE);
	}

	//0x801646F4
	if (ret != 0)
	{
		sub_80163FC8(mcard->mcpsx);
	}

	return 0;///checkme
}

int sub_80164C04(struct mcmenu_t* mcmenu, int index, int a2)
{
	GAMELOOP_GetGT();

	long ret = sub_801644B4(mcmenu->mcard, index);

	if (ret != mcmenu->state.status)
	{
		if ((unsigned int)ret < 4)
		{
			mcmenu->state.status = (mcard_status_t)ret;
			mcmenu->state.fsm = (fsm_t)dword_80168FD4[ret * 2 + 1];
		}
	}

	if (mcmenu->state.fsm != mcmenu->state.fsm_prev)
	{
		index = -1;
	}

	mcmenu->state.fsm_prev = mcmenu->state.fsm;

	if (mcmenu->state.fsm < fsm_error)
	{
		return mcardxFuncTable3[mcmenu->state.fsm](mcmenu, ret, a2, index);
	}
}

int MCARDX_main(struct mcmenu_t* mcmenu, int index)//80165578
{
	struct GameTracker* gt;
	
	gt = GAMELOOP_GetGT();

	gt->gameFlags |= 0x20000000;

	MENUFACE_ChangeStateRandomly(0);

	do_check_controller(gt);

	return sub_80164C04(mcmenu, index, 1);
}

int sub_801C6C54(struct mcpsx_t* mcpsx)
{
	if (mcpsx->state.sync != sync_idle)
	{
		return 8;
	}

	mcpsx->state.observed = 1;

	return mcpsx->state.err;
}

void sub_801C7174(struct mcard_t* mcard)
{
	//s0 = mcard
	//s1 = 1
	int ret;

	ret = sub_801C6C54(mcard->mcpsx);
	
	if (ret != mcard->state.err && ret != 0 && ret < 0)
	{
		assert(FALSE);//unimpl
	}

	if (mcard->state.fsm >= 6)
	{
		mcard->state.status = mcard_status_accessing;
	}

	if (mcard->state.fsm < fsmcard_error)
	{

	}
}

int sub_801C78C4(struct mcmenu_t* mcmenu, int index, int flag)
{
	//s3 = mcmenu
	//s2 = index
	//sp(0x70) = flag
	//sp(0x38) = GAMELOOP_GetGT();

	sub_801C7174(mcmenu->mcard);
	return 0;
}

//x2
int MCARDX_pause(struct mcmenu_t* mcmenu, int index)//801C82A4
{
	struct GameTracker* gt;
	
	gt = GAMELOOP_GetGT();
	
	gt->gameFlags |= 0x20000000;

	do_check_controller(gt);

	return sub_80164C04(mcmenu, index, 0);
}