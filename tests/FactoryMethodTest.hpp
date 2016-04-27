#include "gtest/gtest.h"

#include "FactoryMethod.hpp"

TEST(FactoryMethodCreateProduct1, Normal) {
    std::vector<std::string> productNames {"red1", "red2"};
    CCreator1 cr;
    std::for_each(productNames.begin(), productNames.end(), [&](auto n){
            FactoryMethodProduct* product = cr.factoryMethod(n);
            EXPECT_EQ(n, product->getName());
            EXPECT_EQ(EPR1, product->getType());
            });
}

TEST(FactoryMethodCreateProduct2, Normal) {
    std::vector<std::string> productNames {"green1", "green2"};
    CCreator2 cr;
    std::for_each(productNames.begin(), productNames.end(), [&](auto n){
            FactoryMethodProduct* product = cr.factoryMethod(n);
            EXPECT_EQ(n, product->getName());
            EXPECT_EQ(EPR2, product->getType());
            });
}

TEST(FactoryMethodCreateSetOfProducts, Normal) {
    std::vector< std::vector<std::string> > productNames {{"red1", "red2"}, {"green1", "green2"}};
    std::vector<std::shared_ptr<Creator*> > cr {
        std::make_shared<Creator*>(dynamic_cast<Creator*>(new CCreator1())),
        std::make_shared<Creator*>(dynamic_cast<Creator*>(new CCreator2()))
    };

    int nameCounter = 0;
    std::for_each(cr.begin(), cr.end(), [&](std::shared_ptr<Creator*> c){
            std::for_each(productNames[nameCounter].begin(), productNames[nameCounter].end(), [&](auto n){
                    //TODO: Fix some little mess with pointers
                    Creator* cr = *(c.get());
                    FactoryMethodProduct* product = cr->factoryMethod(n);
                    EXPECT_EQ(n, product->getName());
                    });
                    nameCounter++;
            });
}
