#ifndef __BUTTON_H
#define __BUTTON_H

#define THRESHOLD 100


class GPIOButton {
  public:
    GPIOButton();
    ~GPIOButton() {};
    void acknowledge();
    bool isPressed() { return pressed; };
    void setPressed(const bool & _pressed) { pressed = _pressed; }; 


  private:
   // the schematic is misleading but on the following schema:
   // https://raw.githubusercontent.com/ppelleti/hs-wiringPi/master/pin-diagram.png
   // the "Wpi*" have to be considered instead of the other numbering schemes.
   bool pressed;
};

#endif // __BUTTON_H
