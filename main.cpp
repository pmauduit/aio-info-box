#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <sstream>

#include "ourhome.h"

int main(int argc, char ** argv) {
  OurHome ou;
  ou.login("login", "password");

  std::cout << " BLACK POILUS -- " << std::endl;

  std::cout << std::endl << "== Chores (TO-DO)" << std::endl;
  ou.getChores();

  std::cout << std::endl << "== Groceries (shopping list)" << std::endl;
  ou.getShoppingList();

  ou.logout();
  return 0;
}
