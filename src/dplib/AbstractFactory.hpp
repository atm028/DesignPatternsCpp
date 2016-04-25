#include <string>
#include <memory>

// Product classes
class BaseProduct {
    protected:
        std::string mName;
    public:
        BaseProduct() : mName("") {}
        BaseProduct(const std::string& name) : mName(name) {}
        virtual ~BaseProduct() {}
        std::string getName() { return mName; }
};


class Product1 : public BaseProduct {
    public:
        Product1() : BaseProduct("Product1") {}        
        int getOption() { return 10; }
};

class Product2 : public BaseProduct {
    public:
        Product2() : BaseProduct("Product2") {}        
        int getVersion() { return 10; }
};

//Factory
class ProductFactory {
    public:
        int getVersion();
        std::shared_ptr<BaseProduct> cProduct1() { return std::make_shared<BaseProduct>(Product1()); }
        std::shared_ptr<BaseProduct> cProduct2() { return std::make_shared<BaseProduct>(Product2()); }
};
