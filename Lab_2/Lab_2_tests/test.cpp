#include "pch.h"
#include "../Lab_2/Array.h"
#include "../Lab_2/Slime.h"

TEST(ArrayTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, InsertElement) {
    Array<int> arr;
    arr.insert(42);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 42);
}

TEST(ArrayTest, InsertAtSpecificIndex) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(1, 15);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 15);
    EXPECT_EQ(arr[2], 20);
}

TEST(ArrayTest, RemoveElement) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.remove(0);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 20);
}

TEST(ArrayTest, IndexOperator) {
    Array<int> arr;
    arr.insert(10);
    arr[0] = 15;
    EXPECT_EQ(arr[0], 15);
}

TEST(ArrayTest, IteratorTraversal) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.insert(i);
    }

    auto it = arr.iterator();
    int sum = 0;
    while (it.hasNext()) {
        sum += it.get();
        it.next();
    }
    EXPECT_EQ(sum, 15);
}

TEST(ArrayTest, CopyConstructor) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    Array<int> copy = arr;

    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(copy[0], 10);
    EXPECT_EQ(copy[1], 20);
}

TEST(ArrayTest, MoveConstructor) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    Array<int> moved = std::move(arr);

    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(moved[0], 10);
    EXPECT_EQ(moved[1], 20);
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, ResizeFunctionality) {
    Array<int> arr;
    for (int i = 0; i < 100; ++i) {
        arr.insert(i);
    }
    EXPECT_EQ(arr.size(), 100);

    EXPECT_EQ(arr[8], 8);
    EXPECT_EQ(arr[45], 45);
    EXPECT_EQ(arr[99], 99);
}

TEST(ArrayTest, IteratorDereferenceAndIncrement) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.insert(i);
    }

    auto it = arr.iterator();

    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    it++;
    EXPECT_EQ(*it, 3);

    ++it;
    ++it;
    EXPECT_EQ(*it, 5);

    ++it;
    EXPECT_FALSE(it != arr.end());
}

TEST(ArrayTest, BeginEndRangeFor) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.insert(i);
    }

    int sum = 0;
    for (const auto& value : arr) {
        sum += value;
    }

    EXPECT_EQ(sum, 15);
}

TEST(ArrayTest, CBeginCEnd) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) {
        arr.insert(i);
    }

    int sum = 0;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 15);
}

TEST(ArrayTest, ConstIteratorTraversal) {
    const Array<int> arr = []() {
        Array<int> temp;
        temp.insert(10);
        temp.insert(20);
        temp.insert(30);
        return temp;
        }();

    auto it = arr.iterator();

    std::string result;
    while (it.hasNext()) {
        result += std::to_string(it.get()) + " ";
        it.next();
    }

    EXPECT_EQ(result, "10 20 30 ");
}

TEST(ArrayTest, ConstIteratorRangeFor) {
    const Array<int> arr = []() {
        Array<int> temp;
        temp.insert(10);
        temp.insert(20);
        temp.insert(30);
        return temp;
        }();

    std::string result;
    for (const auto& value : arr) {
        result += std::to_string(value) + " ";
    }

    EXPECT_EQ(result, "10 20 30 ");
}

TEST(ArrayTest, ConstReverseIterator) {
    const Array<int> arr = []() {
        Array<int> temp;
        temp.insert(10);
        temp.insert(20);
        temp.insert(30);
        return temp;
        }();

    auto it = arr.reverseIterator();

    std::string result;
    while (it.hasNext()) {
        result += std::to_string(it.get()) + " ";
        it.next();
    }

    EXPECT_EQ(result, "30 20 10 ");
}

TEST(ArrayTest, ReverseIteratorTraversal) {
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    auto it = arr.reverseIterator();

    std::string result;
    while (it.hasNext()) {
        result += std::to_string(it.get()) + " ";
        it.next();
    }

    EXPECT_EQ(result, "30 20 10 ");
}

TEST(SlimeArrayTest, DefaultConstructor) {
    Array<Slime> arr;
    EXPECT_EQ(arr.size(), 0);
}

TEST(SlimeArrayTest, InsertElement) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].getName(), "Dina");
    EXPECT_EQ(arr[0].getAbilityCount(), 25);
    EXPECT_EQ(arr[1].getName(), "Maria");
    EXPECT_EQ(arr[1].getAbilityCount(), 30);
}

TEST(SlimeArrayTest, InsertAtSpecificIndex) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Tanya", 35));
    arr.insert(1, Slime("Maria", 30));

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0].getName(), "Dina");
    EXPECT_EQ(arr[1].getName(), "Maria");
    EXPECT_EQ(arr[2].getName(), "Tanya");
}

TEST(SlimeArrayTest, RemoveElement) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    arr.remove(1);

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].getName(), "Dina");
    EXPECT_EQ(arr[1].getName(), "Tanya");
}

TEST(SlimeArrayTest, IndexOperator) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));

    EXPECT_EQ(arr[0].getName(), "Dina");
    EXPECT_EQ(arr[0].getAbilityCount(), 25);

    arr[0] = Slime("Maria", 30);
    EXPECT_EQ(arr[0].getName(), "Maria");
    EXPECT_EQ(arr[0].getAbilityCount(), 30);
}

