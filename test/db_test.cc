#include <gtest/gtest.h>
#include "controller.h"
#include <sstream>

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

TEST(TestExistingLogin, TestOther)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_FALSE(ctrl.existingLogin("akdjhsdg"));
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

TEST(TestPasswordMatch, TestWhole)
{
    Controller ctrl;
    EXPECT_TRUE(ctrl.passwordsMatch("abacaba", "abacaba"));
    EXPECT_FALSE(ctrl.passwordsMatch("abacaba", "bacabaca"));
}

TEST(TestAddLogin, TestWithDeleteLogin)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::string dummyLogin = "afsadgfsh";
    std::string dummyPassword = "askdjhgkasdg";
    EXPECT_FALSE(ctrl.existingLogin(dummyLogin));
    ctrl.newLogin(dummyLogin, dummyPassword);
    EXPECT_TRUE(ctrl.existingLogin(dummyLogin));
    EXPECT_TRUE(ctrl.deleteLogin(dummyLogin, dummyPassword));
    EXPECT_FALSE(ctrl.existingLogin(dummyLogin));
    EXPECT_FALSE(ctrl.deleteLogin(dummyLogin, dummyPassword));
}

TEST(TestValidUpdateRequestNumber, TestAll)
{
    Controller ctrl;
    EXPECT_TRUE(ctrl.validUpdateRequestNumber(0));
    EXPECT_TRUE(ctrl.validUpdateRequestNumber(1));
    EXPECT_FALSE(ctrl.validUpdateRequestNumber(2));
    EXPECT_FALSE(ctrl.validUpdateRequestNumber(-1));
}

TEST(TestValidTable, TestAll)
{
    Controller ctrl;
    std::vector<std::string> testVector = {"a", "ba", "ce", "asdfansdf"};
    EXPECT_TRUE(ctrl.validTable("a", testVector));
    EXPECT_TRUE(ctrl.validTable("ba", testVector));
    EXPECT_FALSE(ctrl.validTable("ca", testVector));
    EXPECT_FALSE(ctrl.validTable("lksdmfnkajg", testVector));
}

TEST(TestGetTables, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::string> expectedAnswer = {
        "artist", 
        "discs", 
        "login",
        "operation", 
        "operation_details", 
        "operation_type", 
        "role"};
    EXPECT_EQ(expectedAnswer, ctrl.getTables());
}

TEST(TestColumnsMethods, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::string> expectedAnswer = {
        "artist_id",
        "name"
    };
    std::vector<std::string> columns = ctrl.getColumns("artist");
    ASSERT_EQ(expectedAnswer, columns);

    EXPECT_TRUE(ctrl.validColumnsCount(1, columns));
    EXPECT_TRUE(ctrl.validColumnsCount(2, columns));
    EXPECT_FALSE(ctrl.validColumnsCount(3, columns));
    EXPECT_FALSE(ctrl.validColumnsCount(0, columns));

    EXPECT_TRUE(ctrl.validColumn("name", columns));
    EXPECT_TRUE(ctrl.validColumn("artist_id", columns));
    EXPECT_FALSE(ctrl.validColumn("asdgasdg", columns));
}

TEST(TestIntInputGuard, TestAll)
{
    Controller ctrl;
    std::istringstream in("1 asdgsadgasd");
    int value = 0;
    in >> value;
    EXPECT_TRUE(ctrl.intInputGuard(in));
    in >> value;
    EXPECT_FALSE(ctrl.intInputGuard(in));
}

TEST(TestUpdateQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_TRUE(ctrl.createUpdateQuery("artist", {"name"}, {"'petya'"}, "artist_id = 1"));
    EXPECT_TRUE(ctrl.createUpdateQuery("artist", {"name", "artist_id"}, {"'vasya'", "25"}, "artist_id = 1"));
    EXPECT_FALSE(ctrl.createUpdateQuery("artist", {"name"}, {"petya"}, "artist_id = 1"));
}

TEST(TestDeleteQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_TRUE(ctrl.createDeleteQuery("artist", "artist_id = 1"));
    EXPECT_FALSE(ctrl.createDeleteQuery("artist", "artist_id = afsdag"));
}

TEST(TestInsertQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_TRUE(ctrl.createInsertQuery("artist", {"artist_id", "name"}, {{"17", "'vasya'"}, {"23", "'petya'"}}));
    EXPECT_TRUE(ctrl.createDeleteQuery("artist", "artist_id = 17 OR artist_id = 23"));
    EXPECT_FALSE(ctrl.createInsertQuery("artist", {"artist_id", "name"}, {{"17", "vasya"}, {"23", "petya"}}));
}

TEST(TestMostPopularCDQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::string expectedAnswer = "The most popular cd:Sgt. Pepper's Lonely Hearts Club Band.\nCurrent amount of sold disks:5";
    EXPECT_EQ(expectedAnswer,ctrl.getTheMostPopularCD());
}

TEST(TestMostPopularArtistQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::string expectedAnswer = "The most popular Artist:The Beatles.\nCurrent amount of sold disks:5";
    EXPECT_EQ(expectedAnswer,ctrl.getTheMostPopularArtist());
}

TEST(TestCurrentQuantityOfCDQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::vector<std::string>> expectedAnswer = 
        {{"Goodbye Yellow Brick Road","1","10"},
        {"Sticky Fingers","4","10"},
        {"Led Zeppelin IV","3","10"},
        {"Sgt. Pepper's Lonely Hearts Club Band","5","10"}};
    EXPECT_EQ(expectedAnswer,ctrl.getCurrentQuantityOfCD());
}