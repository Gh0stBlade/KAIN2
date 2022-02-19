#ifndef DRAWS_H
#define DRAWS_H



extern void depthQPTable() ; // 0x800CE174

extern void depthQBlendStart() ; // 0x800CE178

extern void depthQFogStart() ; // 0x800CE17C

extern void depthQFogFar() ; // 0x800CE180

extern void depthQBackColor() ; // 0x800CE184

extern void gameTimer() ; // 0x800CE188

extern void modelFadeValue() ; // 0x800CE18C

extern void lowerSplitPush() ; // 0x800CE190

extern void* overlayAddress; // 0x800CE194

extern void forceDrawInFront() ; // 0x800CE198

extern void ClearBss() ; // 0x80024E4C

extern void clearloop() ; // 0x80024E5C

extern void TimerTick() ; // 0x80024E78

extern void CheckVolatile() ; // 0x80024EB0

extern void DRAW_AnimatedModel_S() ; // 0x80024EBC

extern void anim_loop_top() ; // 0x80024FA0

extern void anim_dont_flip_backface() ; // 0x80025008

extern void sort_max_z2() ; // 0x80025080

extern void mm2L1() ; // 0x8002509C

extern void mm2L2() ; // 0x800250AC

extern void sort_avz_continue2() ; // 0x800250B4

extern void anim_no_pushsort() ; // 0x800250D0

extern void not_colored_fog() ; // 0x800250FC

extern void anim_ft3() ; // 0x80025104

extern void always_ft3() ; // 0x80025114

extern void nofade_ft3_a() ; // 0x80025150

extern void nofade_ft3_b() ; // 0x80025188

extern void nofade_ft3_c() ; // 0x80025200

extern void anim_gt3() ; // 0x80025228

extern void nofade_gt3_a() ; // 0x80025298

extern void nofade_gt3_b2() ; // 0x80025310

extern void nofade_gt3_b() ; // 0x80025328

extern void anim_nontext() ; // 0x80025384

extern void anim_g3() ; // 0x8002538C

extern void anim_g3_from_t() ; // 0x800253C4

extern void anim_g3_overface() ; // 0x80025484

extern void anim_f3() ; // 0x800254DC

extern void anim_next_mface() ; // 0x80025530

extern void exit_AnimDraw() ; // 0x80025548

extern void fogged_face() ; // 0x8002557C

extern void always_fogged_ft3() ; // 0x800255A4

extern void fanim_gt3() ; // 0x80025630

extern void fog_backface() ; // 0x800256D4

extern void fog_backface2() ; // 0x8002573C

extern void fanim_nontext() ; // 0x800257BC

extern void fanim_g3() ; // 0x800257C4

extern void fanim_f3() ; // 0x800258C4

extern void faded_backpoly() ; // 0x80025970

extern void faded_t5_smaller() ; // 0x800259DC

extern void faded_t6_min() ; // 0x800259F0

extern void faded_tpage_calc() ; // 0x800259F4

extern void do_super_subdiv() ; // 0x80025A58

extern void super_clip() ; // 0x80025AC4

extern void DRAW_DisplayTFace_S() ; // 0x80025ACC

extern void rLL2() ; // 0x80025C98

extern void rLL2A() ; // 0x80025CA0

extern void subdiv_begin() ; // 0x80025CA8

extern void not_liquid_26() ; // 0x80025D8C

extern void next_tface_subdiv() ; // 0x80026284

extern void DRAW_DisplaySubdivPolytope_S() ; // 0x8002629C

extern void do_poly() ; // 0x80026430

extern void positive() ; // 0x80026660

extern void not_quad() ; // 0x80026720

extern void no_subdiv() ; // 0x80026734

extern void maxL1() ; // 0x80026770

extern void maxL2() ; // 0x80026784

extern void minL1() ; // 0x80026794

extern void not_liquid_54() ; // 0x800268CC

extern void depthQface() ; // 0x80026910

extern void no_depthQ() ; // 0x80026A54

extern void skip_tface() ; // 0x80026A6C

extern void no_blend2() ; // 0x80026A84

extern void end_of_polytope() ; // 0x80026AA0

extern void draw_trans_fog() ; // 0x80026AD0

extern void OK() ; // 0x80026B04

extern void no_depthQ2() ; // 0x80026BBC

extern void DRAW_DisplayPolytopeList_S(struct _VertexPool*, struct _Terrain*, struct Camera*, struct _PrimPool*, unsigned int**, struct _Position*) ; // 0x80026BD4

extern void PIPE3D_TransformAnimatedInstanceVertices_S() ; // 0x80026E10

extern void DRAW_MFace3_S() ; // 0x80027C48

