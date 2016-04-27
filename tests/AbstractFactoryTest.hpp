#include "gtest/gtest.h"
#include <vector>
#include <algorithm>

#include "AbstractFactory.hpp"

TEST(CreateTwoProducts, Normal) {
    ProductFactory f;
    std::shared_ptr<BaseProduct> p1(f.cProduct1());
    std::shared_ptr<BaseProduct> p2(f.cProduct2());

    EXPECT_EQ("Product1", p1->getName());
    EXPECT_EQ("Product2", p2->getName());
}

TEST(CheckSharedPointerCast, Normal) {
    ProductFactory f;
    std::shared_ptr<Product1> p1(std::dynamic_pointer_cast<Product1>(f.cProduct1()));
    std::shared_ptr<Product2> p2(std::dynamic_pointer_cast<Product2>(f.cProduct1()));
    EXPECT_EQ(10, p1->getOption());
    EXPECT_EQ(10, p2->getVersion());
}

TEST(ArrayOfObjects, Normal) {
    ProductFactory f;
    std::vector<std::shared_ptr<BaseProduct> > v;
    for(int i = 0; i < 5; i++) v.push_back(f.cProduct1());
    std::for_each(v.begin(), v.end(), [](std::shared_ptr<BaseProduct> p){
            EXPECT_EQ("Product1", p->getName());
            });
}

TEST(CheckPointerCast, Normal) {
    BaseProduct* pr = new Product1();
    int op = (dynamic_cast<Product1*>(pr))->getOption();
    EXPECT_EQ(10, op);
}
