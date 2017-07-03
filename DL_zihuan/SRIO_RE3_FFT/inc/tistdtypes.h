
#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

/* If the CSL header files are getting included through the MAKEFILE builds we don’t want
 * to include XDC TARGETS at all. */

#ifdef MAKEFILE_BUILD
#undef xdc_target_types__
#endif

#ifndef xdc_target_types__
/*
 * This '#ifndef STD_' is needed to protect from duplicate definitions
 * of Int, Uns, etc. in DSP/BIOS v4.x (e.g. 4.90, 4.80) since these versions
 * of DSP/BIOS did not contain the '#ifndef_TI_STD_TYPES' logic.
 */
#ifndef STD_

/*
 * Aliases for standard C types
 */
typedef int			Int;
typedef unsigned		Uns;
typedef char			Char;

/* pointer to null-terminated character sequence */
typedef char			*String;

typedef void			*Ptr;		/* pointer to arbitrary type */

typedef unsigned short		Bool;		/* boolean */

#endif /* STD_ */

/*
 * Uint8, Uint16, Uint32, etc are defined to be "smallest unit of
 * available storage that is large enough to hold unsigned or integer
 * of specified size".
 */

/* Handle the 6x ISA */
#if defined(_TMS320C6X)
    /* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */
    typedef unsigned int	Uint32;
    typedef unsigned short	Uint16;
    typedef unsigned char	Uint8;

    /* Signed integer definitions (32bit, 16bit, 8bit) follow... */
    typedef int			Int32;
    typedef short		Int16;
    typedef char		Int8;

/* Handle the 54x, 55x and 28x ISAs */
#elif defined(_TMS320C5XX) || defined(__TMS320C55X__) || defined(_TMS320C28X)
    /* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */
    typedef unsigned long	Uint32;
    typedef unsigned short	Uint16;
    typedef unsigned char	Uint8;

    /* Signed integer definitions (32bit, 16bit, 8bit) follow... */
    typedef long		Int32;
    typedef short		Int16;
    typedef char		Int8;

#else
    /* Other ISAs not supported */
    #error <tistdtypes.h> is not supported for this target
#endif  /* defined(_6x_) */

#else
    #include <xdc/std.h>
#endif /* xdc_target_types__ */

#endif  /* _TI_STD_TYPES */

