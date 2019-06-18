#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <sstream>

#include "ourhome.h"

int main(int argc, char ** argv) {
  OurHome ou;

  ou.login("email", "password");
  ou.getChores();

  return 0;
}
