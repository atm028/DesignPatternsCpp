#include <vector>
#include <string>

enum PR_TYPE {
    EPR1,
    EPR2
};

class FactoryMethodProduct {
    public:
        FactoryMethodProduct(const std::string& n) : mName(n) {}
        virtual std::string getName() { return mName; }
        virtual PR_TYPE getType() = 0;
    protected:
        std::string mName;
};

class Creator {
    public:
        virtual FactoryMethodProduct* factoryMethod(const std::string& name) = 0;
};

class FactoryMethodProduct1 : public FactoryMethodProduct {
    public:
        FactoryMethodProduct1(const std::string& name) : FactoryMethodProduct(name) {}
        PR_TYPE getType() { return EPR1; }
};

class FactoryMethodProduct2 : public FactoryMethodProduct {
    public:
        FactoryMethodProduct2(const std::string& name) : FactoryMethodProduct(name) {}
        PR_TYPE getType() { return EPR2; }
};

class CCreator1 : public Creator {
    public:
        FactoryMethodProduct* factoryMethod(const std::string& name) { 
            return dynamic_cast<FactoryMethodProduct*>(new FactoryMethodProduct1(name)); 
        }
};

class CCreator2 : public Creator {
    public:
        FactoryMethodProduct* factoryMethod(const std::string& name) { 
            return dynamic_cast<FactoryMethodProduct*>(new FactoryMethodProduct2(name)); 
        }
};

