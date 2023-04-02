#include <gtest/gtest.h>
#include "controller.h"

TEST(TestConnection, TestPositive)
{
    Controller ctrl;
    EXPECT_TRUE(ctrl.connectDB("RecordStore.db"));
}

TEST(TestConnection, TestNegative)
{
    Controller ctrl;
    EXPECT_FALSE(ctrl.connectDB("RecordStoreee.db"));
}

TEST(TestResponceAuth, TestSignInOrSignUp)
{
    Controller ctrl;
    EXPECT_TRUE(ctrl.validResponseAuth("sign-in"));
    EXPECT_TRUE(ctrl.validResponseAuth("sign-up"));
}

TEST(TestResponceAuth, TestOther)
{
    Controller ctrl;
    EXPECT_FALSE(ctrl.validResponseAuth("Sign-in"));
    EXPECT_FALSE(ctrl.validResponseAuth("sign in"));
    EXPECT_FALSE(ctrl.validResponseAuth("sign off"));
}

TEST(TestExistingLogin, TestAdmin)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_TRUE(ctrl.existingLogin("admin"));
}

TEST(TestGetRole, TestAdminRole)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_EQ(ctrl.getRole("admin"), std::string("admin"));
}
