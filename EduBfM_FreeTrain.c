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



#include "EduBfM_common.h"
#include "EduBfM_Internal.h"



/*@================================
 * EduBfM_FreeTrain()
 *================================*/
/*
 * Function: Four EduBfM_FreeTrain(TrainID*, Four)
 *
 * Description :
 * (Following description is for original ODYSSEUS/COSMOS BfM.
 *  For ODYSSEUS/EduCOSMOS EduBfM, refer to the EduBfM project manual.)
 *
 *  Free(or unfix) a buffer.
 *  This function simply frees a buffer by decrementing the fix count by 1.
 *
 * Returns :
 *  error code
 *    eBADBUFFERTYPE_BFM - bad buffer type
 *    some errors caused by fuction calls
 */
Four EduBfM_FreeTrain( 
    TrainID             *trainId,       /* IN train to be freed */
    Four                type)           /* IN buffer type */
{
    Four        index;          /* index on buffer holding the train */
    Four 		e;		/* error code */
    Two         fixed;

    /*@ check if the parameter is valid. */
    if (IS_BAD_BUFFERTYPE(type)) ERR(eBADBUFFERTYPE_BFM);	

    // 1. Unfix할 page/train의 hash key로 buffer index를 검색
    index = edubfm_LookUp(trainId, type);

    // case 1. 존재
    if (index != NIL) {
        // fix count가 음수가 되는 경우 -> error print
        if (BI_FIXED(type, index) == 0) {
            printf("fixed counter is less than 0!!!\n");
            printf("trainId = {%d, %d}\n", trainId->volNo, trainId->pageNo);
        } 
        else {
            BI_FIXED(type, index)--;
        }
    }
    // case 2. 존재X
    else {
        return NOTFOUND_IN_HTABLE;
    }

    return( eNOERROR );    
} /* EduBfM_FreeTrain() */