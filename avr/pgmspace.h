1 /* Copyright (c) 2002-2007 Marek Michalkiewicz
2  Copyright (c) 2006, Carlos Lamas
3  Copyright (c) 2009-2010, Jan Waclawek
4  All rights reserved.
5 
6  Redistribution and use in source and binary forms, with or without
7  modification, are permitted provided that the following conditions are met:
8 
9  * Redistributions of source code must retain the above copyright
10  notice, this list of conditions and the following disclaimer.
11  * Redistributions in binary form must reproduce the above copyright
12  notice, this list of conditions and the following disclaimer in
13  the documentation and/or other materials provided with the
14  distribution.
15  * Neither the name of the copyright holders nor the names of
16  contributors may be used to endorse or promote products derived
17  from this software without specific prior written permission.
18 
19  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
20  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
21  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
22  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
23  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
24  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
25  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
26  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
27  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
28  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
29  POSSIBILITY OF SUCH DAMAGE. */
30 
31 /* $Id: pgmspace_8h_source.html,v 1.1.1.5 2014/08/13 06:29:31 joerg_wunsch Exp $ */
32 
33 /*
34  pgmspace.h
35 
36  Contributors:
37  Created by Marek Michalkiewicz <marekm@linux.org.pl>
38  Eric B. Weddington <eric@ecentral.com>
39  Wolfgang Haidinger <wh@vmars.tuwien.ac.at> (pgm_read_dword())
40  Ivanov Anton <anton@arc.com.ru> (pgm_read_float())
41  */
42 
43 /** \file */
44 /** \defgroup avr_pgmspace <avr/pgmspace.h>: Program Space Utilities
45  \code
46  #include <avr/io.h>
47  #include <avr/pgmspace.h>
48  \endcode
49 
50  The functions in this module provide interfaces for a program to access
51  data stored in program space (flash memory) of the device. In order to
52  use these functions, the target device must support either the \c LPM or
53  \c ELPM instructions.
54 
55  \note These functions are an attempt to provide some compatibility with
56  header files that come with IAR C, to make porting applications between
57  different compilers easier. This is not 100% compatibility though (GCC
58  does not have full support for multiple address spaces yet).
59 
60  \note If you are working with strings which are completely based in ram,
61  use the standard string functions described in \ref avr_string.
62 
63  \note If possible, put your constant tables in the lower 64 KB and use
64  pgm_read_byte_near() or pgm_read_word_near() instead of
65  pgm_read_byte_far() or pgm_read_word_far() since it is more efficient that
66  way, and you can still use the upper 64K for executable code.
67  All functions that are suffixed with a \c _P \e require their
68  arguments to be in the lower 64 KB of the flash ROM, as they do
69  not use ELPM instructions. This is normally not a big concern as
70  the linker setup arranges any program space constants declared
71  using the macros from this header file so they are placed right after
72  the interrupt vectors, and in front of any executable code. However,
73  it can become a problem if there are too many of these constants, or
74  for bootloaders on devices with more than 64 KB of ROM.
75  <em>All these functions will not work in that situation.</em>
76 
77  \note For <b>Xmega</b> devices, make sure the NVM controller
78  command register (\c NVM.CMD or \c NVM_CMD) is set to 0x00 (NOP)
79  before using any of these functions.
80 */
81 
82 #ifndef __PGMSPACE_H_
83 #define __PGMSPACE_H_ 1
84 
85 #define __need_size_t
86 #include <inttypes.h>
87 #include <stddef.h>
88 #include <avr/io.h>
89 
90 #ifndef __ATTR_CONST__
91 #define __ATTR_CONST__ __attribute__((__const__))
92 #endif
93 
94 #ifndef __ATTR_PROGMEM__
95 #define __ATTR_PROGMEM__ __attribute__((__progmem__))
96 #endif
97 
98 #ifndef __ATTR_PURE__
99 #define __ATTR_PURE__ __attribute__((__pure__))
100 #endif
101 
102 /**
103  \ingroup avr_pgmspace
104  \def PROGMEM
105 
106  Attribute to use in order to declare an object being located in
107  flash ROM.
108  */
109 #define PROGMEM __ATTR_PROGMEM__
110 
111 #ifdef __cplusplus
112 extern "C" {
113 #endif
114 
115 #if defined(__DOXYGEN__)
116 /*
117  * Doxygen doesn't grok the appended attribute syntax of
118  * GCC, and confuses the typedefs with function decls, so
119  * supply a doxygen-friendly view.
120  */
121 
122 /**
123  \ingroup avr_pgmspace
124  \typedef prog_void
125  \note DEPRECATED
126 
127  This typedef is now deprecated because the usage of the __progmem__
128  attribute on a type is not supported in GCC. However, the use of the
129  __progmem__ attribute on a variable declaration is supported, and this is
130  now the recommended usage.
131 
132  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
133  has been defined before including <avr/pgmspace.h> (either by a
134  #define directive, or by a -D compiler option.)
135 
136  Type of a "void" object located in flash ROM. Does not make much
137  sense by itself, but can be used to declare a "void *" object in
138  flash ROM.
139 */
140 typedef void PROGMEM prog_void;
141 
142 /**
143  \ingroup avr_pgmspace
144  \typedef prog_char
145  \note DEPRECATED
146 
147  This typedef is now deprecated because the usage of the __progmem__
148  attribute on a type is not supported in GCC. However, the use of the
149  __progmem__ attribute on a variable declaration is supported, and this is
150  now the recommended usage.
151 
152  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
153  has been defined before including <avr/pgmspace.h> (either by a
154  #define directive, or by a -D compiler option.)
155 
156  Type of a "char" object located in flash ROM.
157 */
158 typedef char PROGMEM prog_char;
159 
160 /**
161  \ingroup avr_pgmspace
162  \typedef prog_uchar
163  \note DEPRECATED
164 
165  This typedef is now deprecated because the usage of the __progmem__
166  attribute on a type is not supported in GCC. However, the use of the
167  __progmem__ attribute on a variable declaration is supported, and this is
168  now the recommended usage.
169 
170  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
171  has been defined before including <avr/pgmspace.h> (either by a
172  #define directive, or by a -D compiler option.)
173 
174  Type of an "unsigned char" object located in flash ROM.
175 */
176 typedef unsigned char PROGMEM prog_uchar;
177 
178 /**
179  \ingroup avr_pgmspace
180  \typedef prog_int8_t
181  \note DEPRECATED
182 
183  This typedef is now deprecated because the usage of the __progmem__
184  attribute on a type is not supported in GCC. However, the use of the
185  __progmem__ attribute on a variable declaration is supported, and this is
186  now the recommended usage.
187 
188  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
189  has been defined before including <avr/pgmspace.h> (either by a
190  #define directive, or by a -D compiler option.)
191 
192  Type of an "int8_t" object located in flash ROM.
193 */
194 typedef int8_t PROGMEM prog_int8_t;
195 
196 /**
197  \ingroup avr_pgmspace
198  \typedef prog_uint8_t
199  \note DEPRECATED
200 
201  This typedef is now deprecated because the usage of the __progmem__
202  attribute on a type is not supported in GCC. However, the use of the
203  __progmem__ attribute on a variable declaration is supported, and this is
204  now the recommended usage.
205 
206  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
207  has been defined before including <avr/pgmspace.h> (either by a
208  #define directive, or by a -D compiler option.)
209 
210  Type of an "uint8_t" object located in flash ROM.
211 */
212 typedef uint8_t PROGMEM prog_uint8_t;
213 
214 /**
215  \ingroup avr_pgmspace
216  \typedef prog_int16_t
217  \note DEPRECATED
218 
219  This typedef is now deprecated because the usage of the __progmem__
220  attribute on a type is not supported in GCC. However, the use of the
221  __progmem__ attribute on a variable declaration is supported, and this is
222  now the recommended usage.
223 
224  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
225  has been defined before including <avr/pgmspace.h> (either by a
226  #define directive, or by a -D compiler option.)
227 
228  Type of an "int16_t" object located in flash ROM.
229 */
230 typedef int16_t PROGMEM prog_int16_t;
231 
232 /**
233  \ingroup avr_pgmspace
234  \typedef prog_uint16_t
235  \note DEPRECATED
236 
237  This typedef is now deprecated because the usage of the __progmem__
238  attribute on a type is not supported in GCC. However, the use of the
239  __progmem__ attribute on a variable declaration is supported, and this is
240  now the recommended usage.
241 
242  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
243  has been defined before including <avr/pgmspace.h> (either by a
244  #define directive, or by a -D compiler option.)
245 
246  Type of an "uint16_t" object located in flash ROM.
247 */
248 typedef uint16_t PROGMEM prog_uint16_t;
249 
250 /**
251  \ingroup avr_pgmspace
252  \typedef prog_int32_t
253  \note DEPRECATED
254 
255  This typedef is now deprecated because the usage of the __progmem__
256  attribute on a type is not supported in GCC. However, the use of the
257  __progmem__ attribute on a variable declaration is supported, and this is
258  now the recommended usage.
259 
260  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
261  has been defined before including <avr/pgmspace.h> (either by a
262  #define directive, or by a -D compiler option.)
263 
264  Type of an "int32_t" object located in flash ROM.
265 */
266 typedef int32_t PROGMEM prog_int32_t;
267 
268 /**
269  \ingroup avr_pgmspace
270  \typedef prog_uint32_t
271  \note DEPRECATED
272 
273  This typedef is now deprecated because the usage of the __progmem__
274  attribute on a type is not supported in GCC. However, the use of the
275  __progmem__ attribute on a variable declaration is supported, and this is
276  now the recommended usage.
277 
278  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
279  has been defined before including <avr/pgmspace.h> (either by a
280  #define directive, or by a -D compiler option.)
281 
282  Type of an "uint32_t" object located in flash ROM.
283 */
284 typedef uint32_t PROGMEM prog_uint32_t;
285 
286 /**
287  \ingroup avr_pgmspace
288  \typedef prog_int64_t
289  \note DEPRECATED
290 
291  This typedef is now deprecated because the usage of the __progmem__
292  attribute on a type is not supported in GCC. However, the use of the
293  __progmem__ attribute on a variable declaration is supported, and this is
294  now the recommended usage.
295 
296  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
297  has been defined before including <avr/pgmspace.h> (either by a
298  #define directive, or by a -D compiler option.)
299 
300  Type of an "int64_t" object located in flash ROM.
301 
302  \note This type is not available when the compiler
303  option -mint8 is in effect.
304 */
305 typedef int64_t PROGMEM prog_int64_t;
306 
307 /**
308  \ingroup avr_pgmspace
309  \typedef prog_uint64_t
310  \note DEPRECATED
311 
312  This typedef is now deprecated because the usage of the __progmem__
313  attribute on a type is not supported in GCC. However, the use of the
314  __progmem__ attribute on a variable declaration is supported, and this is
315  now the recommended usage.
316 
317  The typedef is only visible if the macro __PROG_TYPES_COMPAT__
318  has been defined before including <avr/pgmspace.h> (either by a
319  #define directive, or by a -D compiler option.)
320 
321  Type of an "uint64_t" object located in flash ROM.
322 
323  \note This type is not available when the compiler
324  option -mint8 is in effect.
325 */
326 typedef uint64_t PROGMEM prog_uint64_t;
327 
328 /** \ingroup avr_pgmspace
329  \def PGM_P
330 
331  Used to declare a variable that is a pointer to a string in program
332  space. */
333 
334 #ifndef PGM_P
335 #define PGM_P const char *
336 #endif
337 
338 /** \ingroup avr_pgmspace
339  \def PGM_VOID_P
340 
341  Used to declare a generic pointer to an object in program space. */
342 
343 #ifndef PGM_VOID_P
344 #define PGM_VOID_P const void *
345 #endif
346 
347 #elif defined(__PROG_TYPES_COMPAT__) /* !DOXYGEN */
348 
349 typedef void prog_void __attribute__((__progmem__,deprecated("prog_void type is deprecated.")));
350 typedef char prog_char __attribute__((__progmem__,deprecated("prog_char type is deprecated.")));
351 typedef unsigned char prog_uchar __attribute__((__progmem__,deprecated("prog_uchar type is deprecated.")));
352 typedef int8_t prog_int8_t __attribute__((__progmem__,deprecated("prog_int8_t type is deprecated.")));
353 typedef uint8_t prog_uint8_t __attribute__((__progmem__,deprecated("prog_uint8_t type is deprecated.")));
354 typedef int16_t prog_int16_t __attribute__((__progmem__,deprecated("prog_int16_t type is deprecated.")));
355 typedef uint16_t prog_uint16_t __attribute__((__progmem__,deprecated("prog_uint16_t type is deprecated.")));
356 typedef int32_t prog_int32_t __attribute__((__progmem__,deprecated("prog_int32_t type is deprecated.")));
357 typedef uint32_t prog_uint32_t __attribute__((__progmem__,deprecated("prog_uint32_t type is deprecated.")));
358 #if !__USING_MINT8
359 typedef int64_t prog_int64_t __attribute__((__progmem__,deprecated("prog_int64_t type is deprecated.")));
360 typedef uint64_t prog_uint64_t __attribute__((__progmem__,deprecated("prog_uint64_t type is deprecated.")));
361 #endif
362 
363 #ifndef PGM_P
364 #define PGM_P const prog_char *
365 #endif
366 
367 #ifndef PGM_VOID_P
368 #define PGM_VOID_P const prog_void *
369 #endif
370 
371 #else /* !defined(__DOXYGEN__), !defined(__PROG_TYPES_COMPAT__) */
372 
373 #ifndef PGM_P
374 #define PGM_P const char *
375 #endif
376 
377 #ifndef PGM_VOID_P
378 #define PGM_VOID_P const void *
379 #endif
380 #endif /* defined(__DOXYGEN__), defined(__PROG_TYPES_COMPAT__) */
381 
382 /* Although in C, we can get away with just using __c, it does not work in
383  C++. We need to use &__c[0] to avoid the compiler puking. Dave Hylands
384  explaned it thusly,
385 
386  Let's suppose that we use PSTR("Test"). In this case, the type returned
387  by __c is a prog_char[5] and not a prog_char *. While these are
388  compatible, they aren't the same thing (especially in C++). The type
389  returned by &__c[0] is a prog_char *, which explains why it works
390  fine. */
391 
392 #if defined(__DOXYGEN__)
393 /*
394  * The #define below is just a dummy that serves documentation
395  * purposes only.
396  */
397 /** \ingroup avr_pgmspace
398  \def PSTR(s)
399 
400  Used to declare a static pointer to a string in program space. */
401 # define PSTR(s) ((const PROGMEM char *)(s))
402 #else /* !DOXYGEN */
403 /* The real thing. */
404 # define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))
405 #endif /* DOXYGEN */
406 
407 #define __LPM_classic__(addr) \
408 (__extension__({ \
409  uint16_t __addr16 = (uint16_t)(addr); \
410  uint8_t __result; \
411  __asm__ __volatile__ \
412  ( \
413  "lpm" "\n\t" \
414  "mov %0, r0" "\n\t" \
415  : "=r" (__result) \
416  : "z" (__addr16) \
417  : "r0" \
418  ); \
419  __result; \
420 }))
421 
422 #define __LPM_enhanced__(addr) \
423 (__extension__({ \
424  uint16_t __addr16 = (uint16_t)(addr); \
425  uint8_t __result; \
426  __asm__ __volatile__ \
427  ( \
428  "lpm %0, Z" "\n\t" \
429  : "=r" (__result) \
430  : "z" (__addr16) \
431  ); \
432  __result; \
433 }))
434 
435 #define __LPM_word_classic__(addr) \
436 (__extension__({ \
437  uint16_t __addr16 = (uint16_t)(addr); \
438  uint16_t __result; \
439  __asm__ __volatile__ \
440  ( \
441  "lpm" "\n\t" \
442  "mov %A0, r0" "\n\t" \
443  "adiw r30, 1" "\n\t" \
444  "lpm" "\n\t" \
445  "mov %B0, r0" "\n\t" \
446  : "=r" (__result), "=z" (__addr16) \
447  : "1" (__addr16) \
448  : "r0" \
449  ); \
450  __result; \
451 }))
452 
453 #define __LPM_word_enhanced__(addr) \
454 (__extension__({ \
455  uint16_t __addr16 = (uint16_t)(addr); \
456  uint16_t __result; \
457  __asm__ __volatile__ \
458  ( \
459  "lpm %A0, Z+" "\n\t" \
460  "lpm %B0, Z" "\n\t" \
461  : "=r" (__result), "=z" (__addr16) \
462  : "1" (__addr16) \
463  ); \
464  __result; \
465 }))
466 
467 #define __LPM_dword_classic__(addr) \
468 (__extension__({ \
469  uint16_t __addr16 = (uint16_t)(addr); \
470  uint32_t __result; \
471  __asm__ __volatile__ \
472  ( \
473  "lpm" "\n\t" \
474  "mov %A0, r0" "\n\t" \
475  "adiw r30, 1" "\n\t" \
476  "lpm" "\n\t" \
477  "mov %B0, r0" "\n\t" \
478  "adiw r30, 1" "\n\t" \
479  "lpm" "\n\t" \
480  "mov %C0, r0" "\n\t" \
481  "adiw r30, 1" "\n\t" \
482  "lpm" "\n\t" \
483  "mov %D0, r0" "\n\t" \
484  : "=r" (__result), "=z" (__addr16) \
485  : "1" (__addr16) \
486  : "r0" \
487  ); \
488  __result; \
489 }))
490 
491 #define __LPM_dword_enhanced__(addr) \
492 (__extension__({ \
493  uint16_t __addr16 = (uint16_t)(addr); \
494  uint32_t __result; \
495  __asm__ __volatile__ \
496  ( \
497  "lpm %A0, Z+" "\n\t" \
498  "lpm %B0, Z+" "\n\t" \
499  "lpm %C0, Z+" "\n\t" \
500  "lpm %D0, Z" "\n\t" \
501  : "=r" (__result), "=z" (__addr16) \
502  : "1" (__addr16) \
503  ); \
504  __result; \
505 }))
506 
507 #define __LPM_float_classic__(addr) \
508 (__extension__({ \
509  uint16_t __addr16 = (uint16_t)(addr); \
510  float __result; \
511  __asm__ __volatile__ \
512  ( \
513  "lpm" "\n\t" \
514  "mov %A0, r0" "\n\t" \
515  "adiw r30, 1" "\n\t" \
516  "lpm" "\n\t" \
517  "mov %B0, r0" "\n\t" \
518  "adiw r30, 1" "\n\t" \
519  "lpm" "\n\t" \
520  "mov %C0, r0" "\n\t" \
521  "adiw r30, 1" "\n\t" \
522  "lpm" "\n\t" \
523  "mov %D0, r0" "\n\t" \
524  : "=r" (__result), "=z" (__addr16) \
525  : "1" (__addr16) \
526  : "r0" \
527  ); \
528  __result; \
529 }))
530 
531 #define __LPM_float_enhanced__(addr) \
532 (__extension__({ \
533  uint16_t __addr16 = (uint16_t)(addr); \
534  float __result; \
535  __asm__ __volatile__ \
536  ( \
537  "lpm %A0, Z+" "\n\t" \
538  "lpm %B0, Z+" "\n\t" \
539  "lpm %C0, Z+" "\n\t" \
540  "lpm %D0, Z" "\n\t" \
541  : "=r" (__result), "=z" (__addr16) \
542  : "1" (__addr16) \
543  ); \
544  __result; \
545 }))
546 
547 #if defined (__AVR_HAVE_LPMX__)
548 #define __LPM(addr) __LPM_enhanced__(addr)
549 #define __LPM_word(addr) __LPM_word_enhanced__(addr)
550 #define __LPM_dword(addr) __LPM_dword_enhanced__(addr)
551 #define __LPM_float(addr) __LPM_float_enhanced__(addr)
552 #else
553 #define __LPM(addr) __LPM_classic__(addr)
554 #define __LPM_word(addr) __LPM_word_classic__(addr)
555 #define __LPM_dword(addr) __LPM_dword_classic__(addr)
556 #define __LPM_float(addr) __LPM_float_classic__(addr)
557 #endif
558 
559 /** \ingroup avr_pgmspace
560  \def pgm_read_byte_near(address_short)
561  Read a byte from the program space with a 16-bit (near) address.
562  \note The address is a byte address.
563  The address is in the program space. */
564 
565 #define pgm_read_byte_near(address_short) __LPM((uint16_t)(address_short))
566 
567 /** \ingroup avr_pgmspace
568  \def pgm_read_word_near(address_short)
569  Read a word from the program space with a 16-bit (near) address.
570  \note The address is a byte address.
571  The address is in the program space. */
572 
573 #define pgm_read_word_near(address_short) __LPM_word((uint16_t)(address_short))
574 
575 /** \ingroup avr_pgmspace
576  \def pgm_read_dword_near(address_short)
577  Read a double word from the program space with a 16-bit (near) address.
578  \note The address is a byte address.
579  The address is in the program space. */
580 
581 #define pgm_read_dword_near(address_short) \
582  __LPM_dword((uint16_t)(address_short))
583 
584 /** \ingroup avr_pgmspace
585  \def pgm_read_float_near(address_short)
586  Read a float from the program space with a 16-bit (near) address.
587  \note The address is a byte address.
588  The address is in the program space. */
589 
590 #define pgm_read_float_near(address_short) \
591  __LPM_float((uint16_t)(address_short))
592 
593 /** \ingroup avr_pgmspace
594  \def pgm_read_ptr_near(address_short)
595  Read a pointer from the program space with a 16-bit (near) address.
596  \note The address is a byte address.
597  The address is in the program space. */
598 
599 #define pgm_read_ptr_near(address_short) \
600  (void*)__LPM_word((uint16_t)(address_short))
601 
602 #if defined(RAMPZ) || defined(__DOXYGEN__)
603 
604 /* Only for devices with more than 64K of program memory.
605  RAMPZ must be defined (see iom103.h, iom128.h).
606 */
607 
608 /* The classic functions are needed for ATmega103. */
609 
610 #define __ELPM_classic__(addr) \
611 (__extension__({ \
612  uint32_t __addr32 = (uint32_t)(addr); \
613  uint8_t __result; \
614  __asm__ __volatile__ \
615  ( \
616  "out %2, %C1" "\n\t" \
617  "mov r31, %B1" "\n\t" \
618  "mov r30, %A1" "\n\t" \
619  "elpm" "\n\t" \
620  "mov %0, r0" "\n\t" \
621  : "=r" (__result) \
622  : "r" (__addr32), \
623  "I" (_SFR_IO_ADDR(RAMPZ)) \
624  : "r0", "r30", "r31" \
625  ); \
626  __result; \
627 }))
628 
629 #define __ELPM_enhanced__(addr) \
630 (__extension__({ \
631  uint32_t __addr32 = (uint32_t)(addr); \
632  uint8_t __result; \
633  __asm__ __volatile__ \
634  ( \
635  "out %2, %C1" "\n\t" \
636  "movw r30, %1" "\n\t" \
637  "elpm %0, Z+" "\n\t" \
638  : "=r" (__result) \
639  : "r" (__addr32), \
640  "I" (_SFR_IO_ADDR(RAMPZ)) \
641  : "r30", "r31" \
642  ); \
643  __result; \
644 }))
645 
646 #define __ELPM_xmega__(addr) \
647 (__extension__({ \
648  uint32_t __addr32 = (uint32_t)(addr); \
649  uint8_t __result; \
650  __asm__ __volatile__ \
651  ( \
652  "in __tmp_reg__, %2" "\n\t" \
653  "out %2, %C1" "\n\t" \
654  "movw r30, %1" "\n\t" \
655  "elpm %0, Z+" "\n\t" \
656  "out %2, __tmp_reg__" \
657  : "=r" (__result) \
658  : "r" (__addr32), \
659  "I" (_SFR_IO_ADDR(RAMPZ)) \
660  : "r30", "r31" \
661  ); \
662  __result; \
663 }))
664 
665 #define __ELPM_word_classic__(addr) \
666 (__extension__({ \
667  uint32_t __addr32 = (uint32_t)(addr); \
668  uint16_t __result; \
669  __asm__ __volatile__ \
670  ( \
671  "out %2, %C1" "\n\t" \
672  "mov r31, %B1" "\n\t" \
673  "mov r30, %A1" "\n\t" \
674  "elpm" "\n\t" \
675  "mov %A0, r0" "\n\t" \
676  "in r0, %2" "\n\t" \
677  "adiw r30, 1" "\n\t" \
678  "adc r0, __zero_reg__" "\n\t" \
679  "out %2, r0" "\n\t" \
680  "elpm" "\n\t" \
681  "mov %B0, r0" "\n\t" \
682  : "=r" (__result) \
683  : "r" (__addr32), \
684  "I" (_SFR_IO_ADDR(RAMPZ)) \
685  : "r0", "r30", "r31" \
686  ); \
687  __result; \
688 }))
689 
690 #define __ELPM_word_enhanced__(addr) \
691 (__extension__({ \
692  uint32_t __addr32 = (uint32_t)(addr); \
693  uint16_t __result; \
694  __asm__ __volatile__ \
695  ( \
696  "out %2, %C1" "\n\t" \
697  "movw r30, %1" "\n\t" \
698  "elpm %A0, Z+" "\n\t" \
699  "elpm %B0, Z" "\n\t" \
700  : "=r" (__result) \
701  : "r" (__addr32), \
702  "I" (_SFR_IO_ADDR(RAMPZ)) \
703  : "r30", "r31" \
704  ); \
705  __result; \
706 }))
707 
708 #define __ELPM_word_xmega__(addr) \
709 (__extension__({ \
710  uint32_t __addr32 = (uint32_t)(addr); \
711  uint16_t __result; \
712  __asm__ __volatile__ \
713  ( \
714  "in __tmp_reg__, %2" "\n\t" \
715  "out %2, %C1" "\n\t" \
716  "movw r30, %1" "\n\t" \
717  "elpm %A0, Z+" "\n\t" \
718  "elpm %B0, Z" "\n\t" \
719  "out %2, __tmp_reg__" \
720  : "=r" (__result) \
721  : "r" (__addr32), \
722  "I" (_SFR_IO_ADDR(RAMPZ)) \
723  : "r30", "r31" \
724  ); \
725  __result; \
726 }))
727 
728 #define __ELPM_dword_classic__(addr) \
729 (__extension__({ \
730  uint32_t __addr32 = (uint32_t)(addr); \
731  uint32_t __result; \
732  __asm__ __volatile__ \
733  ( \
734  "out %2, %C1" "\n\t" \
735  "mov r31, %B1" "\n\t" \
736  "mov r30, %A1" "\n\t" \
737  "elpm" "\n\t" \
738  "mov %A0, r0" "\n\t" \
739  "in r0, %2" "\n\t" \
740  "adiw r30, 1" "\n\t" \
741  "adc r0, __zero_reg__" "\n\t" \
742  "out %2, r0" "\n\t" \
743  "elpm" "\n\t" \
744  "mov %B0, r0" "\n\t" \
745  "in r0, %2" "\n\t" \
746  "adiw r30, 1" "\n\t" \
747  "adc r0, __zero_reg__" "\n\t" \
748  "out %2, r0" "\n\t" \
749  "elpm" "\n\t" \
750  "mov %C0, r0" "\n\t" \
751  "in r0, %2" "\n\t" \
752  "adiw r30, 1" "\n\t" \
753  "adc r0, __zero_reg__" "\n\t" \
754  "out %2, r0" "\n\t" \
755  "elpm" "\n\t" \
756  "mov %D0, r0" "\n\t" \
757  : "=r" (__result) \
758  : "r" (__addr32), \
759  "I" (_SFR_IO_ADDR(RAMPZ)) \
760  : "r0", "r30", "r31" \
761  ); \
762  __result; \
763 }))
764 
765 #define __ELPM_dword_enhanced__(addr) \
766 (__extension__({ \
767  uint32_t __addr32 = (uint32_t)(addr); \
768  uint32_t __result; \
769  __asm__ __volatile__ \
770  ( \
771  "out %2, %C1" "\n\t" \
772  "movw r30, %1" "\n\t" \
773  "elpm %A0, Z+" "\n\t" \
774  "elpm %B0, Z+" "\n\t" \
775  "elpm %C0, Z+" "\n\t" \
776  "elpm %D0, Z" "\n\t" \
777  : "=r" (__result) \
778  : "r" (__addr32), \
779  "I" (_SFR_IO_ADDR(RAMPZ)) \
780  : "r30", "r31" \
781  ); \
782  __result; \
783 }))
784 
785 #define __ELPM_dword_xmega__(addr) \
786 (__extension__({ \
787  uint32_t __addr32 = (uint32_t)(addr); \
788  uint32_t __result; \
789  __asm__ __volatile__ \
790  ( \
791  "in __tmp_reg__, %2" "\n\t" \
792  "out %2, %C1" "\n\t" \
793  "movw r30, %1" "\n\t" \
794  "elpm %A0, Z+" "\n\t" \
795  "elpm %B0, Z+" "\n\t" \
796  "elpm %C0, Z+" "\n\t" \
797  "elpm %D0, Z" "\n\t" \
798  "out %2, __tmp_reg__" \
799  : "=r" (__result) \
800  : "r" (__addr32), \
801  "I" (_SFR_IO_ADDR(RAMPZ)) \
802  : "r30", "r31" \
803  ); \
804  __result; \
805 }))
806 
807 #define __ELPM_float_classic__(addr) \
808 (__extension__({ \
809  uint32_t __addr32 = (uint32_t)(addr); \
810  float __result; \
811  __asm__ __volatile__ \
812  ( \
813  "out %2, %C1" "\n\t" \
814  "mov r31, %B1" "\n\t" \
815  "mov r30, %A1" "\n\t" \
816  "elpm" "\n\t" \
817  "mov %A0, r0" "\n\t" \
818  "in r0, %2" "\n\t" \
819  "adiw r30, 1" "\n\t" \
820  "adc r0, __zero_reg__" "\n\t" \
821  "out %2, r0" "\n\t" \
822  "elpm" "\n\t" \
823  "mov %B0, r0" "\n\t" \
824  "in r0, %2" "\n\t" \
825  "adiw r30, 1" "\n\t" \
826  "adc r0, __zero_reg__" "\n\t" \
827  "out %2, r0" "\n\t" \
828  "elpm" "\n\t" \
829  "mov %C0, r0" "\n\t" \
830  "in r0, %2" "\n\t" \
831  "adiw r30, 1" "\n\t" \
832  "adc r0, __zero_reg__" "\n\t" \
833  "out %2, r0" "\n\t" \
834  "elpm" "\n\t" \
835  "mov %D0, r0" "\n\t" \
836  : "=r" (__result) \
837  : "r" (__addr32), \
838  "I" (_SFR_IO_ADDR(RAMPZ)) \
839  : "r0", "r30", "r31" \
840  ); \
841  __result; \
842 }))
843 
844 #define __ELPM_float_enhanced__(addr) \
845 (__extension__({ \
846  uint32_t __addr32 = (uint32_t)(addr); \
847  float __result; \
848  __asm__ __volatile__ \
849  ( \
850  "out %2, %C1" "\n\t" \
851  "movw r30, %1" "\n\t" \
852  "elpm %A0, Z+" "\n\t" \
853  "elpm %B0, Z+" "\n\t" \
854  "elpm %C0, Z+" "\n\t" \
855  "elpm %D0, Z" "\n\t" \
856  : "=r" (__result) \
857  : "r" (__addr32), \
858  "I" (_SFR_IO_ADDR(RAMPZ)) \
859  : "r30", "r31" \
860  ); \
861  __result; \
862 }))
863 
864 #define __ELPM_float_xmega__(addr) \
865 (__extension__({ \
866  uint32_t __addr32 = (uint32_t)(addr); \
867  float __result; \
868  __asm__ __volatile__ \
869  ( \
870  "in __tmp_reg__, %2" "\n\t" \
871  "out %2, %C1" "\n\t" \
872  "movw r30, %1" "\n\t" \
873  "elpm %A0, Z+" "\n\t" \
874  "elpm %B0, Z+" "\n\t" \
875  "elpm %C0, Z+" "\n\t" \
876  "elpm %D0, Z" "\n\t" \
877  "out %2, __tmp_reg__" \
878  : "=r" (__result) \
879  : "r" (__addr32), \
880  "I" (_SFR_IO_ADDR(RAMPZ)) \
881  : "r30", "r31" \
882  ); \
883  __result; \
884 }))
885 
886 /*
887 Check for architectures that implement RAMPD (avrxmega3, avrxmega5,
888 avrxmega7) as they need to save/restore RAMPZ for ELPM macros so it does
889 not interfere with data accesses.
890 */
891 #if defined (__AVR_HAVE_RAMPD__)
892 
893 #define __ELPM(addr) __ELPM_xmega__(addr)
894 #define __ELPM_word(addr) __ELPM_word_xmega__(addr)
895 #define __ELPM_dword(addr) __ELPM_dword_xmega__(addr)
896 #define __ELPM_float(addr) __ELPM_float_xmega__(addr)
897 
898 #else
899 
900 #if defined (__AVR_HAVE_LPMX__)
901 
902 #define __ELPM(addr) __ELPM_enhanced__(addr)
903 #define __ELPM_word(addr) __ELPM_word_enhanced__(addr)
904 #define __ELPM_dword(addr) __ELPM_dword_enhanced__(addr)
905 #define __ELPM_float(addr) __ELPM_float_enhanced__(addr)
906 
907 #else
908 
909 #define __ELPM(addr) __ELPM_classic__(addr)
910 #define __ELPM_word(addr) __ELPM_word_classic__(addr)
911 #define __ELPM_dword(addr) __ELPM_dword_classic__(addr)
912 #define __ELPM_float(addr) __ELPM_float_classic__(addr)
913 
914 #endif /* __AVR_HAVE_LPMX__ */
915 
916 #endif /* __AVR_HAVE_RAMPD__ */
917 
918 
919 /** \ingroup avr_pgmspace
920  \def pgm_read_byte_far(address_long)
921  Read a byte from the program space with a 32-bit (far) address.
922 
923  \note The address is a byte address.
924  The address is in the program space. */
925 
926 #define pgm_read_byte_far(address_long) __ELPM((uint32_t)(address_long))
927 
928 /** \ingroup avr_pgmspace
929  \def pgm_read_word_far(address_long)
930  Read a word from the program space with a 32-bit (far) address.
931 
932  \note The address is a byte address.
933  The address is in the program space. */
934 
935 #define pgm_read_word_far(address_long) __ELPM_word((uint32_t)(address_long))
936 
937 /** \ingroup avr_pgmspace
938  \def pgm_read_dword_far(address_long)
939  Read a double word from the program space with a 32-bit (far) address.
940 
941  \note The address is a byte address.
942  The address is in the program space. */
943 
944 #define pgm_read_dword_far(address_long) __ELPM_dword((uint32_t)(address_long))
945 
946 /** \ingroup avr_pgmspace
947  \def pgm_read_float_far(address_long)
948  Read a float from the program space with a 32-bit (far) address.
949 
950  \note The address is a byte address.
951  The address is in the program space. */
952 
953 #define pgm_read_float_far(address_long) __ELPM_float((uint32_t)(address_long))
954 
955 /** \ingroup avr_pgmspace
956  \def pgm_read_ptr_far(address_long)
957  Read a pointer from the program space with a 32-bit (far) address.
958 
959  \note The address is a byte address.
960  The address is in the program space. */
961 
962 #define pgm_read_ptr_far(address_long) (void*)__ELPM_word((uint32_t)(address_long))
963 
964 #endif /* RAMPZ or __DOXYGEN__ */
965 
966 /** \ingroup avr_pgmspace
967  \def pgm_read_byte(address_short)
968  Read a byte from the program space with a 16-bit (near) address.
969 
970  \note The address is a byte address.
971  The address is in the program space. */
972 
973 #define pgm_read_byte(address_short) pgm_read_byte_near(address_short)
974 
975 /** \ingroup avr_pgmspace
976  \def pgm_read_word(address_short)
977  Read a word from the program space with a 16-bit (near) address.
978 
979  \note The address is a byte address.
980  The address is in the program space. */
981 
982 #define pgm_read_word(address_short) pgm_read_word_near(address_short)
983 
984 /** \ingroup avr_pgmspace
985  \def pgm_read_dword(address_short)
986  Read a double word from the program space with a 16-bit (near) address.
987 
988  \note The address is a byte address.
989  The address is in the program space. */
990 
991 #define pgm_read_dword(address_short) pgm_read_dword_near(address_short)
992 
993 /** \ingroup avr_pgmspace
994  \def pgm_read_float(address_short)
995  Read a float from the program space with a 16-bit (near) address.
996 
997  \note The address is a byte address.
998  The address is in the program space. */
999 
1000 #define pgm_read_float(address_short) pgm_read_float_near(address_short)
1001 
1002 /** \ingroup avr_pgmspace
1003  \def pgm_read_ptr(address_short)
1004  Read a pointer from the program space with a 16-bit (near) address.
1005 
1006  \note The address is a byte address.
1007  The address is in the program space. */
1008 
1009 #define pgm_read_ptr(address_short) pgm_read_ptr_near(address_short)
1010 
1011 /* pgm_get_far_address() macro
1012 
1013  This macro facilitates the obtention of a 32 bit "far" pointer (only 24 bits
1014  used) to data even passed the 64KB limit for the 16 bit ordinary pointer. It
1015  is similar to the '&' operator, with some limitations.
1016 
1017  Comments:
1018 
1019  - The overhead is minimal and it's mainly due to the 32 bit size operation.
1020 
1021  - 24 bit sizes guarantees the code compatibility for use in future devices.
1022 
1023  - hh8() is an undocumented feature but seems to give the third significant byte
1024  of a 32 bit data and accepts symbols, complementing the functionality of hi8()
1025  and lo8(). There is not an equivalent assembler function to get the high
1026  significant byte.
1027 
1028  - 'var' has to be resolved at linking time as an existing symbol, i.e, a simple
1029  type variable name, an array name (not an indexed element of the array, if the
1030  index is a constant the compiler does not complain but fails to get the address
1031  if optimization is enabled), a struct name or a struct field name, a function
1032  identifier, a linker defined identifier,...
1033 
1034  - The returned value is the identifier's VMA (virtual memory address) determined
1035  by the linker and falls in the corresponding memory region. The AVR Harvard
1036  architecture requires non overlapping VMA areas for the multiple address spaces
1037  in the processor: Flash ROM, RAM, and EEPROM. Typical offset for this are
1038  0x00000000, 0x00800xx0, and 0x00810000 respectively, derived from the linker
1039  script used and linker options. The value returned can be seen then as a
1040  universal pointer.
1041 
1042 */
1043 
1044 #define pgm_get_far_address(var) \
1045 ({ \
1046  uint_farptr_t tmp; \
1047  \
1048  __asm__ __volatile__( \
1049  \
1050  "ldi %A0, lo8(%1)" "\n\t" \
1051  "ldi %B0, hi8(%1)" "\n\t" \
1052  "ldi %C0, hh8(%1)" "\n\t" \
1053  "clr %D0" "\n\t" \
1054  : \
1055  "=d" (tmp) \
1056  : \
1057  "p" (&(var)) \
1058  ); \
1059  tmp; \
1060 })
1061 
1062 
1063 
1064 extern const void * memchr_P(const void *, int __val, size_t __len) __ATTR_CONST__;
1065 extern int memcmp_P(const void *, const void *, size_t) __ATTR_PURE__;
1066 extern void *memccpy_P(void *, const void *, int __val, size_t);
1067 extern void *memcpy_P(void *, const void *, size_t);
1068 extern void *memmem_P(const void *, size_t, const void *, size_t) __ATTR_PURE__;
1069 extern const void * memrchr_P(const void *, int __val, size_t __len) __ATTR_CONST__;
1070 extern char *strcat_P(char *, const char *);
1071 extern const char * strchr_P(const char *, int __val) __ATTR_CONST__;
1072 extern const char * strchrnul_P(const char *, int __val) __ATTR_CONST__;
1073 extern int strcmp_P(const char *, const char *) __ATTR_PURE__;
1074 extern char *strcpy_P(char *, const char *);
1075 extern int strcasecmp_P(const char *, const char *) __ATTR_PURE__;
1076 extern char *strcasestr_P(const char *, const char *) __ATTR_PURE__;
1077 extern size_t strcspn_P(const char *__s, const char * __reject) __ATTR_PURE__;
1078 extern size_t strlcat_P (char *, const char *, size_t );
1079 extern size_t strlcpy_P (char *, const char *, size_t );
1080 extern size_t __strlen_P(const char *) __ATTR_CONST__; /* program memory can't change */
1081 extern size_t strnlen_P(const char *, size_t) __ATTR_CONST__; /* program memory can't change */
1082 extern int strncmp_P(const char *, const char *, size_t) __ATTR_PURE__;
1083 extern int strncasecmp_P(const char *, const char *, size_t) __ATTR_PURE__;
1084 extern char *strncat_P(char *, const char *, size_t);
1085 extern char *strncpy_P(char *, const char *, size_t);
1086 extern char *strpbrk_P(const char *__s, const char * __accept) __ATTR_PURE__;
1087 extern const char * strrchr_P(const char *, int __val) __ATTR_CONST__;
1088 extern char *strsep_P(char **__sp, const char * __delim);
1089 extern size_t strspn_P(const char *__s, const char * __accept) __ATTR_PURE__;
1090 extern char *strstr_P(const char *, const char *) __ATTR_PURE__;
1091 extern char *strtok_P(char *__s, const char * __delim);
1092 extern char *strtok_rP(char *__s, const char * __delim, char **__last);
1093 
1094 extern size_t strlen_PF (uint_farptr_t src) __ATTR_CONST__; /* program memory can't change */
1095 extern size_t strnlen_PF (uint_farptr_t src, size_t len) __ATTR_CONST__; /* program memory can't change */
1096 extern void *memcpy_PF (void *dest, uint_farptr_t src, size_t len);
1097 extern char *strcpy_PF (char *dest, uint_farptr_t src);
1098 extern char *strncpy_PF (char *dest, uint_farptr_t src, size_t len);
1099 extern char *strcat_PF (char *dest, uint_farptr_t src);
1100 extern size_t strlcat_PF (char *dst, uint_farptr_t src, size_t siz);
1101 extern char *strncat_PF (char *dest, uint_farptr_t src, size_t len);
1102 extern int strcmp_PF (const char *s1, uint_farptr_t s2) __ATTR_PURE__;
1103 extern int strncmp_PF (const char *s1, uint_farptr_t s2, size_t n) __ATTR_PURE__;
1104 extern int strcasecmp_PF (const char *s1, uint_farptr_t s2) __ATTR_PURE__;
1105 extern int strncasecmp_PF (const char *s1, uint_farptr_t s2, size_t n) __ATTR_PURE__;
1106 extern char *strstr_PF (const char *s1, uint_farptr_t s2);
1107 extern size_t strlcpy_PF (char *dst, uint_farptr_t src, size_t siz);
1108 extern int memcmp_PF(const void *, uint_farptr_t, size_t) __ATTR_PURE__;
1109 
1110 
1111 __attribute__((__always_inline__)) static __inline__ size_t strlen_P(const char * s);
1112 static __inline__ size_t strlen_P(const char *s) {
1113  return __builtin_constant_p(__builtin_strlen(s))
1114  ? __builtin_strlen(s) : __strlen_P(s);
1115 }
1116 
1117 
1118 
1119 #ifdef __cplusplus
1120 }
1121 #endif
1122 
1123 #endif /* __PGMSPACE_H_ */
prog_uint32_t
uint32_t PROGMEM prog_uint32_t
Definition: pgmspace.h:284
int64_t
signed long long int int64_t
Definition: stdint.h:107
strtok_P
char * strtok_P(char *s, PGM_P delim)
Parses the string into tokens.
Definition: strtok_P.c:60
prog_int32_t
int32_t PROGMEM prog_int32_t
Definition: pgmspace.h:266
prog_uint64_t
uint64_t PROGMEM prog_uint64_t
Definition: pgmspace.h:326
prog_uchar
unsigned char PROGMEM prog_uchar
Definition: pgmspace.h:176
prog_void
void PROGMEM prog_void
Definition: pgmspace.h:140
prog_int64_t
int64_t PROGMEM prog_int64_t
Definition: pgmspace.h:305
memcpy_PF
void * memcpy_PF(void *dest, uint_farptr_t src, size_t len)
Copy a memory block from flash to SRAM.
int16_t
signed int int16_t
Definition: stdint.h:85
int8_t
signed char int8_t
Definition: stdint.h:75
io.h
strcpy_PF
char * strcpy_PF(char *dest, uint_farptr_t src)
Duplicate a string.
strcat_PF
char * strcat_PF(char *dest, uint_farptr_t src)
Concatenates two strings.
PROGMEM
#define PROGMEM
Definition: pgmspace.h:109
strlcat_PF
size_t strlcat_PF(char *dst, uint_farptr_t src, size_t siz)
Concatenate two strings.
strncat_PF
char * strncat_PF(char *dest, uint_farptr_t src, size_t len)
Concatenate two strings.
int32_t
signed long int int32_t
Definition: stdint.h:95
strstr_PF
char * strstr_PF(const char *s1, uint_farptr_t s2)
Locate a substring.
strlcpy_PF
size_t strlcpy_PF(char *dst, uint_farptr_t src, size_t siz)
Copy a string from progmem to RAM.
prog_int8_t
int8_t PROGMEM prog_int8_t
Definition: pgmspace.h:194
memcmp_PF
int memcmp_PF(const void *, uint_farptr_t, size_t) __ATTR_PURE__
Compare memory areas.
strcmp_PF
int strcmp_PF(const char *s1, uint_farptr_t s2) __ATTR_PURE__
Compares two strings.
uint8_t
unsigned char uint8_t
Definition: stdint.h:80
uint_farptr_t
uint32_t uint_farptr_t
Definition: inttypes.h:81
uint32_t
unsigned long int uint32_t
Definition: stdint.h:100
inttypes.h
strncpy_PF
char * strncpy_PF(char *dest, uint_farptr_t src, size_t len)
Duplicate a string until a limited length.
prog_char
char PROGMEM prog_char
Definition: pgmspace.h:158
prog_uint16_t
uint16_t PROGMEM prog_uint16_t
Definition: pgmspace.h:248
uint64_t
unsigned long long int uint64_t
Definition: stdint.h:114
strncmp_PF
int strncmp_PF(const char *s1, uint_farptr_t s2, size_t n) __ATTR_PURE__
Compare two strings with limited length.
strlen_PF
size_t strlen_PF(uint_farptr_t src)
Obtain the length of a string.
uint16_t
unsigned int uint16_t
Definition: stdint.h:90
prog_uint8_t
uint8_t PROGMEM prog_uint8_t
Definition: pgmspace.h:212
strnlen_PF
size_t strnlen_PF(uint_farptr_t src, size_t len)
Determine the length of a fixed-size string.
strcasecmp_PF
int strcasecmp_PF(const char *s1, uint_farptr_t s2) __ATTR_PURE__
Compare two strings ignoring case.
strncasecmp_PF
int strncasecmp_PF(const char *s1, uint_farptr_t s2, size_t n) __ATTR_PURE__
Compare two strings ignoring case.
prog_int16_t
int16_t PROGMEM prog_int16_t
Definition: pgmspace.h:230
