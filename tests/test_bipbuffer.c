#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"

#include "bipbuffer.h"

void Testbipbuffer_set_size_with_init(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);
    CuAssertTrue(tc, 16 == bipbuf_size(cb));
}

void Testbipbuffer_is_empty_after_init(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);
    CuAssertTrue(tc, bipbuf_is_empty(cb));
}

void Testbipbuffer_is_not_empty_after_offer(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);
    bipbuf_offer(cb, (unsigned char*)strdup("abcd"), 4);
    CuAssertTrue(tc, !bipbuf_is_empty(cb));
}

void Testbipbuffer_is_empty_after_poll_release(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);
    bipbuf_offer(cb, (unsigned char*)"abcd", 4);
    bipbuf_poll(cb, 4);
    CuAssertTrue(tc, bipbuf_is_empty(cb));
}

void Testbipbuffer_spaceused_is_zero_after_poll_release(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);
    bipbuf_offer(cb, (unsigned char*)"abcd", 4);
    CuAssertTrue(tc, 4 == bipbuf_used(cb));
    bipbuf_poll(cb, 4);
    CuAssertTrue(tc, 0 == bipbuf_used(cb));
}

void Txestbipbuffer_cant_offer_if_full(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(0);

    CuAssertTrue(tc, 0 == bipbuf_offer(cb, (unsigned char*)"1000", 4));
}

void Testbipbuffer_offer_and_poll(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);

    bipbuf_offer(cb, (unsigned char*)"abcd", 4);
    CuAssertTrue(tc, 0 == strncmp("abcd", (char*)bipbuf_poll(cb, 4), 4));
}

void Testbipbuffer_offer_and_poll_across_boundary(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(6);

    bipbuf_offer(cb, (unsigned char*)"abcdef", 6);
    bipbuf_poll(cb, 4);
    CuAssertTrue(tc, 4 == bipbuf_offer(cb, (unsigned char*)"1234", 4));
    CuAssertTrue(tc, 0 == strncmp("ef", (char*)bipbuf_poll(cb, 2), 4));
    CuAssertTrue(tc, 0 == strncmp("1234", (char*)bipbuf_poll(cb, 4), 4));
}

void Testbipbuffer_cant_read_len_of_what_we_didnt_offer(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(6);

    bipbuf_offer(cb, (unsigned char*)"abcd", 4);
    bipbuf_offer(cb, (unsigned char*)"abcd", 2);
    bipbuf_poll(cb, 2);
    bipbuf_offer(cb, (unsigned char*)"abcd", 2);
    CuAssertTrue(tc, NULL == bipbuf_poll(cb, 6));
}

void Testbipbuffer_cant_poll_nonexistant(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);

    CuAssertTrue(tc, NULL == bipbuf_poll(cb, 4));
}

void Testbipbuffer_cant_poll_twice_when_released(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);

    bipbuf_offer(cb, (unsigned char*)"1000", 4);
    bipbuf_poll(cb, 4);
    bipbuf_poll(cb, 4);
    CuAssertTrue(tc, NULL == bipbuf_poll(cb, 4));
}

void Testbipbuffer_bipbuffers_independant_of_each_other(CuTest * tc)
{
    void *cb, *cb2;

    cb = bipbuf_new(16);
    cb2 = bipbuf_new(16);

    bipbuf_offer(cb, (unsigned char*)"abcd", 4);
    bipbuf_offer(cb2, (unsigned char*)"efgh", 4);
    CuAssertTrue(tc, 0 == strncmp("abcd", (char*)bipbuf_poll(cb, 4), 4));
    CuAssertTrue(tc, 0 == strncmp("efgh", (char*)bipbuf_poll(cb2, 4), 4));
}

void Testbipbuffer_bipbuffers_independant_of_each_other_with_no_polling(
    CuTest * tc)
{
    void *cb, *cb2;

    cb = bipbuf_new(16);
    cb2 = bipbuf_new(16);

    bipbuf_offer(cb, (unsigned char*)"abcd", 4);
    bipbuf_offer(cb2, (unsigned char*)"efgh", 4);
    CuAssertTrue(tc, 0 == strncmp("abcd", (char*)bipbuf_peek(cb, 4), 4));
    CuAssertTrue(tc, 0 == strncmp("efgh", (char*)bipbuf_peek(cb2, 4), 4));
}

#if 0
void Txestbipbuffer_get_unused_when_overlapping(CuTest * tc)
{
    void *cb;

    cb = bipbuf_new(16);

    bipbuf_offer(cb, (unsigned char*)"123", 3);
    bipbuf_poll(cb, 2);
    bipbuf_offer(cb, (unsigned char*)"45", 2);
    CuAssertTrue(tc, 0 == strncmp("1000", (char*)bipbuf_poll(cb, 4), 4));
}
#endif
