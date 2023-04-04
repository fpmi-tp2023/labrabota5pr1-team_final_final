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

TEST(TestHashPassword, TestAdminPassword)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_EQ(ctrl.hashPassword("admin"), "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918");
}

TEST(TestHashPassword, TestHashCorrectness)
{
    Controller ctrl;
    EXPECT_NE(ctrl.hashPassword("abacaba"), ctrl.hashPassword("bacabaca"));
}

TEST(TestCorrectPassword, TestAdminCorrect)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_TRUE(ctrl.correctPassword("admin", "admin"));
}

TEST(TestCorectPassword, TestAdminIncorrect)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_FALSE(ctrl.correctPassword("admin", "abacaba"));
    EXPECT_FALSE(ctrl.correctPassword("admin", "bacabaca"));
}
