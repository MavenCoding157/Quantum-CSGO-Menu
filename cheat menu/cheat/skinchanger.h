enum WeaponIds
{
    WEAPON_DEAGLE = 1,
    WEAPON_ELITE = 2,
    WEAPON_FIVESEVEN = 3,
    WEAPON_GLOCK = 4,
    WEAPON_AK47 = 7,
    WEAPON_AUG = 8,
    WEAPON_AWP = 9,
    WEAPON_FAMAS = 10,
    WEAPON_G3SG1 = 11,
    WEAPON_GALILAR = 13,
    WEAPON_M249 = 14,
    WEAPON_M4A1 = 16,
    WEAPON_MAC10 = 17,
    WEAPON_P90 = 19,
    WEAPON_MP5SD = 23,
    WEAPON_UMP45 = 24,
    WEAPON_XM1014 = 25,
    WEAPON_BIZON = 26,
    WEAPON_MAG7 = 27,
    WEAPON_NEGEV = 28,
    WEAPON_SAWEDOFF = 29,
    WEAPON_TEC9 = 30,
    WEAPON_TASER = 31,
    WEAPON_HKP2000 = 32,
    WEAPON_MP7 = 33,
    WEAPON_MP9 = 34,
    WEAPON_NOVA = 35,
    WEAPON_P250 = 36,
    WEAPON_SHIELD = 37,
    WEAPON_SCAR20 = 38,
    WEAPON_SG556 = 39,
    WEAPON_SSG08 = 40,
    WEAPON_KNIFEGG = 41,
    WEAPON_KNIFE = 42,
    WEAPON_FLASHBANG = 43,
    WEAPON_HEGRENADE = 44,
    WEAPON_SMOKEGRENADE = 45,
    WEAPON_MOLOTOV = 46,
    WEAPON_DECOY = 47,
    WEAPON_INCGRENADE = 48,
    WEAPON_C4 = 49,
    WEAPON_HEALTHSHOT = 57,
    WEAPON_KNIFE_T = 59,
    WEAPON_M4A1_SILENCER = 60,
    WEAPON_USP_SILENCER = 61,
    WEAPON_CZ75A = 63,
    WEAPON_REVOLVER = 64,
    WEAPON_TAGRENADE = 68,
    WEAPON_FISTS = 69,
    WEAPON_BREACHCHARGE = 70,
    WEAPON_TABLET = 72,
    WEAPON_MELEE = 74,
    WEAPON_AXE = 75,
    WEAPON_HAMMER = 76,
    WEAPON_SPANNER = 78,
    WEAPON_KNIFE_GHOST = 80,
    WEAPON_FIREBOMB = 81,
    WEAPON_DIVERSION = 82,
    WEAPON_FRAG_GRENADE = 83,
    WEAPON_SNOWBALL = 84,
    WEAPON_BUMPMINE = 85,
    WEAPON_BAYONET = 500,
    WEAPON_KNIFE_FLIP = 505,
    WEAPON_KNIFE_GUT = 506,
    WEAPON_KNIFE_KARAMBIT = 507,
    WEAPON_KNIFE_M9_BAYONET = 508,
    WEAPON_KNIFE_TACTICAL = 509,
    WEAPON_KNIFE_FALCHION = 512,
    WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
    WEAPON_KNIFE_BUTTERFLY = 515,
    WEAPON_KNIFE_PUSH = 516,
    WEAPON_KNIFE_URSUS = 519,
    WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
    WEAPON_KNIFE_STILETTO = 522,
    WEAPON_KNIFE_WIDOWMAKER = 523,
    STUDDED_BLOODHOUND_GLOVES = 5027,
    T_GLOVES = 5028,
    CT_GLOVES = 5029,
    SPORTY_GLOVES = 5030,
    SLICK_GLOVES = 5031,
    LEATHER_HANDWRAPS = 5032,
    MOTORCYCLE_GLOVES = 5033,
    SPECIALIST_GLOVES = 5034,
    STUDDED_HYDRA_GLOVES = 5035,
};

#include "globals.h"

