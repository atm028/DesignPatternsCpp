#include "gtest/gtest.h"

#include "Builder.hpp"

TEST(BuildNumebrOfProduct1, Normal) {
    std::vector<std::string> winParkNames {"red1", "red2"};
    std::vector<std::string> linParkNames {"green1", "green2"};
    std::shared_ptr<WindowsBuilder> wb(new WindowsBuilder());
    std::shared_ptr<LinuxBuilder> lb(new LinuxBuilder());
    Reader r;

    r.setBuilder(wb);
    auto wp = r.getPark(winParkNames);

    r.setBuilder(lb);
    auto lp = r.getPark(linParkNames);

    auto it = wp.begin();
    for(int i = 0; i < wp.size(); i++) {
        it->second->setFile("a.exe");
        EXPECT_EQ(it->second->getName(), winParkNames[i]);
        EXPECT_EQ(it->second->getOSType(), "Windows");
        EXPECT_EQ(it->second->getShell(), "cmd");
        EXPECT_EQ(it->second->getFile(), "C:/a.exe");
        ++it;
    }

    it = lp.begin();
    for(int i = 0; i < lp.size(); i++) {
        it->second->setFile("a");
        EXPECT_EQ(it->second->getName(), linParkNames[i]);
        EXPECT_EQ(it->second->getOSType(), "Linux");
        EXPECT_EQ(it->second->getShell(), "bash");
        EXPECT_EQ(it->second->getFile(), "/a");
        ++it;
    }
}
