/**
 * #include <stdio.h>
 *
 * FILE* popen(const char* command, const char* type);
 * int pclose(FILE* stream);
 */

#include <cstring>
#include <stdio.h>
#include <string>

int main() {
  char buf[1024];
  fgets(buf, 1024, stdin);

  size_t n = strlen(buf);
  if (buf[n - 1] == '\n') {
    --n;
  }

  std::string command = std::string("cat ") + buf;
  FILE* fp = popen(command.data(), "r");
  while (fgets(buf, 1024, fp) != nullptr) {
    fputs(buf, stdout);
  }

  pclose(fp);

  return 0;
}