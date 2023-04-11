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
        "name"};
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
    ctrl.createInsertQuery("artist", {"artist_id", "name"}, {{"11", "'anton'"}});
    EXPECT_TRUE(ctrl.createUpdateQuery("artist", {"name"}, {"'petya'"}, "artist_id = 11"));
    EXPECT_TRUE(ctrl.createUpdateQuery("artist", {"name", "artist_id"}, {"'vasya'", "25"}, "artist_id = 11"));
    EXPECT_FALSE(ctrl.createUpdateQuery("artist", {"name"}, {"petya"}, "artist_id = 11"));
}

TEST(TestInsertQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_TRUE(ctrl.createInsertQuery("artist", {"artist_id", "name"}, {{"17", "'vasya'"}, {"23", "'petya'"}}));
    EXPECT_TRUE(ctrl.createDeleteQuery("artist", "artist_id = 17 OR artist_id = 23"));
    EXPECT_FALSE(ctrl.createInsertQuery("artist", {"artist_id", "name"}, {{"17", "vasya"}, {"23", "petya"}}));
}

TEST(TestDeleteQuery, TestAll)
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    EXPECT_TRUE(ctrl.createDeleteQuery("artist", "artist_id = 25"));
    EXPECT_FALSE(ctrl.createDeleteQuery("artist", "artist_id = afsdag"));
}

TEST(TestMostPopularCDQuery, TestAll) // 5
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::string expectedAnswer = "The most popular cd: Sgt. Pepper's Lonely Hearts Club Band.\nCurrent amount of sold disks: 5";
    EXPECT_EQ(expectedAnswer, ctrl.getTheMostPopularCD());
}

TEST(TestMostPopularArtistQuery, TestAll) // 6
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::string expectedAnswer = "The most popular Artist: The Beatles.\nCurrent amount of sold disks: 5";
    EXPECT_EQ(expectedAnswer, ctrl.getTheMostPopularArtist());
}

TEST(TestCurrentQuantityOfCDQuery, TestAll) // 3
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::vector<std::string>> expectedAnswer =
        {{"Goodbye Yellow Brick Road", "1", "10"},
         {"Sticky Fingers", "4", "10"},
         {"Led Zeppelin IV", "3", "10"},
         {"Sgt. Pepper's Lonely Hearts Club Band", "5", "10"}};
    EXPECT_EQ(expectedAnswer, ctrl.getCurrentQuantityOfCD());
}

TEST(TestQuantityOfCDPeriodQuery, TestAll) // 4
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::vector<std::string>> expectedAnswer =
        {{"Sgt. Pepper's Lonely Hearts Club Band", "5", "89.95"},
         {"Sticky Fingers", "4", "83.96"},
         {"Led Zeppelin IV", "3", "65.97"},
         {"Goodbye Yellow Brick Road", "1", "23.99"}};
    EXPECT_EQ(expectedAnswer, ctrl.getQuantityOfCDPeriod("2022-00-00", "2022-12-12"));
}

TEST(TestgetInfoArtistQuery, TestAll) // 7
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::string> expectedAnswer =
        {{"The Beatles", "5", "89.95"}};
    EXPECT_EQ(expectedAnswer, ctrl.getInfoArtist("The Beatles"));
}

TEST(TestQuantityDeliveredSoldCDPeriodQuery, TestAll) // 11
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::vector<std::string>> expectedAnswer =
        {{"Highway 61 Revisited", "0", "10"},
         {"Sgt. Pepper's Lonely Hearts Club Band", "5", "0"},
         {"Are You Experienced", "0", "7"},
         {"Led Zeppelin IV", "3", "0"},
         {"Dark Side of the Moon", "0", "8"},
         {"A Night at the Opera", "0", "2"},
         {"Sticky Fingers", "4", "0"},
         {"The Rise and Fall of Ziggy Stardust and the Spiders from Mars", "0", "6"},
         {"Goodbye Yellow Brick Road", "1", "0"},
         {"Who's Next", "0", "9"}};
    EXPECT_EQ(expectedAnswer, ctrl.getQuantityDeliveredSoldCDPeriod("2022-00-00", "2022-12-12"));
}

TEST(TestInfoCDPeriodQuery, TestAll) // 12
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::string> expectedAnswer = {"Led Zeppelin IV", "3", "65.97"};
    EXPECT_EQ(expectedAnswer, ctrl.getInfoCDPeriod("2022-00-00", "2022-12-12", "4"));
}

TEST(TestgetAllCdQuery, TestAll) // 11
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::vector<std::string>> expectedAnswer =
        {{"1", "Highway 61 Revisited"},
         {"2", "Sgt. Pepper's Lonely Hearts Club Band"},
         {"3", "Are You Experienced"},
         {"4", "Led Zeppelin IV"},
         {"5", "Dark Side of the Moon"},
         {"6", "A Night at the Opera"},
         {"7", "Sticky Fingers"},
         {"8", "The Rise and Fall of Ziggy Stardust and the Spiders from Mars"},
         {"9", "Goodbye Yellow Brick Road"},
         {"10", "Who's Next"}};
    EXPECT_EQ(expectedAnswer, ctrl.getAllCd());
}

TEST(TestgetAllArtistQuery, TestAll) // 11
{
    Controller ctrl;
    ctrl.connectDB("RecordStore.db");
    std::vector<std::vector<std::string>> expectedAnswer =
        {
            {"1", "Bob Dylan"},
            {"2", "The Beatles"},
            {"3", "Jimi Hendrix"},
            {"4", "Led Zeppelin"},
            {"5", "Pink Floyd"},
            {"6", "Queen"},
            {"7", "The Rolling Stones"},
            {"8", "David Bowie"},
            {"9", "Elton John"},
            {"10", "The Who"}};
    EXPECT_EQ(expectedAnswer, ctrl.getAllArtist());
}