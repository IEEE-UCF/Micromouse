#include <unity.h>

#include "encoders.h"

void setUp(void) {}

void tearDown(void) {}

void test_framework_sanity(void) { TEST_ASSERT_EQUAL(1, 1); }

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_framework_sanity);

  return UNITY_END();
}
