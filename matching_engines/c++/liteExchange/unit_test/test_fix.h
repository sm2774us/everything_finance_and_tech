#include <fix/fix_message.h>
#include <fix/fix_constants.h>
#include <core/datetime_utility.h>
#include <core/string_utility.h>
#include <string>

TEST(Fix, FixMessage)
{
    fix::FixMessage fixMessage;

    fixMessage.setFixVersion(fix::FixConstants::FixVersion::FIX_4_2);
    fixMessage.setMessageType(fix::FixConstants::MessageType::NEW_ORDER);
    fixMessage.setSenderCompId("SENDER");
    fixMessage.setTargetCompId("TARGET");
    fixMessage.setTag(fix::FixConstants::FIX_TAG_SENDING_TIME, core::getUtcDatetime(core::Subseconds::MICROSECONDS));

    std::string fixString;

    fixMessage.toString(fixString);

    EXPECT_TRUE(core::contains(fixString, "35=D"));
    EXPECT_TRUE(core::contains(fixString, "8=FIX.4.2"));
    EXPECT_TRUE(core::contains(fixString, "49=SENDER"));
    EXPECT_TRUE(core::contains(fixString, "56=TARGET"));
}