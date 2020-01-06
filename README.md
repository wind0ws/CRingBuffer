# CRingBuffer

English Description：

RingBuffer and RingMessageQueue C implementation, inspired by linux kernal kfifo.

1. ringbuffer.c based on `unsigned int` will overflow to zero，see Linux kernal kfifo for more details.
2. ring_buf.c based on one control byte to judge buffer is empty or full.

------------


中文描述：

一个RingBuffer的C实现。提供多种实现方式。
 1. ringbuffer.c 基于unsigned int溢出归0方式，参考Linux kernal kfifo
 2. ring_buf.c 是常规ring buffer实现，读写控制通过一个字节来判空/判满。
