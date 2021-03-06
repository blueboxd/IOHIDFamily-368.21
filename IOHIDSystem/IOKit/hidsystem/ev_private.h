/*
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/******************************************************************************

    ev_private.h
    Internal defs for the events driver.  The contents of this module
    may need to be tweaked slightly from one architecture to the next.
    22 May 1992	Mike Paquette at NeXT Computers, Inc.
    
    Copyright 1992 NeXT, Inc.
    
    Modified:
    

******************************************************************************/

#ifndef _DEV_EV_PRIVATE_H
#define _DEV_EV_PRIVATE_H

#include <IOKit/hidsystem/IOLLEvent.h>

/* Initial cursor position */
#define INIT_CURSOR_X		16
#define INIT_CURSOR_Y		16

/* Default mouse click time and motion constants */
#define	EV_DCLICKTIME	500000000	/* Default nanoseconds for a double-click */
#define	EV_DCLICKSPACE	3		/* Default pixel threshold for double-clicks */

/* Default Wait Cursor Timing Constants (in nanoseconds) */
#define DefaultWCSustain	300000000	/* 0.3 seconds */	
#define DefaultWCFrameRate	75000000        /* 13.3 frames/second */
#define DefaultWCThreshold	1200000000ULL	/* 1.2 seconds */

#define EV_STD_CURSOR	0
#define EV_WAITCURSOR	1
#define EV_WAITCURSOR_1	2
#define EV_WAITCURSOR_2	3
#define EV_MAXCURSOR	(EV_WAITCURSOR_2)

/* Default dim time is 5 minutes, nanoseconds */
#define DAUTODIMPERIOD	(1000ULL*1000ULL*1000ULL*60*5)
/* Default dim level is one-fourth */
#define DDIMBRIGHTNESS	(EV_SCREEN_MAX_BRIGHTNESS/4)


/* Where event numbers start */
#define INITEVENTNUM 13
#define	NULLEVENTNUM 0		/* The event number that never was */

#define MOVEDEVENTMASK \
	(NX_MOUSEMOVEDMASK | NX_LMOUSEDRAGGEDMASK | NX_RMOUSEDRAGGEDMASK | NX_OMOUSEDRAGGEDMASK)
#define COALESCEEVENTMASK \
	(MOVEDEVENTMASK | NX_MOUSEEXITEDMASK)
#define MOUSEEVENTMASK \
	(NX_LMOUSEDOWNMASK|NX_RMOUSEDOWNMASK|NX_LMOUSEUPMASK|NX_RMOUSEUPMASK|NX_OMOUSEDOWNMASK|NX_OMOUSEUPMASK)
#define PRESSUREEVENTMASK \
	(NX_LMOUSEDOWNMASK|NX_LMOUSEUPMASK|NX_MOUSEMOVEDMASK|NX_LMOUSEDRAGGEDMASK)

/* Flags which can modify meaning of special volume/brightness keys */
#define SPECIALKEYS_MODIFIER_MASK \
			(NX_COMMANDMASK|NX_ALTERNATEMASK|NX_CONTROLMASK|NX_SHIFTMASK)

/* Bits in evg->eventFlags owned by keyboard devices */
#define KEYBOARD_FLAGSMASK \
        (NX_ALPHASHIFTMASK | NX_SHIFTMASK | NX_CONTROLMASK | NX_ALTERNATEMASK \
        | NX_COMMANDMASK | NX_NUMERICPADMASK | NX_HELPMASK | NX_SECONDARYFNMASK\
        | NX_DEVICELSHIFTKEYMASK | NX_DEVICERSHIFTKEYMASK | NX_DEVICELCMDKEYMASK \
        | NX_DEVICERCMDKEYMASK | NX_DEVICELALTKEYMASK | NX_DEVICERALTKEYMASK\
        | NX_DEVICELCTLKEYMASK | NX_DEVICERCTLKEYMASK)

/* Some non-zero token to or with screen number */
#define SCREENTOKEN 256

/* A macro to report if the event queue is not empty */
#define EventsInQueue() \
    (eventsOpen && (((EvGlobals*)evg)->LLEHead != ((EvGlobals*)evg)->LLETail))

struct _eventMsg {
    mach_msg_header_t h;
#ifdef mach3xxx			// this is not needed anymore?
    msg_type_t        t;
#endif
};

/******************************************************************************
    EvScreen
    This structure is used by the ev driver.
    It holds information about a single screen: how much private shmem it owns,
    where its private shmem region starts, its global bounds and four procedure
    vectors. This structure is allocated by the ev driver and is filled in
    when a driver calls ev_register_screen().
******************************************************************************/

typedef volatile struct _evScreen {
    IOGraphicsDevice    *instance;      /* Driver instance owning this screen. */
    IOGBounds           *displayBounds; /* Screen's bounds in the coordinate system of the display space */
    IOGBounds           *desktopBounds; /* Screen's bounds in the coordinate system of the desktop space */
    /*  Display space and desktop space are the same unless using a feature that causes them not to be them, like 
        zooming or HiDPI. Both spaces should be contiguous (no disconnected pieces) and the mapping between them
        should be functional (uniquely bidirectional), but the mapping is likely not continuous (if as x->a, 
        f(x)->f(a), then the mapping is continuous) in cases where display space isn't the same as desktop space.
    */
} EvScreen;

/*
 *	We maintain a queue of EventSrc instances attached to the Event
 *	Driver.  These sources are dynamically checked in with the Event
 *	Driver.  When the driver is closed (as on Window Server exit) we
 *	post a relinquishOwnership:client message to the drivers.
 */
typedef struct {
	IOHIDevice * 	eventSrc;
} eventSrcInfo;

typedef struct {
	eventSrcInfo		info;
	queue_chain_t 		link;
} attachedEventSrc;

// No-op XPR stuff
#define xpr_ev_cursor(x, a, b, c, d, e)
#define xpr_ev_post(x, a, b, c, d, e)

// This shares the same bitspace as NX_ALPHASHIFTMASK and NX_DEVICELCTLKEYMASK
#define NX_HIGHCODE_ENCODING_MASK 0x80000000

#endif /* !_DEV_EV_PRIVATE_H */

