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
 * Module: edubfm_FlushTrain.c
 *
 * Description : 
 *  Write a train specified by 'trainId' into the disk.
 *
 * Exports:
 *  Four edubfm_FlushTrain(TrainID *, Four)
 */


#include "EduBfM_common.h"
#include "RDsM.h"
#include "RM.h"
#include "EduBfM_Internal.h"



/*@================================
 * edubfm_FlushTrain()
 *================================*/
/*
 * Function: Four edubfm_FlushTrain(TrainID*, Four)
 *
 * Description : 
 * (Following description is for original ODYSSEUS/COSMOS BfM.
 *  For ODYSSEUS/EduCOSMOS EduBfM, refer to the EduBfM project manual.)
 *
 *  Write a train specified by 'trainId' into the disk.
 *  Construct a hash key using the TrainID 'trainId'(actually same)
 *  in order to look up the buffer in the buffer pool. If it is successfully
 *  found, then force it out to the disk using RDsM, especially
 *  RDsM_WriteTrain().
 *
 * Returns:
 *  error code
 *    some errors caused by function calls
 */

 // related functions: edubfm_LookUp(), RDsM_WriteTrain()

Four edubfm_FlushTrain(
    TrainID 		*trainId,		/* IN train to be flushed */
    Four   			type)			/* IN buffer type */
{
    Four 			e;			/* for errors */
    Four 			index;			/* for an index */


	/* Error check whether using not supported functionality by EduBfM */
	if (RM_IS_ROLLBACK_REQUIRED()) ERR(eNOTSUPPORTED_EDUBFM);

    // 여기부터 시작
    // 1. Search for the array index of the buffer element containing the page/train to be flushed
    index = edubfm_LookUp(trainId, type);
    if (index == eBADHASHKEY_BFM) {
        return eBADHASHKEY_BFM;
    }

    // case 1. DIRTY bit이 켜진 경우
    if (BI_BITS(type, index) & DIRTY == 1) {
        e = RDsM_WriteTrain(BI_BUFFER(type, index), trainId, BI_BUFSIZE(type));
        if (e < 0) {
            ERR(e);
        }

        //DIRTY BIT을 끈다.
        BI_BITS(type, index) ^= DIRTY;
        return e;
    }
	else {
        return( eNOERROR );
    }
}  /* edubfm_FlushTrain */
