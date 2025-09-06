
/home/fox/.build/test/out/prog.elf:     file format elf32-avr


Disassembly of section .text:

00000000 <__vectors>:
   0:	0c 94 34 00 	jmp	0x68	; 0x68 <__ctors_end>
   4:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
   8:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
   c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  10:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  14:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  18:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  1c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  20:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  24:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  28:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  2c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  30:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  34:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  38:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  3c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  40:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  44:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  48:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  4c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  50:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  54:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  58:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  5c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  60:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  64:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>

00000068 <__ctors_end>:
  68:	11 24       	eor	r1, r1
  6a:	1f be       	out	0x3f, r1	; 63
  6c:	cf ef       	ldi	r28, 0xFF	; 255
  6e:	d8 e0       	ldi	r29, 0x08	; 8
  70:	de bf       	out	0x3e, r29	; 62
  72:	cd bf       	out	0x3d, r28	; 61
  74:	0e 94 c4 00 	call	0x188	; 0x188 <main>
  78:	0c 94 f6 00 	jmp	0x1ec	; 0x1ec <_exit>

0000007c <__bad_interrupt>:
  7c:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

00000080 <DelayTimer>:
  80:	cf 93       	push	r28
  82:	df 93       	push	r29
  84:	cd b7       	in	r28, 0x3d	; 61
  86:	de b7       	in	r29, 0x3e	; 62

00000088 <.Loc.1>:
  88:	86 e4       	ldi	r24, 0x46	; 70
  8a:	90 e0       	ldi	r25, 0x00	; 0

0000008c <.Loc.2>:
  8c:	fc 01       	movw	r30, r24
  8e:	10 82       	st	Z, r1

00000090 <.Loc.3>:
  90:	85 e4       	ldi	r24, 0x45	; 69
  92:	90 e0       	ldi	r25, 0x00	; 0

00000094 <.Loc.4>:
  94:	21 e0       	ldi	r18, 0x01	; 1
  96:	fc 01       	movw	r30, r24
  98:	20 83       	st	Z, r18

0000009a <.Loc.5>:
	...

0000009c <.L2>:
  9c:	85 e3       	ldi	r24, 0x35	; 53
  9e:	90 e0       	ldi	r25, 0x00	; 0
  a0:	fc 01       	movw	r30, r24
  a2:	80 81       	ld	r24, Z

000000a4 <.Loc.7>:
  a4:	88 2f       	mov	r24, r24
  a6:	90 e0       	ldi	r25, 0x00	; 0
  a8:	82 70       	andi	r24, 0x02	; 2
  aa:	99 27       	eor	r25, r25

000000ac <.Loc.8>:
  ac:	00 97       	sbiw	r24, 0x00	; 0
  ae:	b1 f3       	breq	.-20     	; 0x9c <.L2>

000000b0 <.Loc.9>:
  b0:	85 e4       	ldi	r24, 0x45	; 69
  b2:	90 e0       	ldi	r25, 0x00	; 0

000000b4 <.Loc.10>:
  b4:	fc 01       	movw	r30, r24
  b6:	10 82       	st	Z, r1

000000b8 <.Loc.11>:
  b8:	85 e3       	ldi	r24, 0x35	; 53
  ba:	90 e0       	ldi	r25, 0x00	; 0

000000bc <.Loc.12>:
  bc:	22 e0       	ldi	r18, 0x02	; 2
  be:	fc 01       	movw	r30, r24
  c0:	20 83       	st	Z, r18

000000c2 <.Loc.13>:
  c2:	00 00       	nop
  c4:	df 91       	pop	r29
  c6:	cf 91       	pop	r28
  c8:	08 95       	ret

000000ca <dummy>:
  ca:	cf 93       	push	r28
  cc:	df 93       	push	r29
  ce:	cd b7       	in	r28, 0x3d	; 61
  d0:	de b7       	in	r29, 0x3e	; 62

000000d2 <.Loc.15>:
  d2:	00 00       	nop
  d4:	df 91       	pop	r29
  d6:	cf 91       	pop	r28
  d8:	08 95       	ret

000000da <HandS>:
  da:	cf 93       	push	r28
  dc:	df 93       	push	r29
  de:	cd b7       	in	r28, 0x3d	; 61
  e0:	de b7       	in	r29, 0x3e	; 62

000000e2 <.Loc.17>:
  e2:	8a e7       	ldi	r24, 0x7A	; 122
  e4:	90 e0       	ldi	r25, 0x00	; 0

000000e6 <.Loc.18>:
  e6:	21 e1       	ldi	r18, 0x11	; 17
  e8:	fc 01       	movw	r30, r24
  ea:	20 83       	st	Z, r18

000000ec <.Loc.19>:
  ec:	0e 94 ee 00 	call	0x1dc	; 0x1dc <wait10us>

000000f0 <.Loc.20>:
  f0:	0e 94 40 00 	call	0x80	; 0x80 <DelayTimer>