constexpr const int GetWeaponPaint(const short& itemDefinition)
{
    switch (itemDefinition)
    {
    case WEAPON_DEAGLE: return 711;
    case WEAPON_AWP:

        if (g_Options.awp_skin[0] == 0)
            return 0;

        if (g_Options.awp_skin[0] == 1)
            return 887;

        if (g_Options.awp_skin[0] == 2)
            return 917;

        if (g_Options.awp_skin[0] == 3)
            return 803;

        if (g_Options.awp_skin[0] == 4)
            return 475;

        if (g_Options.awp_skin[0] == 5)
            return 279;

        if (g_Options.awp_skin[0] == 6)
            return 51;

        if (g_Options.awp_skin[0] == 7)
            return 1026;

        if (g_Options.awp_skin[0] == 8)
            return 736;

        if (g_Options.awp_skin[0] == 9)
            return 756;

        if (g_Options.awp_skin[0] == 10)
            return 446;

        if (g_Options.awp_skin[0] == 11)
            return 344;

        if (g_Options.awp_skin[0] == 12)
            return 640;

        if (g_Options.awp_skin[0] == 13)
            return 525;

        if (g_Options.awp_skin[0] == 14)
            return 259;

        if (g_Options.awp_skin[0] == 15)
            return 227;

        if (g_Options.awp_skin[0] == 16)
            return 174;

        if (g_Options.awp_skin[0] == 17)
            return 838;

        if (g_Options.awp_skin[0] == 18)
            return 424;

        if (g_Options.awp_skin[0] == 19)
            return 84;

    case WEAPON_AK47:

        if (g_Options.ak_skin[0] == 0)
            return 0;

        if (g_Options.ak_skin[0] == 1)
            return 707;

        if (g_Options.ak_skin[0] == 2)
            return 639;

        if (g_Options.ak_skin[0] == 3)
            return 380;

        if (g_Options.ak_skin[0] == 4)
            return 524;

        if (g_Options.ak_skin[0] == 5)
            return 302;

        if (g_Options.ak_skin[0] == 6)
            return 490;

        if (g_Options.ak_skin[0] == 7)
            return 506;

        if (g_Options.ak_skin[0] == 8)
            return 282;

        if (g_Options.ak_skin[0] == 9)
            return 836;

        if (g_Options.ak_skin[0] == 10)
            return 422;

    case WEAPON_M4A1:

        if (g_Options.m4_skin[0] == 0)
            return 0;

        if (g_Options.m4_skin[0] == 1)
            return 309;

        if (g_Options.m4_skin[0] == 2)
            return 844;

        if (g_Options.m4_skin[0] == 3)
            return 695;

        if (g_Options.m4_skin[0] == 4)
            return 632;

        if (g_Options.m4_skin[0] == 5)
            return 985;

        if (g_Options.m4_skin[0] == 6)
            return 588;

        if (g_Options.m4_skin[0] == 7)
            return 400;

        if (g_Options.m4_skin[0] == 8)
            return 1041;

        if (g_Options.m4_skin[0] == 9)
            return 255;

        if (g_Options.m4_skin[0] == 10)
            return 1097;

    case WEAPON_M4A1_SILENCER:

        if (g_Options.m4_silencer_skin[0] == 0)
            return 0;

        if (g_Options.m4_silencer_skin[0] == 1)
            return 984;

        if (g_Options.m4_silencer_skin[0] == 2)
            return 946;

        if (g_Options.m4_silencer_skin[0] == 3)
            return 548;

        if (g_Options.m4_silencer_skin[0] == 4)
            return 497;

        if (g_Options.m4_silencer_skin[0] == 5)
            return 430;

        if (g_Options.m4_silencer_skin[0] == 6)
            return 360;

        if (g_Options.m4_silencer_skin[0] == 7)
            return 714;

        if (g_Options.m4_silencer_skin[0] == 8)
            return 681;

        if (g_Options.m4_silencer_skin[0] == 9)
            return 644;

        if (g_Options.m4_silencer_skin[0] == 10)
            return 440;

    case WEAPON_GLOCK: return 586;
    case WEAPON_USP_SILENCER: return 705;
    case WEAPON_SSG08: return 361;
    }
    return 0;
}