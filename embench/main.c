/* Common main.c for the benchmarks

   Copyright (C) 2014 Embecosm Limited and University of Bristol
   Copyright (C) 2018-2019 Embecosm Limited

   Contributor: James Pallister <james.pallister@bristol.ac.uk>
   Contributor: Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of Embench and was formerly part of the Bristol/Embecosm
   Embedded Benchmark Suite.

   SPDX-License-Identifier: GPL-3.0-or-later */

#include "support.h"
#include <stdarg.h>
#include <stdint.h>

#define STDOUT 0xFFFF0000

#ifndef FAULTLESS
#define FAULTLESS 0
#endif

#define SUCCESS 0xFF
#define FAIL    0x01

/*
__asm (".section .text_init, \"ax\"");
__asm (".global _start");
__asm ("_start:");

// Enable Caches in MRAC
__asm ("li t0, 0x5f555555");
__asm ("csrw 0x7c0, t0");

//__asm ("la t0, FAULTLESS");
//__asm ("csrw 0x8ff, t0");

// Set stack pointer.
__asm ("la sp, STACK");

__asm ("call main");

// Write 0xff to STDOUT for TB to termiate test.
__asm (".global _finish");
__asm ("_finish:");
__asm ("li t0, 0xd0580000");
__asm ("addi t1, zero, 0xff");
__asm ("sb t1, 0(t0)");
__asm ("beq x0, x0, _finish");
__asm (".rept 10");
__asm ("nop");
__asm (".endr");
__asm (".section .text");
*/
#include <time.h>
typedef clock_t CORE_TICKS;
#define EE_TICKS_PER_SEC 1000
#define MYTIMEDIFF(fin,ini) ((fin)-(ini))

uint32_t start_time;
uint32_t stop_time;

CORE_TICKS get_time(void) {
        CORE_TICKS elapsed=(CORE_TICKS)(MYTIMEDIFF(stop_time, start_time));
        return elapsed;
}

unsigned int time_in_secs(CORE_TICKS ticks) {
        unsigned int retval=((unsigned int)ticks) / (unsigned int)EE_TICKS_PER_SEC;
        return retval;
}

// printf for SweRV
static int
whisperPutc(char c)
{
//  __whisper_console_io = c;
//  __whisper_console_io = c;
  *(volatile char*)(STDOUT) = c;
  return c;
}

static int
whisperPuts(const char* s)
{
  while (*s)
    whisperPutc(*s++);
  return 1;
}


static int
whisperPrintDecimal(int value)
{
  char buffer[20];
  int charCount = 0;

  unsigned neg = value < 0;
  if (neg)
    {
      value = -value;
      whisperPutc('-');
    }

  do
    {
      char c = '0' + (value % 10);
      value = value / 10;
      buffer[charCount++] = c;
    }
  while (value);

  char* p = buffer + charCount - 1;
  for (unsigned i = 0; i < charCount; ++i)
    whisperPutc(*p--);

  if (neg)
    charCount++;

  return charCount;
}

static int
whisperPrintInt(int value, int base)
{
  if (base == 10)
    return whisperPrintDecimal(value);

  char buffer[20];
  int charCount = 0;

  unsigned uu = value;

  if (base == 8)
    {
      do
        {
          char c = '0' + (uu & 7);
          buffer[charCount++] = c;
          uu >>= 3;
        }
      while (uu);
    }
  else if (base == 16)
    {
      do
        {
          int digit = uu & 0xf;
          char c = digit < 10 ? '0' + digit : 'a' + digit - 10;
          buffer[charCount++] = c;
          uu >>= 4;
        }
      while (uu);
    }
  else
    return -1;

  char* p = buffer + charCount - 1;
  for (unsigned i = 0; i < charCount; ++i)
    whisperPutc(*p--);

  return charCount;
}


