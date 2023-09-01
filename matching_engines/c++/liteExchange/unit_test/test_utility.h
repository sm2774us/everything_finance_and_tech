#include <core/configuration.h>
#include <core/single_instance.h>
#include <core/string_utility.h>
#include <core/concurrency/thread.h>
#include <string>

TEST(Utility, SingleInstance)
{
    core::SingleInstance singleton;

    bool b = singleton();

    EXPECT_TRUE(b==true);
}

TEST(Utility, ConfigFile)
{
    core::Configuration x;
    core::Configuration::loadFromFile("./test_config.txt", x);

    string val;
    val = x.getStringValue("THREAD_STACK_SIZE");

    EXPECT_STREQ("0", val.c_str()); // string equal
}

TEST(Utility, ReplaceInString)
{
    std::string orig = "xxxyyxx";
    core::replaceInString(orig, "yy", "jjj");
    EXPECT_STREQ("xxxjjjxx", orig.c_str()); // string equal
}