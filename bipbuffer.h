#ifndef BIPBUFFER_H
#define BIPBUFFER_H


/**
 * creat new bip buffer.
 * @param order to the power of two equals size*/
void *bipbuf_new(const unsigned int order);

/**
 * @return number of bytes offered
 **/
int bipbuf_offer(void * cb, const unsigned char *data, const int size);

int bipbuf_get_unused_size(void * cb);

/**
 * Look at data.
 * Don't move cursor
 */
unsigned char *bipbuf_peek(const void * cb, const unsigned int len);

/** 
 * Get pointer to data to read. Move the cursor on.
 *
 * @return pointer to data, null if we can't poll this much data
 */
unsigned char *bipbuf_poll(void * cb, const unsigned int size);

void bipbuf_poll_release(void * cb, const int size);

int bipbuf_get_size(const void * cb);

void bipbuf_free(void * cb);

int bipbuf_is_empty(const void * cb);

/**
 * @return tell us how much space we have assigned */
int bipbuf_get_spaceused(const void* cb);

#endif /* BIPBUFFER_H */
