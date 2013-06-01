

void *bipbuf_new(const unsigned int order);

int bipbuf_offer(void * cb, const unsigned char *data, const int size);

int bipbuf_get_unused_size(void * cb);

unsigned char *bipbuf_peek(const void * cb, const unsigned int len);

unsigned char *bipbuf_poll(void * cb, const unsigned int size);

void bipbuf_poll_release(void * cb, const int size);

int bipbuf_get_size(const void * cb);

void bipbuf_free(void * cb);

int bipbuf_is_empty(const void * cb);

int bipbuf_get_spaceused(const void* cb);
