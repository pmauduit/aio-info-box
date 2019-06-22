#include <iostream>
#include <chrono>
#include <wiringPi.h>

#include "button.h"

static uint64_t now = 1, last = 0;
GPIOButton * registeredButton;

void buttonPressed(void) {
  now = std::chrono::duration_cast<std::chrono::milliseconds>(
		    std::chrono::system_clock::now().time_since_epoch()
		  ).count();
  if (now - last > THRESHOLD) {
    registeredButton->setPressed(TRUE);
    last = now;
  }
}

void GPIOButton::acknowledge(void) { pressed = FALSE; };

GPIOButton::GPIOButton(): pressed(FALSE) {
  wiringPiSetup();
  pinMode(16, INPUT);
  pullUpDnControl(16, PUD_DOWN);
  wiringPiISR (16, INT_EDGE_FALLING, buttonPressed) ;
  registeredButton = this;
}


