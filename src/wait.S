/* ************************************************************************
 *
 *   wait functions
 *
 *   based on code from Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 *   NOTE: In addition to the relevant instructions in this source file,
 *    the cycles for calling the waitXXX function (call instruction) must
 *    also be included.
 *
 * ************************************************************************ */

/*
 *  local macros
 */


/* 
 *  Instruction (2 bytes) | Cycles
 *   brcc                    2 when carry flag is 1, otherwise 1
 *   brcs                    2 when carry flag is 0, otherwise 1
 *   brne                    1 when counter became 0, otherwise 2
 *   dec r18                 1
 *   ldi r18, n              1
 *   nop                     1
 */

.macro nop2
  nop
  nop         // total cycles = 2; total bytes = 2
.endm

.macro nop3
  brcc .
  brcs .      // total cycles = 3; total bytes = 4
.endm

.macro noploop count  // total cycles = 3 * count; total bytes = 6
  ldi r18, \count
loop:
  dec r18
  brne loop
.endm

.macro nop1us
#ifdef __LGT8F__

/*
 *  Instruction | Cycles | Bytes
 *   call          2        4
 *   rcall         1        2
 *   ret           2        2
 *
 *  wait1us total cycles: (#(wait500ns) + 2(ret)) * 2 + 1(rcall) + 2(call)
 */

  rcall wait500ns
  ;proceed with wait500ms	;+500ns

  wait500ns:
#if F_CPU == 8000000     // 1us total cycles = 9 cycles * 125ns
  nop         // total cycles = 1; total bytes = 2
#elif F_CPU == 16000000  // 1us total cycles = 17 cycles * 62.5ns
  nop2
  nop3        // total cycles = 5; total bytes = 6
#elif F_CPU == 20000000  // 1us total cycles = 21 cycles * 50ns
  nop
  noploop 2   // total cycles = 7; total bytes = 8
#elif F_CPU == 24000000  // 1us total cycles = 25 cycles * 41.67ns
  noploop 3   // total cycles = 9; total bytes = 6
#elif F_CPU == 32000000  // 1us total cycles ~ 33 cycles * 31.25ns
  nop
  noploop 4   // total cycles = 13; total bytes = 8
#else
  #error <<< No or wrong MCU type selected! >>>
#endif // F_CPU

#else // ATmega

/*
 * Instruction | Cycles | Bytes
 *  call          4        4
 *  rcall         3        2
 *  ret           4        2
 * wait1us total cycles:
 *  F_CPU = 8MHz:       1(nop) + 4(ret) + 4(call) = 9 * 125ns
 *  F_CPU = 16, 20MHz:  (#(wait500ns) + 4(ret)) * 2 + 3(rcall) + 4(call)
 */

#if F_CPU == 8000000
  nop			;1µs
#elif F_CPU == 16000000
  rcall wait500ns	;500ns
  ;proceed with wait500ms	;+500ns

  wait500ns:
  nop			;500ns
#elif F_CPU == 20000000
  rcall wait500ns	;500ns
  ;proceed with wait500ns	;+500ns

  wait500ns:
  nop3			;500ns
#else
  #error <<< No or wrong MCU type selected! >>>
#endif // F_CPU

#endif
.endm

.func wait1s
.global wait1s	    ; wait 1s
.global wait500ms	; wait 500ms
.global wait400ms	; wait 400ms
.global wait300ms	; wait 300ms
.global wait200ms	; wait 200ms
.global wait100ms	; wait 100ms
.global wait50ms	; wait 50ms
.global wait40ms	; wait 40ms
.global wait30ms	; wait 30ms
.global wait20ms	; wait 20ms
.global wait10ms	; wait 10ms
.global wait5ms		; wait 5ms
.global wait4ms		; wait 4ms
.global wait3ms		; wait 3ms
.global wait2ms		; wait 2ms
.global wait1ms		; wait 1ms
.global wait500us	; wait 500µs
.global wait400us	; wait 400µs
.global wait300us	; wait 300µs
.global wait200us	; wait 200µs
.global wait100us	; wait 100µs
.global wait50us	; wait 50µs
.global wait40us	; wait 40µs
.global wait30us	; wait 30µs
.global wait20us	; wait 20µs
.global wait10us	; wait 10µs
.global wait5us		; wait 5µs
.global wait4us		; wait 4µs
.global wait3us		; wait 3µs
.global wait2us		; wait 2µs
.global wait1us		; wait 1µs

