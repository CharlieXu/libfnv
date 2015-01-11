#include <fnv.h>
#include <fnvutil.h>
#include <stdio.h>

void updateHash1024(void *context, void *buf, uint64_t length)
{
  uint64_t *hval = (uint64_t *) context;
  fnv1024UpdateBuffer(hval, buf, length);
}

void printVersion(void)
{
  printf("fnv1024 version 0.8.1\n");
}

void printHelp(void)
{
  printf("Usage:\n\nfnv1024 [FILE]...\n\n"
         "With no arguments it will read from standard input.\n"
         "This command will print the FNV-1a 1024-bit hash for each listed file in order.\n");
}

int main(int argc, char **argv)
{
  uint64_t hval[16];
  int i;
  int didSomething = 0;
  char result[257];
  int nomoreopts = 0;
  for (i = 1; i < argc; ++i) {
    char *cur = argv[i];
    if (cur[0] == '-' && !nomoreopts) {
      if (cur[1] == '-' && cur[2] == '\0') {
        nomoreopts = 1;
        continue;
      }
      if (cur[1] == 'h' || (cur[2] == 'h' && cur[1] == '-')) {
        printHelp();
        exit(0);
      }
      if (cur[1] == 'v' || (cur[2] == 'v' && cur[1] == '-')) {
        printVersion();
        exit(0);
      }
      printf("Error: unrecognized option: %s\n", cur);
      exit(1);
    }
    fnv1024Init(hval);
    fnvIterateThroughFile(cur, updateHash1024, hval);
    fnv1024ResultHex(result, hval);
    printf("%s\n", result);
    didSomething = 1;
  }
  if (!didSomething) {
    fnv1024Init(hval);
    fnvIterateThroughFile("-", updateHash1024, hval);
    fnv1024ResultHex(result, hval);
    printf("%s\n", result);
  }
  return 0;
}