# CRingBuffer


一个RingBuffer的C实现。提供多种实现方式。
 1. ringbuffer.c 基于unsigned int溢出归0方式，参考Linux kernal kfifo
 2. ring_buf.c 是常规ring buffer实现，读写控制通过一个字节来判空/判满。
