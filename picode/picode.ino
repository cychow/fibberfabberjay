/*
  This example plays a raw, headerless, mono 16b, 44.1 sample using the PWMAudio library on GPIO 1.

  Released to the public domain by Earle F. Philhower, III <earlephilhower@yahoo.com>
*/

#include <PWMAudio.h>
//#include "wav.h"

// wait time in seconds
#define WAIT_TIME random(25, 41)
//#include "ALLIANCES-Violet.h"
//#include "Band.h"
#include "Camouflage.h"
//#include "Colony.h"
//#include "Combat.h"
//#include "Compass.h"
//#include "Defiance.h"
//#include "Dissent.h"
//#include "Fire.h"
//#include "FirstAid.h"
//#include "Food.h"
////#include "GROUPOFBIRDS-Green.h"
//#include "Gaggle.h"
//#include "Hide.h"
//#include "Host.h"
//#include "Murder.h"
//#include "Overthrow.h"
//#include "Parliament.h"
//#include "Protest.h"
//#include "REBELLION-Grey.h"
//#include "SURVIVE-Red.h"
//#include "Safety.h"
//#include "Shelter.h"
//#include "Sponsors.h"
//#include "Stealth.h"
//#include "Trap.h"
//#include "Uprising.h"
//#include "Water.h"
//#include "out.h"

#define LED_PIN 25

// The sample pointers
const int16_t *start = (const int16_t *)out_raw;
const int16_t *p = start;

// Create the PWM audio device on GPIO 1.   Hook amp/speaker between GPIO1 and convenient GND.
PWMAudio pwm(1);

unsigned int count = 0;
unsigned int waiting = 0;
unsigned int led_status = 0;
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
      if (elapsed_time > WAIT_TIME*100*1000) {
        waiting = 0;
      }
    }
    if (count >= sizeof(out_raw)) {
      // jump to beginning
      last_end_time = micros();
      count = 0;
      p = start;
      waiting = 1;
      led_status = !led_status;
      digitalWrite(LED_PIN, led_status);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pwm.onTransmit(cb);
  pwm.begin(44100);
  randomSeed(analogRead(A0));
}

void loop() {
  /* noop, everything is done in the CB */
}

