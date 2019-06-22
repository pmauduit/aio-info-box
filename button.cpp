#include <iostream>
#include <chrono>
#include <iostream>
#include <wiringPi.h>

#include "button.h"

static uint64_t now = 1, last = 0;

GPIOButton::GPIOButton() {
  wiringPiSetup();
  pinMode(16, INPUT);
  pullUpDnControl(16, PUD_DOWN);
  wiringPiISR (16, INT_EDGE_FALLING, this->buttonPressed) ;
}

void GPIOButton::buttonPressed(void) {
  now = std::chrono::duration_cast<std::chrono::milliseconds>(
		    std::chrono::system_clock::now().time_since_epoch()
		  ).count();
  if (now - last > THRESHOLD) {
    std::cout << "Button pressed" << std::endl;
    last = now;
  }
}


