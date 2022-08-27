#include "CORE.H"
#include "MCARDX.H"
#include "PSX/MAIN.H"
#include "GAMELOOP.H"
#include "MCARD/MEMCARD.H"
#include "MENU/MENUDEFS.H"
#include "LOCAL/LOCALSTR.H"
#include <assert.h>

extern long sub_80163DDC(void* data /*s0*/);
extern long sub_80163DC8(void* data /*s0*/);
extern long sub_80164598(void* data /*s0*/, long val, long flag);
extern long sub_801645B8(void* data /*s0*/, long val, long flag);
extern long sub_80163EB0(void* data /*s0*/);
extern long sub_801645E0(void* data /*s0*/, long val, long flag);
extern long sub_801638CC();
extern void sub_80163D38(struct mcpsx_t* mcpsx, int err);

int dword_801691B0[3] = { 2, 0, 0 };


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
	NULL,
	NULL,
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
	NULL,
	NULL,
	NULL,
};

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

void MCARDX_end(struct mcmenu_t* mcmenu)//801648A4
{
	UNIMPLEMENTED();
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
		if (ret < 4)
		{
			assert(FALSE);
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