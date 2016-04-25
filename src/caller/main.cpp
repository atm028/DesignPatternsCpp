#include "google/gtest/gtest.h"
#include <iostream>

#include "ConnectionClassTest.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
