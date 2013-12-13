/*
 
Copyright (c) 2011, Willem-Hendrik Thiart
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * The names of its contributors may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL WILLEM-HENDRIK THIART BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "stdio.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "bipbuffer.h"

#define fail() assert(0)

typedef struct
{
    unsigned long int size;
    /* region A */
    unsigned int a_start, a_end;
    /* region B */
    unsigned int b_end;
    /* is B inuse? */
    int b_inuse;
    void *data;

} bipbuf_t;

int bipbuf_get_unused_size(void * cb)
{
    bipbuf_t *me = cb;

    if (1 == me->b_inuse)
    {
        /* distance between region B and region A */
        return me->a_start - me->b_end;
    }
    else
    {
        /* find out if we should turn on region B?
         * ie. is the distance between B and A more than A to buffer's end? */
        if (me->a_start - me->b_end > me->size - me->a_end)
        {
                me->b_inuse = 1;
                return me->a_start - me->b_end;
        }
        else
        {
            return me->size - me->a_end;
        }
    }
}

/**
 * creat new bip buffer.
 * @param order to the power of two equals size*/
void *bipbuf_new(const unsigned int size)
{
    bipbuf_t *me;

    me = malloc(sizeof(bipbuf_t));
    me->a_start = me->a_end = me->b_end = 0;
    me->size = size;
    me->data = malloc(me->size);
    me->b_inuse = 0;
    return me;
}

void bipbuf_free(void * cb)
{
    bipbuf_t *me = cb;

    free(me->data);
    free(me);
}

int bipbuf_is_empty(const void * cb)
{
    const bipbuf_t *me = cb;
    return me->a_start == me->a_end;
}

/**
 * @return number of bytes offered
 **/
int bipbuf_offer(void * cb, const unsigned char *data, const int size)
{
    bipbuf_t *me = cb;

    /* not enough space */
    if (size > bipbuf_get_unused_size(cb))
        return 0;

    if (1 == me->b_inuse)
    {
        memcpy(me->data + me->b_end, data, size);
        me->b_end += size;
    }
    else
    {
        memcpy(me->data + me->a_end, data, size);
        me->a_end += size;
    }

    return size;
}

/**
 * Look at data.
 * Don't move cursor
 */
unsigned char *bipbuf_peek(const void * cb, const unsigned int size)
{
    const bipbuf_t *me = cb;

    /* make sure we can actually peek at this data */
    if (me->size < me->a_start + size)
        return NULL;

    if (bipbuf_is_empty(cb))
        return NULL;

    return me->data + me->a_start;
}

/** 
 * Get pointer to data to read. Move the cursor on.
 *
 * @return pointer to data, null if we can't poll this much data
 */
unsigned char *bipbuf_poll(void * cb, const unsigned int size)
{
    bipbuf_t *me = cb;
    void *end;

//    printf("%lx %d\n", me->data, me->start);
//    printf("%lx %d %lx\n", me->data, me->start, me->data + me->start);
    if (bipbuf_is_empty(cb))
        return NULL;

    /* make sure we can actually poll this data */
    if (me->size < me->a_start + size)
        return NULL;

    end = me->data + me->a_start;
    me->a_start += size;

    /* we seem to be empty.. */
    if (me->a_start == me->a_end)
    {
        /* replace a with region b */
        if (1 == me->b_inuse)
        {
                me->a_start = 0;
                me->a_end = me->b_end;
                me->b_end = me->b_inuse = 0;
        }
        else
        {
            /* safely move the pointer back to the start because we are empty */
            me->a_start = me->a_end = 0;
        }
    }

    return end;
}

int bipbuf_get_size(const void * cb)
{
    const bipbuf_t *me = cb;

    return me->size;
}

/**
 * @return tell us how much space we have assigned */
int bipbuf_get_spaceused(const void* cb)
{
    const bipbuf_t *me = cb;

    return (me->a_end - me->a_start) + (me->b_end);
}
