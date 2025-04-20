/*
  This example plays a raw, headerless, mono 16b, 44.1 sample using the PWMAudio library on GPIO 1.

  Released to the public domain by Earle F. Philhower, III <earlephilhower@yahoo.com>
*/

#include <PWMAudio.h>
//#include "wav.h"
#include "out.h"

// The sample pointers
const int16_t *start = (const int16_t *)out_raw;
const int16_t *p = start;

// Create the PWM audio device on GPIO 1.   Hook amp/speaker between GPIO1 and convenient GND.
PWMAudio pwm(1);

unsigned int count = 0;
unsigned int waiting = 0;
volatile unsigned int current_time = 0;
volatile unsigned int last_end_time = 0;
volatile unsigned int elapsed_time = 0;

void cb() {
  while (pwm.availableForWrite()) {
    if (!waiting) {
      pwm.write(*p++);
      count += 2;
    } else {
      current_time = micros();
      elapsed_time = current_time - last_end_time;
      pwm.write(0x0000);
      if (elapsed_time > 1000000) {
        waiting = 0;
      }
    }
    if (count >= sizeof(out_raw)) {
      // jump to beginning
      last_end_time = micros();
      count = 0;
      p = start;
      waiting = 1;
    }
  }
}

void setup() {
  pwm.onTransmit(cb);
  pwm.begin(44100);
}

void loop() {
  /* noop, everything is done in the CB */
}
