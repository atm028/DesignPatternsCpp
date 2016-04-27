#include <string>
#include <map>
#include <vector>
#include <memory>

enum OSAttrType {
    OSName,
    OSShell,
    OSRootPath,
    MaxType
};

struct OSAttrSet {
    OSAttrType type;
    std::string attr;
};

OSAttrSet winAttrSet[OSAttrType::MaxType] = {
    { OSShell,      "cmd"   },
    { OSRootPath,   "C:/"   }
};
OSAttrSet linuxAttrSet[OSAttrType::MaxType] = {
    { OSShell,      "bash"  },
    { OSRootPath,   "/"     }
};

class OSPackage {
    public:
        OSPackage(
                const std::string& type, 
                const std::string& shell,
                const std::string& root) : mOSType(type), mShell(shell), mRoot(root) {}
        std::string getName     () { return mName; }
        std::string getOSType   () { return mOSType; }
        std::string getShell    () { return mShell; }
        std::string getFile     () {
            std::string ret(mRoot);
            ret.append(mFile);
            return ret;
        }

        void setName     (const std::string& v) { mName = v; }
        void setFile     (const std::string& v) { mFile = v; }
        
    protected:
        std::string mName, mOSType, mShell, mFile, mRoot;
};

class Builder {
    public:
        auto getPackage() { return mPackage; }
        virtual void addItem(const std::string& name) = 0;

    protected:
        std::map< std::string, std::shared_ptr<OSPackage> > mPackage;
};

class WindowsBuilder : public Builder {
    public:
        virtual void addItem(const std::string& name) {
            if(mPackage.end() == mPackage.find(name)) {
                mPackage[name] = std::make_shared<OSPackage>(OSPackage("Windows", "cmd", "C:/"));
                mPackage[name]->setName(name);
            }
        }
};

class LinuxBuilder :public Builder {
    public:
        virtual void addItem(const std::string& name) {
            if(mPackage.end() == mPackage.find(name)) {
                mPackage[name] = std::make_shared<OSPackage>(OSPackage("Linux", "bash", "/"));
                mPackage[name]->setName(name);
            }
        }
};

class Reader {
    public:
        auto getPark(std::vector<std::string> names) {
            std::vector<std::shared_ptr< OSPackage> > ret;
            std::for_each(names.begin(), names.end(), [&](auto n){ mBuilder->addItem(n); });
            return mBuilder->getPackage();
        }

        void setBuilder(std::shared_ptr<Builder> builder) { mBuilder = builder; }
    protected:
        std::shared_ptr<Builder> mBuilder;
};
