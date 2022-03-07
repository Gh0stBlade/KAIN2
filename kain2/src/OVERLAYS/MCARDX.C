#include "CORE.H"
#include "MCARDX.H"

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

void MCARDX_initialize(mcmenu_t* mcmenu, memcard_t* memcard, int nBlocks)
{
	memset(mcmenu, 0, sizeof(mcmenu_t));

	mcmenu->state.status = mcard_status_new_card;
	mcmenu->state.fsm = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;
	mcmenu->state.running = 0;
	mcmenu->params.nblocks = nBlocks;
	mcmenu->mcard = (mcard_t*)(mcmenu + 1);
	mcmenu->opaque = memcard;

	sub_801A03D4(mcmenu->mcard, mcmenu, mcmenu->params.directory, &mcmenu->params.nfiles);
}