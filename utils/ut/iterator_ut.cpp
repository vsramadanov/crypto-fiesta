#include<gtest/gtest.h>
#include<prefix_tree.h>

class PrefixTreeIteratorFixture : public testing::Test 
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

TEST_F(PrefixTreeIteratorFixture, EmptyIt)
{
    auto it = tree.cbegin();
    EXPECT_STREQ("", it->c_str());
}

TEST_F(PrefixTreeIteratorFixture, NonEmptyIt)
{
    tree.insert("hell");
    tree.insert("hello");
    auto it = tree.cbegin();
    EXPECT_STREQ("hell", it->c_str());
}

TEST_F(PrefixTreeIteratorFixture, Increment)
{
    tree.insert("hell");
    tree.insert("hello");
    tree.insert("helloworld");
    tree.insert("hellway");
    auto it = tree.cbegin();
    EXPECT_STREQ("hello", (++it)->c_str());
    EXPECT_STREQ("helloworld", (++it)->c_str());
    EXPECT_STREQ("hellway", (++it)->c_str());
    EXPECT_STREQ("", (++it)->c_str());
}

TEST_F(PrefixTreeIteratorFixture, RangeBaseFor)
{
    std::vector<std::string> words{"hell", "hello", "helloworld", "hellway"};
    for (auto& word : words)
        tree.insert(word);

    int k = 0;
    for (const auto& key : tree) {
        std::cout << k << ": "<< key << std::endl;
        EXPECT_STREQ(words[k].c_str(), key.c_str());
        k++;
    }
}