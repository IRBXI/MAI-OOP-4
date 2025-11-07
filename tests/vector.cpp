#include <gtest/gtest.h>

#include "vector.hpp" // Include the header for your Vector class

namespace lib::vector {

namespace {

// Test fixture for Vector tests
template <typename T>
class VectorTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

using TestTypes = ::testing::Types<int, double, std::string>;
TYPED_TEST_SUITE(VectorTest, TestTypes);

// Default Constructor
TYPED_TEST(VectorTest, DefaultConstructor) {
    Vector<TypeParam> vec;
    EXPECT_EQ(vec.Size(), 0);
    EXPECT_EQ(vec.Capacity(), 0);
    EXPECT_TRUE(vec.IsEmpty());
}

// Constructor with count and value
TYPED_TEST(VectorTest, CountValueConstructor) {
    TypeParam value = TypeParam{};
    Vector<TypeParam> vec(5, value);
    EXPECT_EQ(vec.Size(), 5);
    EXPECT_GE(vec.Capacity(), 5);
    for (std::size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], value);
    }
}

// Initializer List Constructor
TYPED_TEST(VectorTest, InitializerListConstructor) {
    Vector<TypeParam> vec{TypeParam{}, TypeParam{}, TypeParam{}};
    EXPECT_EQ(vec.Size(), 3);
    EXPECT_GE(vec.Capacity(), 3);
}

