#include <iostream>
#include "gtest/gtest.h"

#include "AbstractFactoryTest.hpp"
#include "BuilderTest.hpp"
#include "FactoryMethodTest.hpp"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