; waiting functions for MCU clock rates of 8MHz, 16MHz, 20MHz, 24MHz, 32MHz
; stacked design to minimize flash memory usage:
; No registers are used, just the stack pointer.
; Maximum RAM usage of ?? bytes for return addresses.


/*
 *  20µs and up (clock rate independent)
 */

wait1s:	
rcall wait500ms	;500ms	11+x return addresses
;proceed with wait500ms	;+500ms	

wait500ms:
rcall wait100ms	;100ms	10+x return addresses
;proceed with wait400ms	;+400ms

wait400ms:
rcall wait100ms	;100ms	10+x return addresses
;proceed with wait300ms	;+300ms

wait300ms:
rcall wait100ms        ;100ms	10+x return addresses
;proceed with wait200ms	;+200ms

wait200ms:
rcall wait100ms	;100ms	10+x return addresses
;proceed with wait100ms ;+100ms

wait100ms:
wdr			;reset watchdog every 100ms!
rcall wait50ms	    	;50ms	9+x return addresses
;proceed with wait50ms 	;+50ms

wait50ms:
rcall wait10ms	    	;10ms	8+x return addresses 
;proceed with wait40ms	;+40ms

wait40ms:
rcall wait10ms	    	;10ms	8+x return addresses
;proceed with wait30us 	;+30ms

wait30ms:
rcall wait10ms	    	;10ms	8+x return addresses
;proceed with wait20us 	;+20ms

wait20ms:
rcall wait10ms	    	;10ms	8+x return addresses
;proceed with wait10ms	;+10ms

wait10ms:
rcall wait5ms	   	;5ms	7+x return addresses
;proceed with wait5ms	;+5ms

wait5ms:
rcall wait1ms	   	;1ms	6+x return addresses
;proceed with wait4ms	;+4ms

wait4ms:
rcall wait1ms	   	;1ms	6+x return addresses
;proceed with wait3ms	;+3ms	

wait3ms:
rcall wait1ms	   	;1ms	6+x return addresses
;proceed with wait2ms	;+2ms

wait2ms:
rcall wait1ms	   	;1ms	6+x return addresses
;proceed with wait1ms	;+1ms

wait1ms:
rcall wait500us    	;500µs	5+x return addresses
;laeuft in wait500us	;+500µs

wait500us:
rcall wait100us    	;100µs	4+x return addresses
;proceed with wait400us	;+400µs

wait400us:
rcall wait100us    	;100µs	4+x return addresses
;proceed with wait300us	;+300µs

wait300us:
rcall wait100us    	;100µs	4+x return addresses
;proceed with wait200us	;+200µs

wait200us:
rcall wait100us    	;100µs	4+x return addresses
;proceed with wait100us	;+100µs	

wait100us:
rcall wait50us	    	;50µs	3+x return addresses
;proceed with wait50us	;+50µs	

wait50us:
rcall wait10us     	;10µs	2+x return addresses
;proceed with wait40us	;+40µs

wait40us:
rcall wait10us     	;10µs	2+x return addresses
;proceed with wait30us	;+30µs

wait30us:
rcall wait10us     	;10µs	2+x return addresses
;proceed with wait20us	;+20µs

wait20us:
rcall wait10us		;10µs	2+x return addresses
;proceed with wait10us	;+10µs

wait10us:		;	1+x return addresses
rcall wait5us		;5µs
;proceed with wait5us	;+5µs

wait5us:
rcall wait1us		;1µs
;proceed with wait4us	;+4µs

wait4us:
rcall wait1us		;1µs
;proceed with wait3us	;+3us

wait3us:
rcall wait1us		;1µs
;proceed with wait2us	;+2µs

wait2us:
rcall wait1us		;1µs
;proceed with wait1us	;+1µs

/*
 *  1µs based on MCU clock rate
 */

wait1us:
nop1us
ret
