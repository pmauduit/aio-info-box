#include <unistd.h>

#include <iostream>
#include <cstring>
#include <sstream>

#include "ourhome.h"
#include "button.h"
#include "LiquidCrystal_I2C.h"

#define SLEEP_POLL 10000
#define LC_ADDRESS 0x27 // or 0x3f, YMMV


extern GPIOButton * registeredButton;

bool waitOrInterrupt(unsigned int duration) {
    for (unsigned int i = 0 ; i < duration ; i += SLEEP_POLL) {
      if (registeredButton->isPressed()) {
	 registeredButton->acknowledge();
	 return true;
      }
      usleep(SLEEP_POLL);
    }
    return false;
}

void display_shift (LiquidCrystal_I2C * const LCD, const std::string &a, const std::string &b) {
    std::string copy1(a);
    std::string copy2(b);

    do {
	// first line
	LCD->clear();
        LCD->write(copy1.substr(0,16).c_str());
	if (copy1.length() >= 16) {
	    copy1.erase(0, 1);
        }
	// second line
	LCD->setCursor(0,1);
	LCD->write(copy2.substr(0,16).c_str());
	if (copy2.length() >= 16) {
	    copy2.erase(0, 1);
        }
	if (waitOrInterrupt(400000)) {
	  return;
	};
    } while ((copy1.length() >= 16) || (copy2.length() >= 16));
    waitOrInterrupt(1000000);
    return;
}

void init_lcd(LiquidCrystal_I2C * const LCD) {
    LCD->write("- BLACK POILUS -");
    LCD->setCursor(0, 1);
    LCD->write("                ");
}

int main(int argc, char ** argv) {
  OurHome ou;
  LiquidCrystal_I2C * LCD = new LiquidCrystal_I2C(LC_ADDRESS, 16, 2);
  init_lcd(LCD);
  GPIOButton btn;

  ou.login("login", "password");
 

  while (true) {
    std::list<std::string> chores = ou.getChores();
    std::list<std::string> shoppingList = ou.getShoppingList();
    int i = 1;
    for (auto const & chore : chores) {
      std::stringstream currentChore;
      std::stringstream title;
      title << "TODO (" << i << "/" << chores.size() << ")";
      currentChore <<  "" << i << ". " << chore;
      display_shift(LCD, title.str(), currentChore.str());
      i++;
    }

    i = 1;
    for (auto const & item : shoppingList) {
      std::stringstream currentItem;
      std::stringstream title;
      title << "COURSES (" << i << "/" << shoppingList.size() << ")";
      currentItem << "" << i << ". " << item;
      display_shift(LCD, title.str(), currentItem.str());
      i++;
    }
  }
  ou.logout();
  return 0;
}
