#include <gtest/gtest.h>
#include <StackNotCopyable.h>

#include "Stack.h"


class StackFixture: public ::testing::Test
{
protected:
    struct Copyable
    {
        Copyable() = default;
        Copyable(const Copyable &) = default;
        Copyable(Copyable &&) = default;

        int value = 0;
    };

    struct NotCopyable
    {
        NotCopyable() = default;
        NotCopyable(const NotCopyable &) = delete;
        NotCopyable(NotCopyable &&) = default;

        int value = 0;
    };

    Stack<Copyable> copyableStack{};
    Stack<Copyable> copyableStackEmpty{};
    StackNotCopyable<NotCopyable> notCopyableStack{};
    StackNotCopyable<NotCopyable> notCopyableStackEmpty{};
    Stack<std::string> stringStackEmpty{};

    void SetUp() override
    {
        copyableStack.push({1});
        copyableStack.push({2});
        copyableStack.push({3});
        copyableStack.push({4});
        copyableStack.push({5});

        notCopyableStack.push({1});
        notCopyableStack.push({2});
        notCopyableStack.push({3});
        notCopyableStack.push({4});
        notCopyableStack.push({5});
    }

    void TearDown() override
    {

    }
};

TEST_F(StackFixture, TestForTest)
{
    EXPECT_EQ(std::is_move_constructible_v<Copyable>, true);
    EXPECT_EQ(std::is_copy_constructible_v<Copyable>, true);

    EXPECT_EQ(std::is_move_constructible_v<NotCopyable>, true);
    EXPECT_EQ(std::is_copy_constructible_v<NotCopyable>, false);
}

TEST_F(StackFixture, Constructors)
{
    EXPECT_EQ(std::is_move_constructible_v<Stack<int>>, true);
    EXPECT_EQ(std::is_copy_constructible_v<Stack<int>>, false);
    EXPECT_EQ(std::is_move_assignable_v<Stack<int>>, true);
    EXPECT_EQ(std::is_copy_assignable_v<Stack<int>>, false);
}

TEST_F(StackFixture, PushMove)
{
    std::string string{"I am a string"};
    stringStackEmpty.push(std::move(string));

    EXPECT_EQ(stringStackEmpty.head(), std::string{"I am a string"});
}

TEST_F(StackFixture, PushCopy)
{
    Copyable copyable{60};
    copyableStackEmpty.push(copyable);

    EXPECT_EQ(copyableStackEmpty.head().value, 60);
}

TEST_F(StackFixture, PushEmplace)
{
    notCopyableStackEmpty.push_emplace(600);

    EXPECT_EQ(notCopyableStackEmpty.head().value, 600);
}

TEST_F(StackFixture, PopSfinaeNotCopy)
{
    using PopType = decltype(notCopyableStack.pop());
    constexpr bool isSame = std::is_same_v<PopType, NotCopyable>;

    EXPECT_EQ(isSame, true);
}

TEST_F(StackFixture, PopSfinaeCopy)
{
    using PopType = decltype(copyableStack.pop());
    constexpr bool isSame = std::is_same_v<PopType, void>;

    EXPECT_EQ(isSame, true);
}

TEST_F(StackFixture, PopCopy)
{
    EXPECT_EQ(copyableStack.head().value, 5);

    copyableStack.pop();

    EXPECT_EQ(copyableStack.head().value, 4);
}

TEST_F(StackFixture, PopNotCopy)
{
    EXPECT_EQ(notCopyableStack.head().value, 5);

    NotCopyable value = notCopyableStack.pop();

    EXPECT_EQ(notCopyableStack.head().value, 4);
    EXPECT_EQ(value.value, 5);
}

TEST_F(StackFixture, Head)
{
    EXPECT_EQ(copyableStack.head().value, 5);
}

TEST_F(StackFixture, PopEmpty)
{
    EXPECT_THROW(
        copyableStackEmpty.pop(),
        StackException
    );
}

TEST_F(StackFixture, PopEmptyNotCopyable)
{
    EXPECT_THROW(
        notCopyableStackEmpty.pop(),
        StackException
    );
}

TEST_F(StackFixture, HeadEmpty)
{
    EXPECT_THROW(
        copyableStackEmpty.head(),
        StackException
    );
}