TEST(SlimeArrayTest, IteratorDereferenceAndIncrement) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    auto it = arr.iterator();
    EXPECT_EQ(it.get().getName(), "Dina");
    ++it;
    EXPECT_EQ(it.get().getName(), "Maria");
    it++;
    EXPECT_EQ(it.get().getName(), "Tanya");
}

TEST(SlimeArrayTest, IteratorTraversal) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    auto it = arr.iterator();
    std::string result;
    while (it != arr.end()) {
        result += it.get().getName() + " ";
        ++it;
    }
    EXPECT_EQ(result, "Dina Maria Tanya ");
}

TEST(SlimeArrayTest, BeginEndRangeFor) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    std::string result;
    for (const auto& Slime : arr) {
        result += Slime.getName() + " ";
    }

    EXPECT_EQ(result, "Dina Maria Tanya ");
}

TEST(SlimeArrayTest, CBeginCEnd) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    std::string result;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
        result += it.get().getName() + " ";
    }

    EXPECT_EQ(result, "Dina Maria Tanya ");
}

TEST(SlimeArrayTest, CopyConstructor) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));

    Array<Slime> copy = arr;

    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(copy[0].getName(), "Dina");
    EXPECT_EQ(copy[1].getName(), "Maria");
}

TEST(SlimeArrayTest, MoveConstructor) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));

    Array<Slime> moved = std::move(arr);

    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(moved[0].getName(), "Dina");
    EXPECT_EQ(moved[1].getName(), "Maria");
    EXPECT_EQ(arr.size(), 0);
}

TEST(SlimeArrayTest, ResizeFunctionality) {
    Array<Slime> arr;

    for (int i = 0; i < 100; ++i) {
        arr.insert(Slime("Slime" + std::to_string(i), i));
    }

    EXPECT_EQ(arr.size(), 100);
    EXPECT_EQ(arr[8].getName(), "Slime8");
    EXPECT_EQ(arr[8].getAbilityCount(), 8);
    EXPECT_EQ(arr[45].getName(), "Slime45");
    EXPECT_EQ(arr[45].getAbilityCount(), 45);
    EXPECT_EQ(arr[99].getName(), "Slime99");
    EXPECT_EQ(arr[99].getAbilityCount(), 99);
}

TEST(SlimeArrayTest, InsertAndAccess) {
    Array<Slime> arr;

    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].getName(), "Dina");
    EXPECT_EQ(arr[0].getAbilityCount(), 25);
    EXPECT_EQ(arr[1].getName(), "Maria");
    EXPECT_EQ(arr[1].getAbilityCount(), 30);
}

TEST(SlimeArrayTest, RemoveSlime) {
    Array<Slime> arr;

    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    arr.remove(1);

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].getName(), "Dina");
    EXPECT_EQ(arr[1].getName(), "Tanya");
}

TEST(SlimeArrayTest, IteratorSlime) {
    Array<Slime> arr;

    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    auto it = arr.iterator();

    EXPECT_EQ(it.get().getName(), "Dina");
    ++it;
    EXPECT_EQ(it.get().getName(), "Maria");
    it++;
    EXPECT_EQ(it.get().getName(), "Tanya");
}

TEST(SlimeArrayTest, RangeForSlime) {
    Array<Slime> arr;

    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    std::string result;
    for (const auto& Slime : arr) {
        result += Slime.getName() + " ";
    }

    EXPECT_EQ(result, "Dina Maria Tanya ");
}

TEST(SlimeArrayTest, ConstIteratorTraversal) {
    const Array<Slime> arr = []() {
        Array<Slime> temp;
        temp.insert(Slime("Dina", 25));
        temp.insert(Slime("Maria", 30));
        temp.insert(Slime("Tanya", 35));
        return temp;
        }();

    auto it = arr.iterator();

    std::string result;
    while (it.hasNext()) {
        result += it.get().getName() + " ";
        it.next();
    }

    EXPECT_EQ(result, "Dina Maria Tanya ");
}

TEST(SlimeArrayTest, ConstReverseIterator) {
    const Array<Slime> arr = []() {
        Array<Slime> temp;
        temp.insert(Slime("Dina", 25));
        temp.insert(Slime("Maria", 30));
        temp.insert(Slime("Tanya", 35));
        return temp;
        }();

    auto it = arr.reverseIterator();

    std::string result;
    while (it.hasNext()) {
        result += it.get().getName() + " ";
        it.next();
    }

    EXPECT_EQ(result, "Tanya Maria Dina ");
}


TEST(SlimeArrayTest, ReverseIteratorTraversal) {
    Array<Slime> arr;
    arr.insert(Slime("Dina", 25));
    arr.insert(Slime("Maria", 30));
    arr.insert(Slime("Tanya", 35));

    auto it = arr.reverseIterator();

    std::string result;
    while (it.hasNext()) {
        result += it.get().getName() + " ";
        it.next();
    }

    EXPECT_EQ(result, "Tanya Maria Dina ");
}