// Copy Constructor
TYPED_TEST(VectorTest, CopyConstructor) {
    Vector<TypeParam> original{TypeParam{}, TypeParam{}};
    Vector<TypeParam> copy(original);
    EXPECT_EQ(original.Size(), copy.Size());
    for (std::size_t i = 0; i < original.Size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

// Move Constructor
TYPED_TEST(VectorTest, MoveConstructor) {
    Vector<TypeParam> original{TypeParam{}, TypeParam{}};
    Vector<TypeParam> moved(std::move(original));
    EXPECT_EQ(moved.Size(), 2);
    EXPECT_EQ(original.Size(), 0); // NOLINT(bugprone-use-after-move)
    EXPECT_EQ(original.Data(), nullptr);
}

// Copy Assignment
TYPED_TEST(VectorTest, CopyAssignment) {
    Vector<TypeParam> original{TypeParam{}, TypeParam{}};
    Vector<TypeParam> copy;
    copy = original;
    EXPECT_EQ(original.Size(), copy.Size());
    for (std::size_t i = 0; i < original.Size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

// Move Assignment
TYPED_TEST(VectorTest, MoveAssignment) {
    Vector<TypeParam> original{TypeParam{}, TypeParam{}};
    Vector<TypeParam> moved;
    moved = std::move(original);
    EXPECT_EQ(moved.Size(), 2);
    EXPECT_EQ(original.Size(), 0); // NOLINT(bugprone-use-after-move)
    EXPECT_EQ(original.Data(), nullptr);
}

// Front and Back - only test with non-empty vectors since they're noexcept
TYPED_TEST(VectorTest, FrontBack) {
    TypeParam first = TypeParam{};
    TypeParam last = TypeParam{} + TypeParam{};
    Vector<TypeParam> vec{first, TypeParam{}, last};
    EXPECT_EQ(vec.Front(), first);
    EXPECT_EQ(vec.Back(), last);

    // Test const versions
    const Vector<TypeParam>& const_vec = vec;
    EXPECT_EQ(const_vec.Front(), first);
    EXPECT_EQ(const_vec.Back(), last);
}

// Operator[] and At
TYPED_TEST(VectorTest, SubscriptAndAt) {
    Vector<TypeParam> vec{TypeParam{}, TypeParam{}};
    EXPECT_NO_THROW(vec[0]);
    EXPECT_NO_THROW(vec.At(0));
    EXPECT_THROW(vec.At(2), std::out_of_range);

    // Test const versions
    const Vector<TypeParam>& const_vec = vec;
    EXPECT_NO_THROW(const_vec[0]);
    EXPECT_NO_THROW(const_vec.At(0));
    EXPECT_THROW(const_vec.At(2), std::out_of_range);
}

// Size, Capacity, Data, IsEmpty
TYPED_TEST(VectorTest, CapacityAndData) {
    Vector<TypeParam> vec;
    EXPECT_TRUE(vec.IsEmpty());
    vec.PushBack(TypeParam{});
    EXPECT_FALSE(vec.IsEmpty());
    EXPECT_EQ(vec.Size(), 1);
    EXPECT_GE(vec.Capacity(), 1);
    EXPECT_NE(vec.Data(), nullptr);

    // Test const Data()
    const Vector<TypeParam>& const_vec = vec;
    EXPECT_NE(const_vec.Data(), nullptr);
}

// Resize
TYPED_TEST(VectorTest, Resize) {
    Vector<TypeParam> vec;
    vec.Resize(3, TypeParam{});
    EXPECT_EQ(vec.Size(), 3);
    for (std::size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], TypeParam{});
    }
}

// Reserve
TYPED_TEST(VectorTest, Reserve) {
    Vector<TypeParam> vec;
    vec.Reserve(10);
    EXPECT_GE(vec.Capacity(), 10);
    EXPECT_EQ(vec.Size(), 0);
}

// Clear
TYPED_TEST(VectorTest, Clear) {
    Vector<TypeParam> vec{TypeParam{}, TypeParam{}};
    vec.Clear();
    EXPECT_EQ(vec.Size(), 0);
    EXPECT_TRUE(vec.IsEmpty());
}

// Insert
TYPED_TEST(VectorTest, Insert) {
    Vector<TypeParam> vec{TypeParam{}, TypeParam{}};
    vec.Insert(1, TypeParam{} + TypeParam{});
    EXPECT_EQ(vec.Size(), 3);
    EXPECT_EQ(vec[1], TypeParam{} + TypeParam{});
}

// Erase
TYPED_TEST(VectorTest, Erase) {
    Vector<TypeParam> vec{TypeParam{1}, TypeParam{2}, TypeParam{3}};
    vec.Erase(0, 2);
    EXPECT_EQ(vec.Size(), 1);
    EXPECT_EQ(vec[0], TypeParam{3});
}

// PushBack and EmplaceBack
TYPED_TEST(VectorTest, PushBackEmplaceBack) {
    Vector<TypeParam> vec;
    vec.PushBack(TypeParam{});
    vec.EmplaceBack(TypeParam{});
    EXPECT_EQ(vec.Size(), 2);
}

// PopBack
TYPED_TEST(VectorTest, PopBack) {
    Vector<TypeParam> vec{TypeParam{}, TypeParam{}};
    vec.PopBack();
    EXPECT_EQ(vec.Size(), 1);
}

// Swap
TYPED_TEST(VectorTest, Swap) {
    Vector<TypeParam> a{TypeParam{1}};
    Vector<TypeParam> b{TypeParam{2}};
    swap(a, b);
    EXPECT_EQ(a[0], TypeParam{2});
    EXPECT_EQ(b[0], TypeParam{1});
}

// Test that Front() and Back() are indeed noexcept
TEST(VectorTest, NoExceptSpecifications) {
    EXPECT_TRUE(noexcept(Vector<int>().Front()));
    EXPECT_TRUE(noexcept(Vector<int>().Back()));
    EXPECT_TRUE(noexcept(Vector<int>().Size()));
    EXPECT_TRUE(noexcept(Vector<int>().Capacity()));
    EXPECT_TRUE(noexcept(Vector<int>().Data()));
    EXPECT_TRUE(noexcept(Vector<int>().IsEmpty()));
    EXPECT_TRUE(noexcept(Vector<int>().Clear()));
}

// Edge Cases and Error Conditions
TEST(VectorTest, OutOfRangeAccess) {
    Vector<int> vec{1, 2};
    EXPECT_THROW(vec.At(5), std::out_of_range);
}

// Type Requirements (Copy Constructible)
TEST(VectorTest, NoCopyForNonCopyable) {
    struct NonCopyable {
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = default;
    };
    // Should compile only for move operations
    Vector<NonCopyable> vec;
    vec.PushBack(NonCopyable{});
    vec.EmplaceBack();
}

} // namespace

} // namespace lib::vector
