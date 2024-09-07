#include <emscripten.h>

#include <cwasm.h>

EMSCRIPTEN_KEEPALIVE
int thing(int a) { return a * 2; }

int main() {}