000000f4 <.Loc.21>:
  f4:	88 e2       	ldi	r24, 0x28	; 40
  f6:	90 e0       	ldi	r25, 0x00	; 0

000000f8 <.Loc.22>:
  f8:	22 e2       	ldi	r18, 0x22	; 34
  fa:	fc 01       	movw	r30, r24
  fc:	20 83       	st	Z, r18

000000fe <.Loc.23>:
  fe:	87 e2       	ldi	r24, 0x27	; 39
 100:	90 e0       	ldi	r25, 0x00	; 0

00000102 <.Loc.24>:
 102:	23 e3       	ldi	r18, 0x33	; 51
 104:	fc 01       	movw	r30, r24
 106:	20 83       	st	Z, r18

00000108 <.Loc.25>:
 108:	0e 94 f2 00 	call	0x1e4	; 0x1e4 <wait2us>

0000010c <.Loc.26>:
 10c:	0e 94 65 00 	call	0xca	; 0xca <dummy>

00000110 <.Loc.27>:
 110:	00 00       	nop
 112:	df 91       	pop	r29
 114:	cf 91       	pop	r28
 116:	08 95       	ret

00000118 <timer1_init>:
 118:	cf 93       	push	r28
 11a:	df 93       	push	r29
 11c:	cd b7       	in	r28, 0x3d	; 61
 11e:	de b7       	in	r29, 0x3e	; 62

00000120 <.Loc.29>:
 120:	80 e8       	ldi	r24, 0x80	; 128
 122:	90 e0       	ldi	r25, 0x00	; 0

00000124 <.Loc.30>:
 124:	fc 01       	movw	r30, r24
 126:	10 82       	st	Z, r1

00000128 <.Loc.31>:
 128:	81 e8       	ldi	r24, 0x81	; 129
 12a:	90 e0       	ldi	r25, 0x00	; 0

0000012c <.Loc.32>:
 12c:	21 e0       	ldi	r18, 0x01	; 1
 12e:	fc 01       	movw	r30, r24
 130:	20 83       	st	Z, r18

00000132 <.Loc.33>:
 132:	00 00       	nop
 134:	df 91       	pop	r29
 136:	cf 91       	pop	r28
 138:	08 95       	ret

0000013a <cycles>:
 13a:	cf 93       	push	r28
 13c:	df 93       	push	r29
 13e:	00 d0       	rcall	.+0      	; 0x140 <L0^A>

00000140 <L0^A>:
 140:	00 d0       	rcall	.+0      	; 0x142 <L0^A>

00000142 <L0^A>:
 142:	cd b7       	in	r28, 0x3d	; 61
 144:	de b7       	in	r29, 0x3e	; 62

00000146 <.Loc.35>:
 146:	0e 94 8c 00 	call	0x118	; 0x118 <timer1_init>

0000014a <.Loc.36>:
 14a:	84 e8       	ldi	r24, 0x84	; 132
 14c:	90 e0       	ldi	r25, 0x00	; 0

0000014e <.Loc.37>:
 14e:	fc 01       	movw	r30, r24
 150:	80 81       	ld	r24, Z
 152:	91 81       	ldd	r25, Z+1	; 0x01
 154:	9a 83       	std	Y+2, r25	; 0x02
 156:	89 83       	std	Y+1, r24	; 0x01

00000158 <.Loc.38>:
 158:	0e 94 f3 00 	call	0x1e6	; 0x1e6 <wait1us>

0000015c <.Loc.39>:
 15c:	84 e8       	ldi	r24, 0x84	; 132
 15e:	90 e0       	ldi	r25, 0x00	; 0

00000160 <.Loc.40>:
 160:	fc 01       	movw	r30, r24
 162:	80 81       	ld	r24, Z
 164:	91 81       	ldd	r25, Z+1	; 0x01
 166:	9c 83       	std	Y+4, r25	; 0x04
 168:	8b 83       	std	Y+3, r24	; 0x03

0000016a <.Loc.41>:
 16a:	2b 81       	ldd	r18, Y+3	; 0x03
 16c:	3c 81       	ldd	r19, Y+4	; 0x04
 16e:	89 81       	ldd	r24, Y+1	; 0x01
 170:	9a 81       	ldd	r25, Y+2	; 0x02
 172:	a9 01       	movw	r20, r18
 174:	48 1b       	sub	r20, r24
 176:	59 0b       	sbc	r21, r25
 178:	ca 01       	movw	r24, r20

0000017a <.Loc.42>:
 17a:	0f 90       	pop	r0
 17c:	0f 90       	pop	r0
 17e:	0f 90       	pop	r0
 180:	0f 90       	pop	r0
 182:	df 91       	pop	r29
 184:	cf 91       	pop	r28
 186:	08 95       	ret

00000188 <main>:
 188:	cf 93       	push	r28
 18a:	df 93       	push	r29
 18c:	00 d0       	rcall	.+0      	; 0x18e <L0^A>

