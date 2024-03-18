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
 * Module: EduBfM_DiscardAll.c
 *
 * Description :
 *  Discard all buffers.
 *
 * Exports:
 *  Four EduBfM_DiscardAll(void)
 */


#include "EduBfM_common.h"
#include "EduBfM_Internal.h"



/*@================================
 * EduBfM_DiscardAll()
 *================================*/
/*
 * Function: Four EduBfM_DiscardAll(void)
 *
 * Description :
 * (Following description is for original ODYSSEUS/COSMOS BfM.
 *  For ODYSSEUS/EduCOSMOS EduBfM, refer to the EduBfM project manual.)
 *
 *  Discard all buffers.
 *
 * Returns:
 *  error code
 */
Four EduBfM_DiscardAll(void)
{
    Four 	e;			/* error */
    Two 	i;			/* index */
    Four 	type;			/* buffer type */

    // Flush가 아닌, Discard. DIRTY bit과는 관련이 없다. 

    // 1. 모든 bufTable의 값을 초기화
    for (type = 0; type < NUM_BUF_TYPES; type++) {
        for (i = 0; i < BI_NBUFS(type); i++) {
            // set pageNo to NIL(-1)
            SET_NILBFMHASHKEY(BI_KEY(type, i));
            // do not need to initialize fixed
            // reset all bits
            BI_BITS(type, i) = ALL_0;
            // do not need to initialize nextHashEntry
        }
    }

    // 2. 모든 hashTable entry를 delete
    edubfm_DeleteAll();

    return(eNOERROR);

}  /* EduBfM_DiscardAll() */
