#ifndef INLINE_C_EXT_H
#define INLINE_C_EXT_H

#if defined(PSXPC_VERSION)
#define gte_ldlvnlsv( r0 )                       \
	((int*)&MAC1)[0] = ((unsigned short*)r0)[0]; \
	((int*)&MAC2)[0] = ((unsigned short*)r0)[1]; \
	((int*)&MAC3)[0] = ((unsigned short*)r0)[2]; \

#define gte_stlvnlsv( r0 )                       \
	((unsigned short*)r0)[0] = ((int*)&MAC1)[0]; \
	((unsigned short*)r0)[1] = ((int*)&MAC2)[0]; \
	((unsigned short*)r0)[2] = ((int*)&MAC3)[0]; \

#define gte_ldv0sv( r0 )                       \
	((int*)&R11)[0] = ((unsigned short*)r0)[0] | ((unsigned short*)r0)[1]; \
	((int*)&R13)[0] = ((unsigned short*)r0)[2];\

#define gte_ldv2_ext( r0 )                     \
	((int*)&R31)[0] = ((int*)r0)[0];       \
	((int*)&R33)[0] = ((int*)r0)[1];       \

#define gte_ldv3_ext( r0 ) \
	((int*)&R11)[0] = ((int*)r0)[0]; \
	((int*)&R13)[0] = ((int*)r0)[1]; \
	((int*)&R22)[0] = ((int*)r0)[2]; \

#define gte_ldsv_ext( r0, r1, r2 ) \
	IR1 = r0; \
    IR2 = r1; \
    IR3 = r2; \

#define gte_stsv_ext( r0, r1, r2 ) \
	r0 = IR1; \
	r1 = IR2; \
	r2 = IR3; \

#elif defined(PSX_VERSION)
#define gte_ldlvnlsv( r0 ) __asm__ volatile (			\
	"lhu	$12, 0( %0 );"					\
	"lhu	$13, 2 ( %0 );"					\
	"lhu	$14, 4 ( %0 );"					\
	"mtc2	$12, $25;"					\
	"mtc2	$13, $26;"					\
	"mtc2	$14, $27"					\
	:							\
	: "r"( r0 )						\
	: "$12", "$13", "$14" ) 


#define gte_stlvnlsv( r0 ) __asm__ volatile (			\
	"mfc2	$12, $25;"					\
	"mfc2	$13, $26;"					\
	"mfc2	$14, $27;"					\
	"sh	$12, 0( %0 );"					\
	"sh	$13, 2( %0 );"					\
	"sh	$14, 4( %0 )"					\
	:							\
	: "r"( r0 )						\
	: "$12", "$13", "$14", "memory" )

#define gte_ldv0sv( r0 ) __asm__ volatile (			\
	"lhu	$12, 0( %0 );"					\
	"lhu	$13, 2( %0 );"					\
	"lhu	$14, 4( %0 );"					\
    "or     $12, $13;" \
	"ctc2	$12, $0;"					\
	"ctc2	$14, $1;"					\
	:							\
	: "r"( r0 )						\
	: "$12", "$13", "$14" )

#define gte_ldv2_ext( r0 ) __asm__ volatile (			\
	"lw		$12, 0( %0 );"					\
	"lw		$13, 4( %0 );"					\
	"ctc2	$12, $3;"					\
	"ctc2	$13, $4;"					\
	:							\
	: "r"( r0 )						\
	: "$12", "$13" )

#define gte_ldv3_ext( r0 ) __asm__ volatile (			\
	"lw		$12, 0( %0 );"					\
	"lw		$13, 4( %0 );"					\
	"lw		$14, 8( %0 );"					\
	"ctc2	$12, $0;"					\
	"ctc2	$13, $1;"					\
	"ctc2	$14, $2;"					\
	:							\
	: "r"( r0 )						\
	: "$12", "$13", "$14" )

#define gte_ldsv_ext( r0, r1, r2 ) __asm__ volatile (		\
	"mtc2	%0, $9;"					\
	"mtc2	%1, $10;"					\
	"mtc2	%2, $11;"					\
	:							\
	: "r"( r0 ), "r"( r1 ), "r"( r2 ) )

#define gte_stsv_ext( r0, r1, r2 ) __asm__ volatile (			\
	"mfc2	%0, $25;"					\
	"mfc2	%1, $26;"					\
	"mfc2	%2, $27;"					\
	:							\
	: "r"( r0 ), "r"( r1 ), "r"( r2 ) )
#endif

#endif