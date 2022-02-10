typedef struct {
    short left;	       /* Lch */
    short right;       /* Rch */
} SpuVolume;

typedef struct {
    SpuVolume	volume;		  /* volume       */
    long	reverb;		  /* reverb on/off */
    long	mix;		  /* mixing on/off */
} SpuExtAttr;

typedef struct {
    unsigned long	mask;	  /* settings mask */

    SpuVolume		mvol;	  /* master volume */
    SpuVolume		mvolmode; /* master volume mode */
    SpuVolume		mvolx;	  /* current master volume */
    SpuExtAttr		cd;	  /* CD input attributes */
    SpuExtAttr		ext;	  /* external digital input attributes */
} SpuCommonAttr;

typedef struct {
    unsigned long	mask;	  /* settings mask */

    long		mode;	  /* reverb mode */
    SpuVolume		depth;	  /* reverb depth */
    long                delay;	  /* Delay Time  (ECHO, DELAY only)   */
    long                feedback; /* Feedback    (ECHO only)          */
} SpuReverbAttr;

typedef void (*SpuTransferCallbackProc)(void);
