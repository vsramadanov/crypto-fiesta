#include<gtest/gtest.h>
#include<prefix_tree.h>

class PrefixTreeFixture : public testing::Test 
{
protected: 
  void SetUp() override 
  {
  }

  void TearDown() override
  {
  }

  utils::prefix_tree tree;
};

TEST_F(PrefixTreeFixture, Insert)
{
    tree.insert("hello");
    tree.insert("hello");

    EXPECT_EQ(tree.size(), 1);
}

TEST_F(PrefixTreeFixture, InsertCaseSense)
{
    tree.insert("hello");
    tree.insert("hElLo");

    EXPECT_EQ(tree.size(), 1);
}

TEST_F(PrefixTreeFixture, Contains)
{
    tree.insert("hello");

    EXPECT_TRUE(tree.contains("hello"));
    EXPECT_EQ(tree.size(), 1);
}

TEST_F(PrefixTreeFixture, ContainsPartial)
{
    tree.insert("hello");

    EXPECT_FALSE(tree.contains("hell"));
    EXPECT_EQ(tree.size(), 1);
}

TEST_F(PrefixTreeFixture, Match)
{
    tree.insert("hello");
    EXPECT_EQ(tree.match("hell").size(), 0);
}

TEST_F(PrefixTreeFixture, FullMatchOneWord)
{
    tree.insert("hello");
    auto vec = tree.match("hello"); 
    EXPECT_EQ(vec.size(), 1);

    EXPECT_STREQ(vec[0].c_str(), "hello");
}

TEST_F(PrefixTreeFixture, FullMatchWords)
{
    tree.insert("hello");
    tree.insert("helfo");
    tree.insert("heflo");

    auto vec = tree.match("hel?o"); 
    EXPECT_EQ(vec.size(), 2);

    EXPECT_STREQ(vec[0].c_str(), "helfo");
    EXPECT_STREQ(vec[1].c_str(), "hello");
}

TEST_F(PrefixTreeFixture, MatchLast)
{
    tree.insert("hello");
    
    auto vec = tree.match("hell?"); 
    EXPECT_EQ(vec.size(), 1);

    EXPECT_STREQ(vec[0].c_str(), "hello");
}

TEST_F(PrefixTreeFixture, MultiMatch)
{
    tree.insert("hello");
    tree.insert("hell");
    tree.insert("helloworld");
    tree.insert("range");
    tree.insert("value");

    
    auto vec = tree.match("?????"); 
    EXPECT_EQ(vec.size(), 3);

    EXPECT_STREQ(vec[0].c_str(), "hello");
    EXPECT_STREQ(vec[1].c_str(), "range");
    EXPECT_STREQ(vec[2].c_str(), "value");
}