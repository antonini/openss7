/*****************************************************************************

 @(#) src/drivers/ecdis.h

 -----------------------------------------------------------------------------

 Copyright (c) 2008-2015  Monavacon Limited <http://www.monavacon.com/>
 Copyright (c) 2001-2008  OpenSS7 Corporation <http://www.openss7.com/>
 Copyright (c) 1997-2001  Brian F. G. Bidulock <bidulock@openss7.org>

 All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU Affero General Public License as published by the Free
 Software Foundation; version 3 of the License.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
 details.

 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>, or
 write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
 02139, USA.

 -----------------------------------------------------------------------------

 U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
 behalf of the U.S. Government ("Government"), the following provisions apply
 to you.  If the Software is supplied by the Department of Defense ("DoD"), it
 is classified as "Commercial Computer Software" under paragraph 252.227-7014
 of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
 successor regulations) and the Government is acquiring only the license rights
 granted herein (the license rights customarily provided to non-Government
 users).  If the Software is supplied to any unit or agency of the Government
 other than DoD, it is classified as "Restricted Computer Software" and the
 Government's rights in the Software are defined in paragraph 52.227-19 of the
 Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
 the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
 (or any successor regulations).

 -----------------------------------------------------------------------------

 Commercial licensing and support of this software is available from OpenSS7
 Corporation at a fee.  See http://www.openss7.com/

 *****************************************************************************/

#ifndef __LOCAL_ECDIS_H__
#define __LOCAL_ECDIS_H__

/*
 * SpanDSP - a series of DSP components for telephony
 *
 * ec_disable_detector.h - A detector which should eventually meet the
 *                         G.164/G.165 requirements for detecting the
 *                         2100Hz echo cancellor disable tone.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001 Steve Underwood
 *
 * All rights reserved.
 */

#include "biquad.h"

typedef struct {
	biquad2_state_t notch;
	int notch_level;
	int channel_level;
	int tone_present;
	int tone_cycle_duration;
	int good_cycles;
	int hit;
} echo_can_disable_detector_state_t;

#define FALSE 0
#define TRUE (!FALSE)

static inline void
echo_can_disable_detector_init(echo_can_disable_detector_state_t * det)
{
	/* Elliptic notch */
	/* This is actually centred at 2095Hz, but gets the balance we want, due to the asymmetric
	   walls of the notch */
	biquad2_init(&det->notch,
		     (int32_t) (-0.7600000 * 32768.0),
		     (int32_t) (-0.1183852 * 32768.0),
		     (int32_t) (-0.5104039 * 32768.0),
		     (int32_t) (0.1567596 * 32768.0), (int32_t) (1.0000000 * 32768.0));

	det->channel_level = 0;
	det->notch_level = 0;
	det->tone_present = FALSE;
	det->tone_cycle_duration = 0;
	det->good_cycles = 0;
	det->hit = 0;
}

/*- End of function --------------------------------------------------------*/

static inline int
echo_can_disable_detector_update(echo_can_disable_detector_state_t * det, int16_t amp)
{
	int16_t notched;

	notched = biquad2(&det->notch, amp);
	/* Estimate the overall energy in the channel, and the energy in the notch (i.e. overall
	   channel energy - tone energy => noise). Use abs instead of multiply for speed (is it
	   really faster?). Damp the overall energy a little more for a stable result. Damp the
	   notch energy a little less, so we don't damp out the blip every time the phase reverses */
	det->channel_level += ((abs(amp) - det->channel_level) >> 5);
	det->notch_level += ((abs(notched) - det->notch_level) >> 4);
	if (det->channel_level > 280) {
		/* There is adequate energy in the channel. Is it mostly at 2100Hz? */
		if (det->notch_level * 6 < det->channel_level) {
			/* The notch says yes, so we have the tone. */
			if (!det->tone_present) {
				/* Do we get a kick every 450+-25ms? */
				if (det->tone_cycle_duration >= 425 * 8
				    && det->tone_cycle_duration <= 475 * 8) {
					det->good_cycles++;
					if (det->good_cycles > 2)
						det->hit = TRUE;
				}
				det->tone_cycle_duration = 0;
			}
			det->tone_present = TRUE;
		} else {
			det->tone_present = FALSE;
		}
		det->tone_cycle_duration++;
	} else {
		det->tone_present = FALSE;
		det->tone_cycle_duration = 0;
		det->good_cycles = 0;
	}
	return det->hit;
}

/*- End of function --------------------------------------------------------*/
/*- End of file ------------------------------------------------------------*/

#endif				/* __LOCAL_ECDIS_H__ */
