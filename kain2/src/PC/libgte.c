#include "LIBGTE.H"
#include <stdlib.h>
#include <string.h>

#define FIXROT(x)	((unsigned)x) & 0xfff

typedef signed __int64 s64;
typedef unsigned __int64 u64;
typedef signed int s32;

static MATRIX M_id =
{
	ONE,	0,		0,
	0,		ONE,	0,
	0,		0,		ONE,
	0,		0,		0
};

static short rsin_tbl[] =
{
		0,   6,  13,  19,  25,  31,  38,  44,  50,  57,  63,  69,  75,  82,  88,  94,
	  101, 107, 113, 119, 126, 132, 138, 144, 151, 157, 163, 170, 176, 182, 188, 195,
	  201, 207, 214, 220, 226, 232, 239, 245, 251, 257, 264, 270, 276, 283, 289, 295,
	  301, 308, 314, 320, 326, 333, 339, 345, 351, 358, 364, 370, 376, 383, 389, 395,
	  401, 408, 414, 420, 426, 433, 439, 445, 451, 458, 464, 470, 476, 483, 489, 495,
	  501, 508, 514, 520, 526, 533, 539, 545, 551, 557, 564, 570, 576, 582, 589, 595,
	  601, 607, 613, 620, 626, 632, 638, 644, 651, 657, 663, 669, 675, 682, 688, 694,
	  700, 706, 713, 719, 725, 731, 737, 744, 750, 756, 762, 768, 774, 781, 787, 793,
	  799, 805, 811, 818, 824, 830, 836, 842, 848, 854, 861, 867, 873, 879, 885, 891,
	  897, 904, 910, 916, 922, 928, 934, 940, 946, 953, 959, 965, 971, 977, 983, 989,
	  995,1001,1007,1014,1020,1026,1032,1038,1044,1050,1056,1062,1068,1074,1080,1086,
	 1092,1099,1105,1111,1117,1123,1129,1135,1141,1147,1153,1159,1165,1171,1177,1183,
	 1189,1195,1201,1207,1213,1219,1225,1231,1237,1243,1249,1255,1261,1267,1273,1279,
	 1285,1291,1297,1303,1309,1315,1321,1327,1332,1338,1344,1350,1356,1362,1368,1374,
	 1380,1386,1392,1398,1404,1409,1415,1421,1427,1433,1439,1445,1451,1457,1462,1468,
	 1474,1480,1486,1492,1498,1503,1509,1515,1521,1527,1533,1538,1544,1550,1556,1562,
	 1567,1573,1579,1585,1591,1596,1602,1608,1614,1620,1625,1631,1637,1643,1648,1654,
	 1660,1666,1671,1677,1683,1689,1694,1700,1706,1711,1717,1723,1729,1734,1740,1746,
	 1751,1757,1763,1768,1774,1780,1785,1791,1797,1802,1808,1813,1819,1825,1830,1836,
	 1842,1847,1853,1858,1864,1870,1875,1881,1886,1892,1898,1903,1909,1914,1920,1925,
	 1931,1936,1942,1947,1953,1958,1964,1970,1975,1981,1986,1992,1997,2002,2008,2013,
	 2019,2024,2030,2035,2041,2046,2052,2057,2062,2068,2073,2079,2084,2090,2095,2100,
	 2106,2111,2117,2122,2127,2133,2138,2143,2149,2154,2159,2165,2170,2175,2181,2186,
	 2191,2197,2202,2207,2213,2218,2223,2228,2234,2239,2244,2249,2255,2260,2265,2270,
	 2276,2281,2286,2291,2296,2302,2307,2312,2317,2322,2328,2333,2338,2343,2348,2353,
	 2359,2364,2369,2374,2379,2384,2389,2394,2399,2405,2410,2415,2420,2425,2430,2435,
	 2440,2445,2450,2455,2460,2465,2470,2475,2480,2485,2490,2495,2500,2505,2510,2515,
	 2520,2525,2530,2535,2540,2545,2550,2555,2559,2564,2569,2574,2579,2584,2589,2594,
	 2598,2603,2608,2613,2618,2623,2628,2632,2637,2642,2647,2652,2656,2661,2666,2671,
	 2675,2680,2685,2690,2694,2699,2704,2709,2713,2718,2723,2727,2732,2737,2741,2746,
	 2751,2755,2760,2765,2769,2774,2779,2783,2788,2792,2797,2802,2806,2811,2815,2820,
	 2824,2829,2833,2838,2843,2847,2852,2856,2861,2865,2870,2874,2878,2883,2887,2892,
	 2896,2901,2905,2910,2914,2918,2923,2927,2932,2936,2940,2945,2949,2953,2958,2962,
	 2967,2971,2975,2979,2984,2988,2992,2997,3001,3005,3009,3014,3018,3022,3026,3031,
	 3035,3039,3043,3048,3052,3056,3060,3064,3068,3073,3077,3081,3085,3089,3093,3097,
	 3102,3106,3110,3114,3118,3122,3126,3130,3134,3138,3142,3146,3150,3154,3158,3162,
	 3166,3170,3174,3178,3182,3186,3190,3194,3198,3202,3206,3210,3214,3217,3221,3225,
	 3229,3233,3237,3241,3244,3248,3252,3256,3260,3264,3267,3271,3275,3279,3282,3286,
	 3290,3294,3297,3301,3305,3309,3312,3316,3320,3323,3327,3331,3334,3338,3342,3345,
	 3349,3352,3356,3360,3363,3367,3370,3374,3378,3381,3385,3388,3392,3395,3399,3402,
	 3406,3409,3413,3416,3420,3423,3426,3430,3433,3437,3440,3444,3447,3450,3454,3457,
	 3461,3464,3467,3471,3474,3477,3481,3484,3487,3490,3494,3497,3500,3504,3507,3510,
	 3513,3516,3520,3523,3526,3529,3532,3536,3539,3542,3545,3548,3551,3555,3558,3561,
	 3564,3567,3570,3573,3576,3579,3582,3585,3588,3591,3594,3597,3600,3603,3606,3609,
	 3612,3615,3618,3621,3624,3627,3630,3633,3636,3639,3642,3644,3647,3650,3653,3656,
	 3659,3661,3664,3667,3670,3673,3675,3678,3681,3684,3686,3689,3692,3695,3697,3700,
	 3703,3705,3708,3711,3713,3716,3719,3721,3724,3727,3729,3732,3734,3737,3739,3742,
	 3745,3747,3750,3752,3755,3757,3760,3762,3765,3767,3770,3772,3775,3777,3779,3782,
	 3784,3787,3789,3791,3794,3796,3798,3801,3803,3805,3808,3810,3812,3815,3817,3819,
	 3822,3824,3826,3828,3831,3833,3835,3837,3839,3842,3844,3846,3848,3850,3852,3854,
	 3857,3859,3861,3863,3865,3867,3869,3871,3873,3875,3877,3879,3881,3883,3885,3887,
	 3889,3891,3893,3895,3897,3899,3901,3903,3905,3907,3909,3910,3912,3914,3916,3918,
	 3920,3921,3923,3925,3927,3929,3930,3932,3934,3936,3937,3939,3941,3943,3944,3946,
	 3948,3949,3951,3953,3954,3956,3958,3959,3961,3962,3964,3965,3967,3969,3970,3972,
	 3973,3975,3976,3978,3979,3981,3982,3984,3985,3987,3988,3989,3991,3992,3994,3995,
	 3996,3998,3999,4001,4002,4003,4005,4006,4007,4008,4010,4011,4012,4014,4015,4016,
	 4017,4019,4020,4021,4022,4023,4024,4026,4027,4028,4029,4030,4031,4032,4034,4035,
	 4036,4037,4038,4039,4040,4041,4042,4043,4044,4045,4046,4047,4048,4049,4050,4051,
	 4052,4053,4053,4054,4055,4056,4057,4058,4059,4060,4060,4061,4062,4063,4064,4064,
	 4065,4066,4067,4067,4068,4069,4070,4070,4071,4072,4072,4073,4074,4074,4075,4076,
	 4076,4077,4077,4078,4079,4079,4080,4080,4081,4081,4082,4082,4083,4083,4084,4084,
	 4085,4085,4086,4086,4087,4087,4088,4088,4088,4089,4089,4089,4090,4090,4090,4091,
	 4091,4091,4092,4092,4092,4092,4093,4093,4093,4093,4094,4094,4094,4094,4094,4095,
	 4095,4095,4095,4095,4095,4095,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,
	 4096
};

