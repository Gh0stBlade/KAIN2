#include "Game/CORE.H"
#include "WORSHIP.H"

struct _MonsterFunctionTable WORSHIP_FunctionTable;

void WORSHIP_Init(struct _Instance* instance) // Matching - 98.30%
{
    struct _MonsterAttributes* ma;
    struct _MonsterVars* mv;
    struct _Instance* weapon;

    short var_v0;

    mv = (struct _MonsterVars*)instance->extraData;
    ma = (struct _MonsterAttributes*)instance->data;
    if (mv->age == 0)
    {
        weapon = HUMAN_CreateWeapon(instance, 12, ma->rightWeaponSegment);
        if (weapon)
        {
            HUMAN_Init(weapon);
        }
    }
    else
    {
        HUMAN_CreateWeapon(instance, 11, ma->leftWeaponSegment);
        weapon = HUMAN_CreateWeapon(instance, 11, ma->rightWeaponSegment);
        if (weapon)
        {
            HUMAN_Init(weapon);
        }
    }
    if (mv->age == 0)
    {
        mv->soulJuice = 20480;
    }
    else
    {
        mv->soulJuice = 24576;
    }
}

void WORSHIP_CombatEntry(struct _Instance* instance) // Matching - 100%
{
    struct _MonsterVars* mv;
    struct _MonsterAttributes* ma;
    struct _Instance* child; // Not exist in sysdump

    mv = (struct _MonsterVars*)instance->extraData;
    if (mv->enemy && mv->enemy->distance < mv->subAttr->combatAttributes->combatRange && mv->age == 1)
    {
        ma = (struct _MonsterAttributes*)instance->data;
        child = instance->LinkChild;
        if (child != NULL) {
            if ((child->LinkSibling == 0) && (child->ParentLinkNode == ma->rightWeaponSegment))
            {
                INSTANCE_UnlinkFromParent(child);
                INSTANCE_LinkToParent(child, instance, ma->leftWeaponSegment);
                mv->mvFlags |= 0x20;
            }
        }
        else
        {
            HUMAN_CreateWeapon(instance, 11, ma->leftWeaponSegment);
            mv->mvFlags |= 0x20;
        }
    }
    MON_CombatEntry(instance);
}
