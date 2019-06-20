#include <unistd.h>

#include <iostream>
#include <cstring>
#include <sstream>

#include "ourhome.h"
#include "LiquidCrystal_I2C.h"

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
	usleep(400000);
    } while ((copy1.length() >= 16) || (copy2.length() >= 16));
    sleep(1);
    return;
}

void init_lcd(LiquidCrystal_I2C * const LCD) {
    LCD->write("- BLACK POILUS -");
    LCD->setCursor(0, 1);
    LCD->write("                ");
}

int main(int argc, char ** argv) {
  OurHome ou;
  LiquidCrystal_I2C * LCD = new LiquidCrystal_I2C(0x3f, 16, 2);
  init_lcd(LCD);
  ou.login("login", "password");

  std::cout << " BLACK POILUS -- " << std::endl;

  std::cout << std::endl << "== Chores (TO-DO)" << std::endl;
  std::list<std::string> chores = ou.getChores();

  int i = 1;
  for (auto const & chore : chores) {
    std::stringstream currentChore;
    currentChore <<  "" << i << ". " << chore;
    display_shift(LCD, "- TODO", currentChore.str());
    i++;
    sleep(1);
  }
  std::cout << std::endl << "== Groceries (shopping list)" << std::endl;
  std::list<std::string> shoppingList = ou.getShoppingList();
  i = 1;
  std::cout << "shoppingList size: " << shoppingList.size() << std::endl;
  for (auto const & item : shoppingList) {
    std::stringstream currentItem;
    currentItem << "" << i << ". " << item;
    display_shift(LCD, "- To Buy", currentItem.str());
    i++;
    sleep(1);
  }
  ou.logout();
  return 0;
}
