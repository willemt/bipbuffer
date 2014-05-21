#ifndef BIPBUFFER_H
#define BIPBUFFER_H

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


/**
 * creat new bip buffer.
 * @param order to the power of two equals size*/
bipbuf_t *bipbuf_new(const unsigned int order);

/**
 * De-allocate bip buffer */
void bipbuf_poll_release(bipbuf_t *me, const int size);

/**
 * @return number of bytes offered
 **/
int bipbuf_offer(bipbuf_t *me, const unsigned char *data, const int size);

/**
 * @return bytes of unused space the bipbuffer has */
int bipbuf_get_unused_size(bipbuf_t* me);

/**
 * Look at data.
 * Don't move cursor
 */
unsigned char *bipbuf_peek(const bipbuf_t* me, const unsigned int len);

/** 
 * Get pointer to data to read. Move the cursor on.
 *
 * @return pointer to data, null if we can't poll this much data
 */
unsigned char *bipbuf_poll(bipbuf_t* me, const unsigned int size);

int bipbuf_get_size(const bipbuf_t* me);

void bipbuf_free(bipbuf_t* me);

int bipbuf_is_empty(const bipbuf_t* me);

/**
 * @return tell us how much space we have assigned */
int bipbuf_get_spaceused(const bipbuf_t* cb);

#endif /* BIPBUFFER_H */
