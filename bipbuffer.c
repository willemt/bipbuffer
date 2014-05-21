
#include "stdio.h"

#include <stdlib.h>

/* for memcpy */
#include <string.h>

#include <assert.h>

#include "bipbuffer.h"

#define fail() assert(0)

int bipbuf_get_unused_size(bipbuf_t* me)
{
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

bipbuf_t *bipbuf_new(const unsigned int size)
{
    bipbuf_t *me = malloc(sizeof(bipbuf_t));
    me->a_start = me->a_end = me->b_end = 0;
    me->size = size;
    me->data = malloc(me->size);
    me->b_inuse = 0;
    return me;
}

void bipbuf_free(bipbuf_t* me)
{
    free(me->data);
    free(me);
}

int bipbuf_is_empty(const bipbuf_t* me)
{
    return me->a_start == me->a_end;
}

int bipbuf_offer(bipbuf_t* me, const unsigned char *data, const int size)
{
    /* not enough space */
    if (size > bipbuf_get_unused_size(me))
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

unsigned char *bipbuf_peek(const bipbuf_t* me, const unsigned int size)
{
    /* make sure we can actually peek at this data */
    if (me->size < me->a_start + size)
        return NULL;

    if (bipbuf_is_empty(me))
        return NULL;

    return me->data + me->a_start;
}

unsigned char *bipbuf_poll(bipbuf_t* me, const unsigned int size)
{
    void *end;

    if (bipbuf_is_empty(me))
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

int bipbuf_get_size(const bipbuf_t* me)
{
    return me->size;
}

int bipbuf_get_spaceused(const bipbuf_t* me)
{
    return (me->a_end - me->a_start) + (me->b_end);
}
