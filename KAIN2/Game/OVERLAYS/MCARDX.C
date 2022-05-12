#include "CORE.H"
#include "MCARDX.H"
#include "PSX/MAIN.H"

#include "MCARD/MEMCARD.H"

void sub_801A039C(mcard_t* card)
{
	card->state.err = mcpsx_err_new_card;
	card->state.fsm = fsmcard_new_card;
	card->state.status = mcard_status_new_card;
	card->state.not_exists = 0;
}

void sub_8019FC00(mcpsx_t* mcpsx, mcard_t* card)
{
	memset(mcpsx, 0, sizeof(mcpsx_t));
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

void sub_801A03D4(mcard_t* card, mcmenu_t* mcmenu, mcpsx_directory_t* directory, long* nfiles)
{
	memset(card, 0, sizeof(mcard_t));

	card->mcpsx = ((mcpsx_t*)card + 1);
	
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

void MCARDX_initialize(mcmenu_t* mcmenu, memcard_t* memcard, int nblocks)
{
	memset(mcmenu, 0, sizeof(mcmenu_t));

	mcmenu->state.status = mcard_status_new_card;
	mcmenu->state.fsm = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;
	mcmenu->state.running = 0;
	mcmenu->params.nblocks = nblocks;
	mcmenu->mcard = (mcard_t*)(mcmenu + 1);
	mcmenu->opaque = memcard;

	sub_801A03D4(mcmenu->mcard, mcmenu, mcmenu->params.directory, &mcmenu->params.nfiles);
}

int MCARDX_set_buffer(mcmenu_t* mcmenu, void* buffer, int nbytes)
{
	mcmenu->params.buffer = buffer;
	mcmenu->params.nbytes = nbytes;
	return sub_801A1ADC(buffer, mcmenu->params.nblocks);
}

void MCARDX_begin()
{
	///mainTrackerX.mainState = 4;//Dirty hack for now.
}

int MCARDX_main(mcmenu_t* mcmenu, int index)
{
	return 0;
}