/******************************************************************************/
/*                                                                            */
/*    ODYSSEUS/EduCOSMOS Educational-Purpose Object Storage System            */
/*                                                                            */
/*    Developed by Professor Kyu-Young Whang et al.                           */
/*                                                                            */
/*    Database and Multimedia Laboratory                                      */
/*                                                                            */
/*    Computer Science Department and                                         */
/*    Advanced Information Technology Research Center (AITrc)                 */
/*    Korea Advanced Institute of Science and Technology (KAIST)              */
/*                                                                            */
/*    e-mail: kywhang@cs.kaist.ac.kr                                          */
/*    phone: +82-42-350-7722                                                  */
/*    fax: +82-42-350-8380                                                    */
/*                                                                            */
/*    Copyright (c) 1995-2013 by Kyu-Young Whang                              */
/*                                                                            */
/*    All rights reserved. No part of this software may be reproduced,        */
/*    stored in a retrieval system, or transmitted, in any form or by any     */
/*    means, electronic, mechanical, photocopying, recording, or otherwise,   */
/*    without prior written permission of the copyright owner.                */
/*                                                                            */
/******************************************************************************/
/*
 * Module: edubfm_AllocTrain.c
 *
 * Description : 
 *  Allocate a new buffer from the buffer pool.
 *
 * Exports:
 *  Four edubfm_AllocTrain(Four)
 */


#include <errno.h>
#include "EduBfM_common.h"
#include "EduBfM_Internal.h"


extern CfgParams_T sm_cfgParams;

/*@================================
 * edubfm_AllocTrain()
 *================================*/
/*
 * Function: Four edubfm_AllocTrain(Four)
 *
 * Description : 
 * (Following description is for original ODYSSEUS/COSMOS BfM.
 *  For ODYSSEUS/EduCOSMOS EduBfM, refer to the EduBfM project manual.)
 *
 *  Allocate a new buffer from the buffer pool.
 *  The used buffer pool is specified by the parameter 'type'.
 *  This routine uses the second chance buffer replacement algorithm
 *  to select a victim.  That is, if the reference bit of current checking
 *  entry (indicated by BI_NEXTVICTIM(type), macro for
 *  bufInfo[type].nextVictim) is set, then simply clear
 *  the bit for the second chance and proceed to the next entry, otherwise
 *  the current buffer indicated by BI_NEXTVICTIM(type) is selected to be
 *  returned.
 *  Before return the buffer, if the dirty bit of the victim is set, it 
 *  must be force out to the disk.
 *
 * Returns;
 *  1) An index of a new buffer from the buffer pool
 *  2) Error codes: Negative value means error code.
 *     eNOUNFIXEDBUF_BFM - There is no unfixed buffer.
 *     some errors caused by fuction calls
 */

Four edubfm_AllocTrain(
    Four 	type)			/* IN type of buffer (PAGE or TRAIN) */
{
    Four 	e;			/* for error */
    Four 	victim;			/* return value */
    Four 	i;   

	/* Error check whether using not supported functionality by EduBfM */
	if(sm_cfgParams.useBulkFlush) ERR(eNOTSUPPORTED_EDUBFM);

    // 여기부터 진행
    // Use the second chance buffer replacement algorithm to select the buffer element to be allocated.
    victim = BI_NEXTVICTIM(type);
    
    //NEXTVICTIM은 buffer replacement algorithm에 의해 결정되는게 아니라, 거기부터 second chance algorithm을 시작한다는 의미
    for (i = 0; i< 2*BI_NBUFS(type); i++) {
        if (!BI_FIXED(type, victim)) {
            // 만약 refer bit이 켜져있다면, 꺼준다.
            if (BI_BITS(type, victim) & REFER == 1) {
                BI_BITS(type, victim) ^= REFER;
            }
            // 꺼져있는 victim을 선정하면 된다.
            else break;
        }
        victim = (victim + 1) % BI_NBUFS(type);
    }

    // victim의 DIRTY bit이 켜져있다면, 덮어쓰기 전 flush를 진행해야한다.
    if (BI_BITS(type, victim) & DIRTY == 1) {
        e = edubfm_FlushTrain(&BI_KEY(type, victim), type);
        if (e != eNOERROR) return e;

        BI_BITS(type, victim) ^= DIRTY;
    }

    // victim을 완전히 밀어버린다.
    BI_BITS(type, victim) = ALL_0;
    edubfm_Delete(&BI_KEY(type, victim), type);
    BI_NEXTVICTIM(type) = (victim + 1) % BI_NBUFS(type);

    return( victim );
    
}  /* edubfm_AllocTrain */
