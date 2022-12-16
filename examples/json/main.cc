#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"
using json = nlohmann::json;
int main() {
  std::ifstream f("/Volumes/code/daily/examples/json/person.json");
  json person = json::parse(f);
  std::cout << person << std::endl;
  return 0;
}