int
whisperPrintfImpl(const char* format, va_list ap)
{
  int count = 0;  // Printed character count

  for (const char* fp = format; *fp; fp++)
    {
      if (*fp != '%')
        {
          whisperPutc(*fp);
          ++count;
          continue;
        }

      ++fp;  // Skip %

      if (*fp == 0)
        break;

      if (*fp == '%')
        {
          whisperPutc('%');
          continue;
        }

      if (*fp == '-')
        {
          fp++;  // Pad right not yet implemented.
        }

      while (*fp == '0')
        {
          fp++;  // Pad zero not yet implented.
        }

      if (*fp == '*')
        {
          int width = va_arg(ap, int);
          fp++;  // Width not yet implemented.
        }
      else
        {
          while (*fp >= '0' && *fp <= '9')
            ++fp;   // Width not yet implemented.
        }

      switch (*fp)
        {
        case 'd':
          count += whisperPrintDecimal(va_arg(ap, int));
          break;

        case 'u':
          count += whisperPrintDecimal((unsigned) va_arg(ap, unsigned));
          break;

        case 'x':
        case 'X':
          count += whisperPrintInt(va_arg(ap, int), 16);
          break;

        case 'o':
          count += whisperPrintInt(va_arg(ap, int), 8);
          break;

        case 'c':
          whisperPutc(va_arg(ap, int));
          ++count;
          break;

        case 's':
          count += whisperPuts(va_arg(ap, char*));
          break;
        }
    }

  return count;
}

int
whisperPrintf(const char* format, ...)
{
  va_list ap;

  va_start(ap, format);
  int code = whisperPrintfImpl(format, ap);
  va_end(ap);

  return code;
}


int
swerv_printf(const char* format, ...)
{
  va_list ap;

  va_start(ap, format);
  int code = whisperPrintfImpl(format, ap);
  va_end(ap);

  return code;
}

// Simple RV32IM-safe float print (3 decimal digits)
void print_float(float f)
{
    if (f < 0) {
        whisperPutc('-');
        f = -f;
    }

    int int_part = (int)f;               // integer part
    int frac_part = (int)((f - int_part) * 1000 + 0.5f); // 3 decimals

    whisperPrintDecimal(int_part);
    whisperPutc('.');
    if (frac_part < 100) whisperPutc('0');
    if (frac_part < 10)  whisperPutc('0');
    whisperPrintDecimal(frac_part);
}

// Read the hardware cycle counter
int barebones_clock()
{
    unsigned int* hardwareCounterAddr = (unsigned int*)0xFFFFFF00; // memory-mapped counter
    return *hardwareCounterAddr;
}

static inline int get_insts_count(void)
{
    return *(volatile int*)0xFFFFFF10;
}

static inline int get_jump_insts_count(void)
{
    return *(volatile int*)0xFFFFFF20;
}

static inline int get_mispred_count(void)
{
    return *(volatile int*)0xFFFFFF30;
}


int __attribute__ ((used))
main (int argc __attribute__ ((unused)),
      char *argv[] __attribute__ ((unused)))
{
  int i;
  volatile int result;
  int correct;

  swerv_printf("EMbench init...!\n");

  initialise_benchmark ();
  //warm_caches (WARMUP_HEAT);

  swerv_printf("Starting benchmark...\n");
  int Begin_Time = barebones_clock();
  int mispred_start = get_mispred_count();
  int j_inst_start = get_jump_insts_count();
  int inst_start = get_insts_count();

  result = benchmark ();

  int End_Time = barebones_clock();
  int mispred_end = get_mispred_count();
  int j_inst_end = get_jump_insts_count();
  int inst_end = get_insts_count();

  swerv_printf("Time: %d\n", End_Time - Begin_Time);
  swerv_printf("Instructions: %d\n", inst_end - inst_start);
  swerv_printf("Jump/Branch Instructions: %d\n", j_inst_end - j_inst_start);
  swerv_printf("Mispredictions: %d\n", mispred_end - mispred_start);
  swerv_printf("Result: %d\n", result);


  swerv_printf("Verifying benchmark...\n");
  correct = verify_benchmark (result);
  swerv_printf("Verification result: %d\n", correct);
  swerv_printf("Returning...\n");

  if (correct)
    return SUCCESS;
  else
    return FAIL;
  //return (!correct);
}				/* main () */


/*
   Local Variables:
   mode: C
   c-file-style: "gnu"
   End:
*/
