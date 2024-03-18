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
 * Module: EduBfM_GetTrain.c
 *
 * Description : 
 *  Return a buffer which has the disk content indicated by `trainId'.
 *
 * Exports:
 *  Four EduBfM_GetTrain(TrainID *, char **, Four)
 */


#include "EduBfM_common.h"
#include "EduBfM_Internal.h"



/*@================================
 * EduBfM_GetTrain()
 *================================*/
/*
 * Function: EduBfM_GetTrain(TrainID*, char**, Four)
 *
 * Description : 
 * (Following description is for original ODYSSEUS/COSMOS BfM.
 *  For ODYSSEUS/EduCOSMOS EduBfM, refer to the EduBfM project manual.)
 *
 *  Return a buffer which has the disk content indicated by `trainId'.
 *  Before the allocation of a buffer, look up the train in the buffer
 *  pool using hashing mechanism.   If the train already  exist in the pool
 *  then simply return it and set the reference bit of the correponding
 *  buffer table entry.   Otherwise, i.e. the train does not exist in the
 *  pool, allocate a buffer (a buffer selected as victim may be forced out
 *  by the buffer replacement algorithm), read a disk train into the 
 *  selected buffer train, and return it.
 *
 * Returns:
 *  error code
 *    eBADBUFFER_BFM - Invalid Buffer
 *    eBADBUFFERTYPE_BFM - Invalid Buffer type
 *    some errors caused by function calls
 *
 * Side effects:
 *  1) parameter retBuf
 *     pointer to buffer holding the disk train indicated by `trainId'
 */
Four EduBfM_GetTrain(
    TrainID             *trainId,               /* IN train to be used */
    char                **retBuf,               /* OUT pointer to the returned buffer */
    Four                type )                  /* IN buffer type */
{
    Four                e;                      /* for error */
    Four                index;                  /* index of the buffer pool */
    Two                 fixed;
    One                 bits;


    /*@ Check the validity of given parameters */
    /* Some restrictions may be added         */
    if(retBuf == NULL) ERR(eBADBUFFER_BFM);

    /* Is the buffer type valid? */
    if(IS_BAD_BUFFERTYPE(type)) ERR(eBADBUFFERTYPE_BFM);	

    // 여기부터 시작
    /*
     *  edubfm_LookUp()?
     *  Look up the given key in the hash table and return its
     *  corressponding index to the buffer table.
     */
    // 찾고자하는 train이 bufferPool 안에 존재하는지를 살펴본다.
    index = edubfm_LookUp(trainId, type);
    
    // case 1. 존재
    if (index != NIL) {
        // fixed 값을 1 증가시킨다.
        fixed = BI_FIXED(type, index);
        fixed++;
        BI_FIXED(type, index) = fixed;

        // bits 값을 REFER로 맞춘다.
        bits = BI_BITS(type, index);
        bits = REFER;
        BI_BITS(type, index) = bits;
        
        // return할 element를 buffer element로 맞춘다.
        *refBuf = BI_BUFFER(type, index);
    }
    // case 2. 존재하지 않음
    else {
        // 1. bufferPool에서 index를 할당 받는다.
        
        // 2. disk에 존재하는 train을 읽는다.

        // 3. bufferTable에 train을 쓴다.

        // 4. Hash table을 update한다.

        *refBuf = BI_BUFFER(type, index);
    }

    return(eNOERROR);   /* No error */

}  /* EduBfM_GetTrain() */
