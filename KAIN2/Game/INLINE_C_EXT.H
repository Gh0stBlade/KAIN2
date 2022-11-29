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

#endif

#endif