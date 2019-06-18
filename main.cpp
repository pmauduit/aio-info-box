#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <sstream>

#include "ourhome.h"

int main(int argc, char ** argv) {
  OurHome ou;

  std::cout << " BLACK POILUS -- " << std::endl;

  ou.login("email", "password");

  std::cout << std::endl << "== Chores (TO-DO)" << std::endl;
  ou.getChores();

  std::cout << std::endl << "== Groceries (shopping list)" << std::endl;
  ou.getShoppingList();

  return 0;
}