//0001:00028de0       _gte_RTPS                  00429de0 f   libgte.obj
//0001:00029140       _gte_RTPT                  0042a140 f   libgte.obj
//0001:00029490       _gte_MVMVA                 0042a490 f   libgte.obj
//0001:000297d0       _gte_DPCL                  0042a7d0 f   libgte.obj
//0001:000299e0       _gte_DPCS                  0042a9e0 f   libgte.obj
//0001:00029bd0       _gte_DPCT                  0042abd0 f   libgte.obj
//0001:00029de0       _gte_INTPL                 0042ade0 f   libgte.obj
//0001:00029fd0       _gte_SQR                   0042afd0 f   libgte.obj
//0001:0002a0a0       _gte_NCS                   0042b0a0 f   libgte.obj
//0001:0002a340       _gte_NCT                   0042b340 f   libgte.obj
//0001:0002a630       _gte_NCDS                  0042b630 f   libgte.obj
//0001:0002a9f0       _gte_NCDT                  0042b9f0 f   libgte.obj
//0001:0002ae50       _gte_NCCS                  0042be50 f   libgte.obj
//0001:0002b190       _gte_NCCT                  0042c190 f   libgte.obj
//0001:0002b530       _gte_CDP                   0042c530 f   libgte.obj
//0001:0002b820       _gte_CC                    0042c820 f   libgte.obj
//0001:0002ba70       _gte_NCLIP                 0042ca70 f   libgte.obj
//0001:0002bae0       _gte_AVSZ3                 0042cae0 f   libgte.obj
//0001:0002bb30       _gte_AVSZ4                 0042cb30 f   libgte.obj
//0001:0002bb90       _gte_OP                    0042cb90 f   libgte.obj
//0001:0002bce0       _gte_GPF                   0042cce0 f   libgte.obj
//0001:0002bee0       _gte_GPL                   0042cee0 f   libgte.obj
//0001:0002c110       _gte_ldv0                  0042d110 f   libgte.obj
//0001:0002c130       _gte_ldv1                  0042d130 f   libgte.obj
//0001:0002c150       _gte_ldv2                  0042d150 f   libgte.obj
//0001:0002c170       _gte_ldv3                  0042d170 f   libgte.obj
//0001:0002c1b0       _gte_ldv3c                 0042d1b0 f   libgte.obj
//0001:0002c1f0       _gte_ldv3c_vertc           0042d1f0 f   libgte.obj
//0001:0002c230       _gte_ldv01                 0042d230 f   libgte.obj
//0001:0002c260       _gte_ldv01c                0042d260 f   libgte.obj
//0001:0002c290       _gte_ldrgb                 0042d290 f   libgte.obj
//0001:0002c2a0       _gte_ldrgb3                0042d2a0 f   libgte.obj
//0001:0002c2d0       _gte_ldrgb3c               0042d2d0 f   libgte.obj
//0001:0002c300       _gte_ldlv0                 0042d300 f   libgte.obj
//0001:0002c330       _gte_ldlvl                 0042d330 f   libgte.obj
//0001:0002c350       _gte_ldsv                  0042d350 f   libgte.obj
//0001:0002c380       _gte_ldbv                  0042d380 f   libgte.obj
//0001:0002c3a0       _gte_ldcv                  0042d3a0 f   libgte.obj
//0001:0002c3d0       _gte_ldclmv                0042d3d0 f   libgte.obj
//0001:0002c400       _gte_lddp                  0042d400 f   libgte.obj
//0001:0002c410       _gte_ldsxy0                0042d410 f   libgte.obj
//0001:0002c420       _gte_ldsxy1                0042d420 f   libgte.obj
//0001:0002c430       _gte_ldsxy2                0042d430 f   libgte.obj
//0001:0002c440       _gte_ldsxy3                0042d440 f   libgte.obj
//0001:0002c460       _gte_ldsxy3c               0042d460 f   libgte.obj
//0001:0002c480       _gte_ldsz3                 0042d480 f   libgte.obj
//0001:0002c4a0       _gte_ldsz4                 0042d4a0 f   libgte.obj
//0001:0002c4d0       _gte_ldopv1                0042d4d0 f   libgte.obj
//0001:0002c4f0       _gte_ldopv2                0042d4f0 f   libgte.obj
//0001:0002c510       _gte_ldlzc                 0042d510 f   libgte.obj
//0001:0002c520       _gte_ldbkdir               0042d520 f   libgte.obj
//0001:0002c540       _gte_ldfcdir               0042d540 f   libgte.obj
//0001:0002c560       _gte_ldsvrtrow0            0042d560 f   libgte.obj
//0001:0002c580       _gte_ldsvllrow0            0042d580 f   libgte.obj
//0001:0002c5a0       _gte_ldsvlcrow0            0042d5a0 f   libgte.obj
//0001:0002c5c0       _gte_ldtr                  0042d5c0 f   libgte.obj
//0001:0002c5e0       _gte_ld_intpol_uv0         0042d5e0 f   libgte.obj
//0001:0002c600       _gte_ld_intpol_uv1         0042d600 f   libgte.obj
//0001:0002c620       _gte_ld_intpol_bv0         0042d620 f   libgte.obj
//0001:0002c640       _gte_ld_intpol_bv1         0042d640 f   libgte.obj
//0001:0002c660       _gte_ld_intpol_sv0         0042d660 f   libgte.obj
//0001:0002c690       _gte_ld_intpol_sv1         0042d690 f   libgte.obj
//0001:0002c6c0       _gte_ldfc                  0042d6c0 f   libgte.obj
//0001:0002c6e0       _gte_ldopv2SV              0042d6e0 f   libgte.obj
//0001:0002c710       _gte_ldopv1SV              0042d710 f   libgte.obj
//0001:0002c740       _gte_stsxy                 0042d740 f   libgte.obj
//0001:0002c750       _gte_stsxy3                0042d750 f   libgte.obj
//0001:0002c780       _gte_stsxy3c               0042d780 f   libgte.obj
//0001:0002c7a0       _gte_stsxy2                0042d7a0 f   libgte.obj
//0001:0002c7b0       _gte_stsxy1                0042d7b0 f   libgte.obj
//0001:0002c7c0       _gte_stsxy0                0042d7c0 f   libgte.obj
//0001:0002c7d0       _gte_stsxy01               0042d7d0 f   libgte.obj
//0001:0002c7f0       _gte_stsxy01c              0042d7f0 f   libgte.obj
//0001:0002c810       _gte_stsxy3_f3             0042d810 f   libgte.obj
//0001:0002c830       _gte_stsxy3_g3             0042d830 f   libgte.obj
//0001:0002c850       _gte_stsxy3_ft3            0042d850 f   libgte.obj
//0001:0002c870       _gte_stsxy3_gt3            0042d870 f   libgte.obj
//0001:0002c890       _gte_stsxy3_f4             0042d890 f   libgte.obj
//0001:0002c8b0       _gte_stsxy3_g4             0042d8b0 f   libgte.obj
//0001:0002c8d0       _gte_stsxy3_ft4            0042d8d0 f   libgte.obj
//0001:0002c8f0       _gte_stsxy3_gt4            0042d8f0 f   libgte.obj
//0001:0002c910       _gte_stdp                  0042d910 f   libgte.obj
//0001:0002c920       _gte_stflg                 0042d920 f   libgte.obj
//0001:0002c930       _gte_stflg_4               0042d930 f   libgte.obj
//0001:0002c950       _gte_stsz                  0042d950 f   libgte.obj
//0001:0002c960       _gte_stsz3                 0042d960 f   libgte.obj
//0001:0002c990       _gte_stsz4                 0042d990 f   libgte.obj
//0001:0002c9c0       _gte_stsz3c                0042d9c0 f   libgte.obj
//0001:0002c9e0       _gte_stsz4c                0042d9e0 f   libgte.obj
//0001:0002ca10       _gte_stszotz               0042da10 f   libgte.obj
//0001:0002ca20       _gte_stotz                 0042da20 f   libgte.obj
//0001:0002ca30       _gte_stopz                 0042da30 f   libgte.obj
//0001:0002ca40       _gte_stlvl                 0042da40 f   libgte.obj
//0001:0002ca60       _gte_stlvnl                0042da60 f   libgte.obj
//0001:0002ca80       _gte_stlvnl0               0042da80 f   libgte.obj
//0001:0002ca90       _gte_stlvnl1               0042da90 f   libgte.obj
//0001:0002caa0       _gte_stlvnl2               0042daa0 f   libgte.obj
//0001:0002cab0       _gte_stsv                  0042dab0 f   libgte.obj
//0001:0002cae0       _gte_stclmv                0042dae0 f   libgte.obj
//0001:0002cb10       _gte_stbv                  0042db10 f   libgte.obj
//0001:0002cb30       _gte_stcv                  0042db30 f   libgte.obj
//0001:0002cb50       _gte_strgb                 0042db50 f   libgte.obj
//0001:0002cb60       _gte_strgb3                0042db60 f   libgte.obj
//0001:0002cb90       _gte_strgb3_g3             0042db90 f   libgte.obj
//0001:0002cbb0       _gte_strgb3_gt3            0042dbb0 f   libgte.obj
//0001:0002cbd0       _gte_strgb3_g4             0042dbd0 f   libgte.obj
//0001:0002cbf0       _gte_strgb3_gt4            0042dbf0 f   libgte.obj
//0001:0002cc10       _gte_sttr                  0042dc10 f   libgte.obj
//0001:0002cc30       _gte_stlzc                 0042dc30 f   libgte.obj
//0001:0002cc40       _gte_stfc                  0042dc40 f   libgte.obj
//0001:0002cc60       _gte_mvlvtr                0042dc60 f   libgte.obj
//0001:0002cc90       _gte_nop                   0042dc90 f   libgte.obj
//0001:0002cca0       _gte_subdvl                0042dca0 f   libgte.obj
//0001:0002cce0       _gte_subdvd                0042dce0 f   libgte.obj
//0001:0002cd20       _gte_adddvl                0042dd20 f   libgte.obj
//0001:0002cd60       _gte_adddvd                0042dd60 f   libgte.obj
//0001:0002cda0       _ApplyMatrix               0042dda0 f   libgte.obj
VECTOR* ApplyMatrix(MATRIX* m, SVECTOR* v0, VECTOR* v1)
{
	v1->vx = (v0->vx * m->m[0][0] + v0->vy * m->m[0][1] + v0->vz * m->m[0][2]) >> 12;
	v1->vy = (v0->vx * m->m[1][0] + v0->vy * m->m[1][1] + v0->vz * m->m[1][2]) >> 12;
	v1->vz = (v0->vx * m->m[2][0] + v0->vy * m->m[2][1] + v0->vz * m->m[2][2]) >> 12;
	return v1;
}
//0001:0002ce20       _ApplyMatrixLV             0042de20 f   libgte.obj
VECTOR* ApplyMatrixLV(MATRIX* m, VECTOR* v0, VECTOR* v1)
{
	s64 vx = ((s64)v0->vx * (s64)m->m[0][0] + (s64)v0->vy * (s64)m->m[0][1] + (s64)v0->vz * (s64)m->m[0][2]),
		vy = ((s64)v0->vx * (s64)m->m[1][0] + (s64)v0->vy * (s64)m->m[1][1] + (s64)v0->vz * (s64)m->m[1][2]),
		vz = ((s64)v0->vx * (s64)m->m[2][0] + (s64)v0->vy * (s64)m->m[2][1] + (s64)v0->vz * (s64)m->m[2][2]);
	v1->vx = (s32)(vx >> 12);
	v1->vy = (s32)(vy >> 12);
	v1->vz = (s32)(vz >> 12);
	return v1;
}
//0001:0002cea0       _ApplyMatrixSV             0042dea0 f   libgte.obj
SVECTOR* ApplyMatrixSV(MATRIX* m, SVECTOR* v0, SVECTOR* v1)
{
	v1->vx = (v0->vx * m->m[0][0] + v0->vy * m->m[0][1] + v0->vz * m->m[0][2]) >> 12;
	v1->vy = (v0->vx * m->m[1][0] + v0->vy * m->m[1][1] + v0->vz * m->m[1][2]) >> 12;
	v1->vz = (v0->vx * m->m[2][0] + v0->vy * m->m[2][1] + v0->vz * m->m[2][2]) >> 12;
	return v1;
}
//0001:0002cf20       _ApplyRotMatrix            0042df20 f   libgte.obj
//0001:0002cf70       _ApplyRotMatrixLV          0042df70 f   libgte.obj
//0001:0002cfc0       _AverageZ3                 0042dfc0 f   libgte.obj
//0001:0002d020       _AverageZ4                 0042e020 f   libgte.obj
//0001:0002d090       _ColorCol                  0042e090 f   libgte.obj
//0001:0002d0d0       _ColorDpq                  0042e0d0 f   libgte.obj
//0001:0002d120       _CompMatrix                0042e120 f   libgte.obj
MATRIX* CompMatrix(MATRIX* m0, MATRIX* m1, MATRIX* m2)
{
	MATRIX m = { 0 };

	MulMatrix0(m0, m1, &m);
	ApplyMatrixLV(m0, (VECTOR*)m1->t, (VECTOR*)m.t);
	m.t[0] += m0->t[0];
	m.t[1] += m0->t[1];
	m.t[2] += m0->t[2];
	memcpy(m2, &m, sizeof(MATRIX));

	return m2;
}
//0001:0002d2b0       _CompMatrixLV              0042e2b0 f   libgte.obj
//0001:0002d440       _DpqColor                  0042e440 f   libgte.obj
//0001:0002d470       _DpqColor3                 0042e470 f   libgte.obj
//0001:0002d4d0       _DpqColorLight             0042e4d0 f   libgte.obj
//0001:0002d520       _FlipRotMatrixX            0042e520 f   libgte.obj
//0001:0002d550       _FlipTRX                   0042e550 f   libgte.obj
//0001:0002d560       _InitGeom                  0042e560 f   libgte.obj
//0001:0002d580       _Intpl                     0042e580 f   libgte.obj
//0001:0002d5c0       _LightColor                0042e5c0 f   libgte.obj
//0001:0002d610       _LoadAverage0              0042e610 f   libgte.obj
//0001:0002d6a0       _LoadAverage12             0042e6a0 f   libgte.obj
//0001:0002d730       _LoadAverageByte           0042e730 f   libgte.obj
//0001:0002d7a0       _LoadAverageCol            0042e7a0 f   libgte.obj
//0001:0002d830       _LoadAverageShort0         0042e830 f   libgte.obj
//0001:0002d8c0       _LoadAverageShort12        0042e8c0 f   libgte.obj
//0001:0002d950       _LocalLight                0042e950 f   libgte.obj
//0001:0002d9a0       _Lzc                       0042e9a0 f   libgte.obj
//0001:0002d9b0       _MulMatrix                 0042e9b0 f   libgte.obj
MATRIX* MulMatrix(MATRIX* m0, MATRIX* m1)
{
	return MulMatrix0(m0, m1, m0);
}
//0001:0002dae0       _MulMatrix0                0042eae0 f   libgte.obj
MATRIX* MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2)
{
	MATRIX temp = { 0 };

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				temp.m[i][j] += (m0->m[i][k] * m1->m[k][j]) >> 12;

	memcpy(m2, &temp, sizeof(temp) - 12);

	return m2;
}
//0001:0002dc10       _MulMatrix2                0042ec10 f   libgte.obj
MATRIX* MulMatrix2(MATRIX* m0, MATRIX* m1)
{
	return MulMatrix0(m0, m1, m1);
}
//0001:0002dd40       _MulRotMatrix              0042ed40 f   libgte.obj
//0001:0002de40       _MulRotMatrix0             0042ee40 f   libgte.obj
//0001:0002df40       _NormalClip                0042ef40 f   libgte.obj
//0001:0002dfb0       _NormalColor               0042efb0 f   libgte.obj
//0001:0002dfe0       _NormalColor3              0042efe0 f   libgte.obj
//0001:0002e050       _NormalColorCol            0042f050 f   libgte.obj
//0001:0002e090       _NormalColorCol3           0042f090 f   libgte.obj
//0001:0002e110       _NormalColorDpq            0042f110 f   libgte.obj
//0001:0002e150       _NormalColorDpq3           0042f150 f   libgte.obj
//0001:0002e1d0       _OuterProduct0             0042f1d0 f   libgte.obj
//0001:0002e240       _OuterProduct0SV           0042f240 f   libgte.obj
//0001:0002e2b0       _OuterProduct0SVL          0042f2b0 f   libgte.obj
//0001:0002e320       _OuterProduct12            0042f320 f   libgte.obj
//0001:0002e390       _OuterProduct12SV          0042f390 f   libgte.obj
//0001:0002e400       _OuterProduct12SVL         0042f400 f   libgte.obj
//0001:0002e470       _PopMatrix                 0042f470 f   libgte.obj
//0001:0002e4c0       _PushMatrix                0042f4c0 f   libgte.obj
//0001:0002e530       _ratan2                    0042f530 f   libgte.obj
//0001:0002e5f0       _rcos                      0042f5f0 f   libgte.obj
int rcos(int r)
{
	return rsin(r + 1024);
}
//0001:0002e650       _ReadColorMatrix           0042f650 f   libgte.obj
//0001:0002e6a0       _ReadGeomOffset            0042f6a0 f   libgte.obj
//0001:0002e6c0       _ReadGeomScreen            0042f6c0 f   libgte.obj
//0001:0002e6d0       _ReadLightMatrix           0042f6d0 f   libgte.obj
//0001:0002e720       _ReadRotMatrix             0042f720 f   libgte.obj
//0001:0002e770       _RotAverage3               0042f770 f   libgte.obj
//0001:0002e840       _RotAverageNclip3          0042f840 f   libgte.obj
//0001:0002e970       _RotAverageNclipColorCol3  0042f970 f   libgte.obj
//0001:0002eb10       _RotAverageNclipColorDpq3  0042fb10 f   libgte.obj
//0001:0002ecb0       _RotColorDpq               0042fcb0 f   libgte.obj
//0001:0002ed20       _RotColorDpq3              0042fd20 f   libgte.obj
//0001:0002ee10       _RotMatrix                 0042fe10 f   libgte.obj
MATRIX* RotMatrix(SVECTOR* r, MATRIX* m)
{
	memcpy(m, &M_id, sizeof(m->m));
	RotMatrixZ(r->vz, m);
	RotMatrixY(r->vy, m);
	RotMatrixX(r->vx, m);

	return m;
}
//0001:0002f160       _RotMatrix_gte             00430160 f   libgte.obj
MATRIX* RotMatrix_gte(SVECTOR* r, MATRIX* m)
{
	return RotMatrix(r, m);
}
//0001:0002f3f0       _RotMatrixC                004303f0 f   libgte.obj
MATRIX* RotMatrixC(SVECTOR* r, MATRIX* m)
{
	return RotMatrix(r, m);
}
//0001:0002f680       _RotMatrixX                00430680 f   libgte.obj
MATRIX* RotMatrixX(long angle, MATRIX* m)
{
	MATRIX m0;

	angle = FIXROT(angle);

	int sx = rsin(angle);
	int cx = rcos(angle);
	m0.m[0][0] = ONE, m0.m[0][1] = 0, m0.m[0][2] = 0;
	m0.m[1][0] = 0, m0.m[1][1] = cx, m0.m[1][2] = -sx;
	m0.m[2][0] = 0, m0.m[2][1] = sx, m0.m[2][2] = cx;
	m0.t[0] = 0, m0.t[1] = 0, m0.t[2] = 0;
	MulMatrix0(&m0, m, m);

	return m;
}
//0001:0002f880       _RotMatrixY                00430880 f   libgte.obj
MATRIX* RotMatrixY(long angle, MATRIX* m)
{
	MATRIX m0;

	angle = FIXROT(angle);

	int sy = rsin(angle);
	int cy = rcos(angle);
	m0.m[0][0] = cy, m0.m[0][1] = 0, m0.m[0][2] = sy;
	m0.m[1][0] = 0, m0.m[1][1] = ONE, m0.m[1][2] = 0;
	m0.m[2][0] = -sy, m0.m[2][1] = 0, m0.m[2][2] = cy;
	m0.t[0] = 0, m0.t[1] = 0, m0.t[2] = 0;
	MulMatrix0(&m0, m, m);

	return m;
}
//0001:0002fa80       _RotMatrixYXZ              00430a80 f   libgte.obj
MATRIX* RotMatrixYXZ(SVECTOR* r, MATRIX* m)
{
	memcpy(m, &M_id, sizeof(m->m));
	RotMatrixZ(r->vz, m);
	RotMatrixX(r->vx, m);
	RotMatrixY(r->vy, m);

	return m;
}
//0001:0002fdd0       _RotMatrixYXZ_gte          00430dd0 f   libgte.obj
MATRIX* RotMatrixYXZ_gte(SVECTOR* r, MATRIX* m)
{
	return RotMatrixYXZ(r, m);
}
//0001:00030060       _RotMatrixZ                00431060 f   libgte.obj
MATRIX* RotMatrixZ(long angle, MATRIX* m)
{
	MATRIX m0;

	angle = FIXROT(angle);

	int sz = rsin(angle);
	int cz = rcos(angle);
	m0.m[0][0] = cz, m0.m[0][1] = -sz, m0.m[0][2] = 0;
	m0.m[1][0] = sz, m0.m[1][1] = cz, m0.m[1][2] = 0;
	m0.m[2][0] = 0, m0.m[2][1] = 0, m0.m[2][2] = ONE;
	m0.t[0] = 0, m0.t[1] = 0, m0.t[2] = 0;
	MulMatrix0(&m0, m, m);

	return m;
}
//0001:00030260       _RotMatrixZYX              00431260 f   libgte.obj
MATRIX* RotMatrixZYX(SVECTOR* r, MATRIX* m)
{
	memcpy(m, &M_id, sizeof(m->m));
	RotMatrixX(r->vz, m);
	RotMatrixY(r->vx, m);
	RotMatrixZ(r->vy, m);

	return m;
}
//0001:000305b0       _RotMatrixZYX_C            004315b0 f   libgte.obj
MATRIX* RotMatrixZYX_C(SVECTOR* r, MATRIX* m)
{
	return RotMatrixYXZ(r, m);
}
//0001:00030900       _RotMatrixZYX_gte          00431900 f   libgte.obj
MATRIX* RotMatrixZYX_gte(SVECTOR* r, MATRIX* m)
{
	return RotMatrixYXZ(r, m);
}
//0001:00030b90       _RotNclip3                 00431b90 f   libgte.obj
//0001:00030c80       _RotTrans                  00431c80 f   libgte.obj
//0001:00030ce0       _RotTransPers              00431ce0 f   libgte.obj
//0001:00030d30       _RotTransPers3             00431d30 f   libgte.obj
//0001:00030dc0       _RotTransSV                00431dc0 f   libgte.obj
//0001:00030e20       _rsin                      00431e20 f   libgte.obj
int rsin(int r)
{
	// flip negative values
	if (r < 0) r = -r;
	// wrap value to 0-4095 max
	r &= 0xFFF;

	switch (r >> 10)
	{
	case 0: return rsin_tbl[r & 0x3ff];		// 0-1023
	case 1: return rsin_tbl[2048 - r];		// 1024-2047
	case 2: return -rsin_tbl[r - 2048];		// 2048-3071
	}

	return -rsin_tbl[4096 - r];	// 3072-4095
}
//0001:00030e80       _ScaleMatrix               00431e80 f   libgte.obj
MATRIX* ScaleMatrix(MATRIX* m, VECTOR* v)
{
	for (int i = 0; i < 3; i++)
	{
		m->m[i][0] = (short)(((long)m->m[i][0] * v->vx) >> 12);
		m->m[i][1] = (short)(((long)m->m[i][1] * v->vy) >> 12);
		m->m[i][2] = (short)(((long)m->m[i][2] * v->vz) >> 12);
	}

	return m;
}
//0001:00030f20       _SetBackColor              00431f20 f   libgte.obj
//0001:00030f50       _SetColorMatrix            00431f50 f   libgte.obj
//0001:00030f80       _SetDQA                    00431f80 f   libgte.obj
//0001:00030f90       _SetDQB                    00431f90 f   libgte.obj
//0001:00030fa0       _SetFarColor               00431fa0 f   libgte.obj
//0001:00030fd0       _SetFogFar                 00431fd0 f   libgte.obj
//0001:00030fe0       _SetFogNear                00431fe0 f   libgte.obj
//0001:00030ff0       _SetFogNearFar             00431ff0 f   libgte.obj
//0001:00031050       _SetGeomOffset             00432050 f   libgte.obj
//0001:00031070       _SetGeomScreen             00432070 f   libgte.obj
//0001:00031080       _SetLightMatrix            00432080 f   libgte.obj
//0001:000310b0       _SetRGBcd                  004320b0 f   libgte.obj
//0001:000310c0       _SetRotMatrix              004320c0 f   libgte.obj
//0001:000310f0       _SetTransMatrix            004320f0 f   libgte.obj
//0001:00031110       _SetTransVector            00432110 f   libgte.obj
//0001:00031130       _Square0                   00432130 f   libgte.obj
//0001:000311f0       _Square12                  004321f0 f   libgte.obj
//0001:000312c0       _TransMatrix               004322c0 f   libgte.obj
//0001:000312e0       _TransposeMatrix           004322e0 f   libgte.obj
//0001:00031330       _gte_ldsvrtrow2            00432330 f   libgte.obj
//0001:00031350       _gte_stv0                  00432350 f   libgte.obj
//0001:00031370       _gte_stv1                  00432370 f   libgte.obj
//0001:00031390       _gte_stv2                  00432390 f   libgte.obj
//0001:000313b0       _gte_lddqa                 004323b0 f   libgte.obj
//0001:000313c0       _gte_lddqb                 004323c0 f   libgte.obj
//0001:000313d0       _gte_lddqab                004323d0 f   libgte.obj
//0001:000313f0       _gte_stmac0                004323f0 f   libgte.obj