0000018e <L0^A>:
 18e:	cd b7       	in	r28, 0x3d	; 61
 190:	de b7       	in	r29, 0x3e	; 62

00000192 <.Loc.44>:
 192:	0e 94 9d 00 	call	0x13a	; 0x13a <cycles>
 196:	9a 83       	std	Y+2, r25	; 0x02
 198:	89 83       	std	Y+1, r24	; 0x01

0000019a <.Loc.45>:
 19a:	80 e0       	ldi	r24, 0x00	; 0
 19c:	90 e0       	ldi	r25, 0x00	; 0

0000019e <.Loc.46>:
 19e:	0f 90       	pop	r0
 1a0:	0f 90       	pop	r0
 1a2:	df 91       	pop	r29
 1a4:	cf 91       	pop	r28
 1a6:	08 95       	ret

000001a8 <wait1s>:
 1a8:	00 d0       	rcall	.+0      	; 0x1aa <wait500ms>

000001aa <wait500ms>:
 1aa:	03 d0       	rcall	.+6      	; 0x1b2 <wait100ms>

000001ac <wait400ms>:
 1ac:	02 d0       	rcall	.+4      	; 0x1b2 <wait100ms>

000001ae <wait300ms>:
 1ae:	01 d0       	rcall	.+2      	; 0x1b2 <wait100ms>

000001b0 <wait200ms>:
 1b0:	00 d0       	rcall	.+0      	; 0x1b2 <wait100ms>

000001b2 <wait100ms>:
 1b2:	a8 95       	wdr

000001b4 <.Loc.6>:
 1b4:	00 d0       	rcall	.+0      	; 0x1b6 <wait50ms>

000001b6 <wait50ms>:
 1b6:	03 d0       	rcall	.+6      	; 0x1be <wait10ms>

000001b8 <wait40ms>:
 1b8:	02 d0       	rcall	.+4      	; 0x1be <wait10ms>

000001ba <wait30ms>:
 1ba:	01 d0       	rcall	.+2      	; 0x1be <wait10ms>

000001bc <wait20ms>:
 1bc:	00 d0       	rcall	.+0      	; 0x1be <wait10ms>

000001be <wait10ms>:
 1be:	00 d0       	rcall	.+0      	; 0x1c0 <wait5ms>

000001c0 <wait5ms>:
 1c0:	03 d0       	rcall	.+6      	; 0x1c8 <wait1ms>

000001c2 <wait4ms>:
 1c2:	02 d0       	rcall	.+4      	; 0x1c8 <wait1ms>

000001c4 <wait3ms>:
 1c4:	01 d0       	rcall	.+2      	; 0x1c8 <wait1ms>

000001c6 <wait2ms>:
 1c6:	00 d0       	rcall	.+0      	; 0x1c8 <wait1ms>

000001c8 <wait1ms>:
 1c8:	00 d0       	rcall	.+0      	; 0x1ca <wait500us>

000001ca <wait500us>:
 1ca:	03 d0       	rcall	.+6      	; 0x1d2 <wait100us>

000001cc <wait400us>:
 1cc:	02 d0       	rcall	.+4      	; 0x1d2 <wait100us>

000001ce <wait300us>:
 1ce:	01 d0       	rcall	.+2      	; 0x1d2 <wait100us>

000001d0 <wait200us>:
 1d0:	00 d0       	rcall	.+0      	; 0x1d2 <wait100us>

000001d2 <wait100us>:
 1d2:	00 d0       	rcall	.+0      	; 0x1d4 <wait50us>

000001d4 <wait50us>:
 1d4:	03 d0       	rcall	.+6      	; 0x1dc <wait10us>

000001d6 <wait40us>:
 1d6:	02 d0       	rcall	.+4      	; 0x1dc <wait10us>

000001d8 <wait30us>:
 1d8:	01 d0       	rcall	.+2      	; 0x1dc <wait10us>

000001da <wait20us>:
 1da:	00 d0       	rcall	.+0      	; 0x1dc <wait10us>

000001dc <wait10us>:
 1dc:	00 d0       	rcall	.+0      	; 0x1de <wait5us>

000001de <wait5us>:
 1de:	03 d0       	rcall	.+6      	; 0x1e6 <wait1us>

000001e0 <wait4us>:
 1e0:	02 d0       	rcall	.+4      	; 0x1e6 <wait1us>

000001e2 <wait3us>:
 1e2:	01 d0       	rcall	.+2      	; 0x1e6 <wait1us>

000001e4 <wait2us>:
 1e4:	00 d0       	rcall	.+0      	; 0x1e6 <wait1us>

000001e6 <wait1us>:
 1e6:	00 d0       	rcall	.+0      	; 0x1e8 <wait500ns>

000001e8 <wait500ns>:
	...

000001ea <.Loc.32>:
 1ea:	08 95       	ret

000001ec <_exit>:
 1ec:	f8 94       	cli

000001ee <__stop_program>:
 1ee:	ff cf       	rjmp	.-2      	; 0x1ee <__stop_program>
