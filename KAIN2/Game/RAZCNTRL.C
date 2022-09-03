#include "CORE.H"
#include "RAZCNTRL.H"


void ProcessRazControl(long *command)
{
#if defined(PSX_VERSION)

#if 0
	sub_80070C08:
	lw      $v0, 0($a0)
		lw      $v1, -0x5B74($gp)
		nop
		and $v0, $v1
		beqz    $v0, loc_80070C38
		nop
		lw      $v0, -0x5B4C($gp)
		sw      $zero, -0x5B50($gp)
		addiu   $v0, 1
		sw      $v0, -0x5B4C($gp)
		j       loc_80070C58
		nop

		loc_80070C38 :
	lw      $v0, -0x5B50($gp)
		lw      $v1, -0x5B4C($gp)
		addiu   $v0, 1
		sw      $v0, -0x5B50($gp)
		beqz    $v1, loc_80070C54
		nop
		sw      $v1, -0x5B54($gp)

		loc_80070C54:
	sw      $zero, -0x5B4C($gp)

		loc_80070C58 :
		lw      $v1, -0x5B4C($gp)
		nop
		slti    $v0, $v1, 6
		bnez    $v0, loc_80070C70
		nop
		sw      $v1, -0x5B54($gp)

		loc_80070C70:
	lw      $v0, -0x5B50($gp)
		nop
		slti    $v0, 3
		bnez    $v0, locret_80070C88
		nop
		sw      $zero, -0x5B54($gp)

		locret_80070C88:
	jr      $ra
		nop
#endif

#elif defined(PC_VERSION)
	int v1; // eax

	if ((dword_4FAD98 & *command) != 0)
	{
		dword_C55180 = 0;
		v1 = ++dword_C55184;
	}
	else
	{
		++dword_C55180;
		if (dword_C55184)
			dword_C5517C = dword_C55184;
		v1 = 0;
		dword_C55184 = 0;
	}
	if (v1 > 5)
		dword_C5517C = v1;
	if (dword_C55180 > 2)
		dword_C5517C = 0;
#endif
}
