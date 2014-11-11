.. image:: https://travis-ci.org/willemt/bipbuffer.png
   :target: https://travis-ci.org/willemt/bipbuffer

Implementation of the fantastic bipbuffer datastructure invented by Simon Cooke. For more details see:

http://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist

bipbuffers are great because the caller doesn't need to worry about the item falling across the buffer's boundary. Saying that, if you're on a UNIX system it's possible to use mmap to give a normal circular buffer the same ease of use, see: https://github.com/willemt/cbuffer

License
-------
BSD