extern void DRAW_MFace3_MAXZ0() ; // 0x80027C88

extern void DRAW_MFace3_MAXZ1() ; // 0x80027CA4

extern void DRAW_MFace3_MT() ; // 0x80027CB4

extern void DRAW_MFace3_OTZ() ; // 0x80027CD0

extern void DRAW_MFace3_F3() ; // 0x80027CFC

extern void DRAW_MFace3_F3_NCDPQ() ; // 0x80027D20

extern void DRAW_MFace3_F3_AddPrim() ; // 0x80027D80

extern void DRAW_MFace3_FT3() ; // 0x80027DBC

extern void DRAW_MFace3_FT3_NCDPQ() ; // 0x80027DF4

extern void DRAW_MFace3_FT3_AddPrim() ; // 0x80027E74

extern void DRAW_MFace3_Gouraud() ; // 0x80027EC8

extern void DRAW_MFace3_G3() ; // 0x80027F74

extern void DRAW_MFace3_GT3() ; // 0x80027FFC

extern void DRAW_MFace3_GT_nofade() ; // 0x8002807C

extern void DRAW_MFace3_Return() ; // 0x800280CC

extern void DRAW_MFace3_FadeBackpoly() ; // 0x800280D4

extern void DMF_faded_t2_smaller() ; // 0x80028144

extern void DMF_faded_t3_min() ; // 0x80028158

extern void DMF_faded_tpage_calc() ; // 0x8002815C

extern void DRAW_MFace3_fog_g3() ; // 0x800281C4

extern void DRAW_MFace3_fog_ft3() ; // 0x80028294

extern void DRAW_MFace3_fog_gt3() ; // 0x8002832C

extern void DRAW_MFace3_fog_backface() ; // 0x800283BC

extern void DRAW_SplitModel_S() ; // 0x8002842C

extern void DSM_noFade() ; // 0x800284C0

extern void DSM_SetOutcode0() ; // 0x80028508

extern void DSM_SetOutcode1() ; // 0x80028514

extern void DSM_IncrementMFace() ; // 0x800285C8

extern void DSM_BeginLoop() ; // 0x800285CC

extern void DSM_Outcode0() ; // 0x80028674

extern void DSM_Outcode7() ; // 0x80028684

extern void DSM_Outcode7_LDV3() ; // 0x80028688

extern void DSM_Outcode7_MG() ; // 0x800286DC

extern void DSM_Outcode7_G() ; // 0x80028794

extern void DSM_Outcode7_Flat() ; // 0x800287A8

extern void DSM_Outcode7_Draw() ; // 0x800287BC

extern void DSM_Outcode1() ; // 0x800287CC

extern void DSM_Outcode1_MG() ; // 0x80028B18

extern void DSM_Outcode1_G() ; // 0x80028BD0

extern void DSM_Outcode1_Flat() ; // 0x80028BE4

extern void DSM_Outcode1_Draw() ; // 0x80028C00

extern void DSM_Outcode2() ; // 0x80028CB0

extern void DSM_Outcode2_MG() ; // 0x80029004

extern void DSM_Outcode2_G() ; // 0x800290BC

extern void DSM_Outcode2_Flat() ; // 0x800290D0

extern void DSM_Outcode2_Draw() ; // 0x800290EC

extern void DSM_Outcode4() ; // 0x800291B0

extern void DSM_Outcode4_MG() ; // 0x80029504

extern void DSM_Outcode4_G() ; // 0x800295C0

extern void DSM_Outcode4_Flat() ; // 0x800295D4

extern void DSM_Outcode4_Draw() ; // 0x800295F0

extern void DSM_Outcode3() ; // 0x800296B0

extern void DSM_Outcode3_MG() ; // 0x800299F4

extern void DSM_Outcode3_G() ; // 0x80029AAC

extern void DSM_Outcode3_Flat() ; // 0x80029AC0

extern void DSM_Outcode3_Draw() ; // 0x80029ADC

extern void DSM_Outcode5() ; // 0x80029B8C

extern void DSM_Outcode5_MG() ; // 0x80029EE0

extern void DSM_Outcode5_G() ; // 0x80029F9C

extern void DSM_Outcode5_Flat() ; // 0x80029FB0

extern void DSM_Outcode5_Draw() ; // 0x80029FCC

extern void DSM_Outcode6() ; // 0x8002A084

extern void DSM_Outcode6_MG() ; // 0x8002A3D8

extern void DSM_Outcode6_G() ; // 0x8002A490

extern void DSM_Outcode6_Flat() ; // 0x8002A4A4

extern void DSM_Outcode6_Draw() ; // 0x8002A4C0

extern void DSM_CleanUp() ; // 0x8002A578


#endif
