#include "CORE.H"
#include "MCARDX.H"
#include "PSX/MAIN.H"
#include "GAMELOOP.H"
#include "MCARD/MEMCARD.H"
#include "MENU/MENUDEFS.H"

#include <assert.h>

void sub_80164598()
{

}

void (*mcardxFuncTable[12])() = {

	NULL,
	NULL,
	NULL,
	sub_80164598,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

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
	mcpsx->params.offset = 0;
}

void sub_801A03D4(struct mcard_t* card, struct mcmenu_t* mcmenu, struct mcpsx_directory_t* directory, long* nfiles)
{
	memset(card, 0, sizeof(struct mcard_t));

	card->mcpsx = (struct mcpsx_t*)((struct mcard_t*)card + 1);
	
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

void MCARDX_initialize(struct mcmenu_t* mcmenu, struct memcard_t* memcard, int nblocks)
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
	//s0 = mcpsx
	MemCardStart();

	sub_80163C8C(mcpsx);

	mcpsx->state.mode = mode_running;
}

void sub_8016445C(struct mcard_t* mcard)
{
	//s0 = mcard
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

long sub_80163F94(struct mcpsx_t* mcpsx)
{
	if (mcpsx->state.sync != sync_idle)
	{
		return 8;
	}

	mcpsx->state.observed = 1;

	return mcpsx->state.err;
}

void sub_801644B4(struct mcard_t* mcard)
{
	long ret;//a0
	//s0 = mcard
	ret = sub_80163F94(mcard->mcpsx);
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
		mcardxFuncTable[mcard->state.fsm]();
	}
}

void sub_80164C04(struct mcmenu_t* mcmenu, int index, int a2)
{
	//s3 = mcmenu
	//s2 = index
	//sp(0x70) = a2

	GAMELOOP_GetGT();

	sub_801644B4(mcmenu->mcard);
}

int MCARDX_main(struct mcmenu_t* mcmenu, int index)//80165578
{
	struct GameTracker* gt;
	//s1 = mcmenu
	//s2 = index

	gt = GAMELOOP_GetGT();

	gt->gameFlags |= 0x20000000;

	MENUFACE_ChangeStateRandomly(0);

	do_check_controller(gt);

	sub_80164C04(mcmenu, index, 1);

	return 0;
}