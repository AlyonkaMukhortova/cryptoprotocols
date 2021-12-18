#include "protocol_funcs.h"





int main(int argc, char** argv) {
  int v = strcmp(argv[1], "-v");
  int err = file_checker(argv[argc - 1]);
  if (err == WRONG_VALUE) {
    printf("Invalid file!\n");
  }
  else{
    printf("Valid file!\n");
    int a = cracker(argv[argc - 1], v);

    return 0;
  }
}
