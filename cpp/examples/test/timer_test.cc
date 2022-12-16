#include "cpp/examples/timer.h"

#include "gtest/gtest.h"

#include <unistd.h>

TEST(Timer, Timer) {
  {
    examples::Timer t;
    sleep(2);
    long du = t.s();
    ASSERT_EQ(du, 2);
  }
}