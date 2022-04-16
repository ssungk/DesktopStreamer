#include "sc/ScreenCapturer.h"

int main(int argc, char* argv[])
{
  auto sc = std::make_shared<ds::ScreenCapturer>();
  sc->Run();
  return 0;
}