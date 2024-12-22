#include "gtest/gtest.h"
#include "../libs/hash_table.h"
#include "../libs/listD.h"
#include "../libs/listS.h"
#include "../libs/massive.h"
#include "../libs/queue.h"
#include "../libs/stack.h"

// тест массива --------------------------------------------------------------------------------------------------------

TEST(StrArrayTest, Initialization) {
    size_t initialCapacity = 5;
    StrArray array(initialCapacity);
    EXPECT_EQ(array.sizeM(), 0);
    EXPECT_EQ(array.getCapacity(), initialCapacity);
}

TEST(StrArrayTest, Push) {
    StrArray array;
    array.push("hello");
    EXPECT_EQ(array.sizeM(), 1);
    array.push("world");
    EXPECT_EQ(array.sizeM(), 2);
    string result;
    EXPECT_TRUE(array.get(0, result));
    EXPECT_EQ(result, "hello");
    EXPECT_TRUE(array.get(1, result));
    EXPECT_EQ(result, "world");
}

TEST(StrArrayTest, Resize) {
    StrArray array(2);
    array.push("one");
    array.push("two");
    array.push("three");
    EXPECT_EQ(array.sizeM(), 3);
    EXPECT_EQ(array.getCapacity(), 4);
}

TEST(StrArrayTest, Pushi) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.pushi(1, "three");
    EXPECT_EQ(array.sizeM(), 3);
    string result;
    EXPECT_TRUE(array.get(0, result));
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result));
    EXPECT_EQ(result, "three");
    EXPECT_TRUE(array.get(2, result));
    EXPECT_EQ(result, "two");
}

TEST(StrArrayTest, PushiOutOfBounds) {
    StrArray array;
    array.push("one");
    array.push("two");
    testing::internal::CaptureStdout();
    array.pushi(3, "three"); // Индекс выходит за пределы допустимого диапазона
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "6:ERROR: Index out of range.\n");
    EXPECT_EQ(array.sizeM(), 2); // Размер не должен измениться
    string result;
    EXPECT_TRUE(array.get(0, result));
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result));
    EXPECT_EQ(result, "two");
}

TEST(StrArrayTest, Get) {
    StrArray array;
    array.push("one");
    array.push("two");
    string result;
    EXPECT_TRUE(array.get(0, result));
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result));
    EXPECT_EQ(result, "two");
    EXPECT_FALSE(array.get(2, result));
}

TEST(StrArrayTest, Replace) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.replace(1, "three");
    string result;
    EXPECT_TRUE(array.get(1, result));
    EXPECT_EQ(result, "three");
}

TEST(StrArrayTest, Delete) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.push("three");
    array.del(1);
    EXPECT_EQ(array.sizeM(), 2);
    string result;
    EXPECT_TRUE(array.get(0, result));
    EXPECT_EQ(result, "one");
    EXPECT_TRUE(array.get(1, result));
    EXPECT_EQ(result, "three");
}

TEST(StrArrayTest, SaveToFile) {
    StrArray array;
    array.push("one");
    array.push("two");
    array.push("three");
    array.saveToFile("test.txt");
    ifstream file("test.txt");
    string content;
    file >> content;
    EXPECT_EQ(content, "one;two;three");
    file.close();
    remove("test.txt");
}

// тест хеш таблицы --------------------------------------------------------------------------------------------------

TEST(HashTableTest, CreateHashTable) {
    HashTable table(10);
    EXPECT_EQ(table.getCapacity(), 10);
}

TEST(HashTableTest, PushExistingKey) {
    HashTable table(10);
    table.push("key1", "value1");
    testing::internal::CaptureStdout();
    table.push("key1", "value2");
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "6:ERROR: Key already exists.\n");
}

TEST(HashTableTest, PushSuccess) {
    HashTable table(10);
    table.push("key1", "value1");
    string result;
    EXPECT_TRUE(table.get("key1", result));
    EXPECT_EQ(result, "value1");
}

TEST(HashTableTest, GetKeyNotFound) {
    HashTable table(10);
    string result;
    EXPECT_FALSE(table.get("key1", result));
}

TEST(HashTableTest, GetKeyFound) {
    HashTable table(10);
    table.push("key1", "value1");
    string result;
    EXPECT_TRUE(table.get("key1", result));
    EXPECT_EQ(result, "value1");
}

TEST(HashTableTest, DeleteKeySuccess) {
    HashTable table(10);
    table.push("key1", "value1");
    EXPECT_TRUE(table.del("key1"));
    string result;
    EXPECT_FALSE(table.get("key1", result));
}

TEST(HashTableTest, DeleteKeyNotFound) {
    HashTable table(10);
    EXPECT_FALSE(table.del("key1"));
}

TEST(HashTableTest, SaveAndLoadFromFile) {
    HashTable table(10);
    table.push("key1", "value1");
    table.push("key2", "value2");
    table.saveToFile("test.txt");

    HashTable loadedTable(10);
    loadedTable.loadFromFile("test.txt");

    string result;
    EXPECT_TRUE(loadedTable.get("key1", result));
    EXPECT_EQ(result, "value1");
    EXPECT_TRUE(loadedTable.get("key2", result));
    EXPECT_EQ(result, "value2");
    remove("test.txt");
}

TEST(HashTableTest, LargeNumberOfOperations) {
    HashTable table(100);
    for (int i = 0; i < 1000; ++i) {
        table.push("key" + to_string(i), "value" + to_string(i));
    }
    for (int i = 0; i < 1000; ++i) {
        string result;
        EXPECT_TRUE(table.get("key" + to_string(i), result));
        EXPECT_EQ(result, "value" + to_string(i));
    }
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(table.del("key" + to_string(i)));
    }
    for (int i = 0; i < 1000; ++i) {
        string result;
        EXPECT_FALSE(table.get("key" + to_string(i), result));
    }
}

// тест двусвязного списка -------------------------------------------------------------------------------------------

TEST(ListDTest, CreateList) {
    ListD list;
    EXPECT_EQ(list.getHead(), nullptr);
    EXPECT_EQ(list.getTail(), nullptr);
}

// Тест на добавление элементов в начало списка
TEST(ListDTest, PushHead) {
    ListD list;
    list.pushh("value1");
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");

    list.pushh("value2");
    EXPECT_EQ(list.getHead()->data, "value2");
    EXPECT_EQ(list.getHead()->next->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на добавление элементов в конец списка
TEST(ListDTest, PushTail) {
    ListD list;
    list.pusht("value1");
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");

    list.pusht("value2");
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getHead()->next->data, "value2");
    EXPECT_EQ(list.getTail()->data, "value2");
}

// Тест на удаление элемента из головы списка
TEST(ListDTest, DeleteHead) {
    ListD list;
    list.pushh("value1");
    list.delh();
    EXPECT_EQ(list.getHead(), nullptr);
    EXPECT_EQ(list.getTail(), nullptr);

    list.pushh("value1");
    list.pushh("value2");
    list.delh();
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на удаление элемента из хвоста списка
TEST(ListDTest, DeleteTail) {
    ListD list;
    list.pusht("value1");
    list.delt();
    EXPECT_EQ(list.getHead(), nullptr);
    EXPECT_EQ(list.getTail(), nullptr);

    list.pusht("value1");
    list.pusht("value2");
    list.delt();
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на удаление элемента по значению
TEST(ListDTest, DeleteValue) {
    ListD list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");
    list.delv("value2");
    EXPECT_EQ(list.getHead()->data, "value3");
    EXPECT_EQ(list.getHead()->next->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");

    testing::internal::CaptureStdout();
    list.delv("value4"); // Элемент не найден
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Элемент не найден.\n");
    // Проверка на то, что список не изменился
    EXPECT_EQ(list.getHead()->data, "value3");
    EXPECT_EQ(list.getHead()->next->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на вывод списка
TEST(ListDTest, PrintList) {
    ListD list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");

    testing::internal::CaptureStdout();
    list.printList();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "value3 value2 value1 \n");
}

// Тест на сохранение списка в файл
TEST(ListDTest, SaveToFile) {
    ListD list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");

    string filename = "test_list.txt";
    list.saveToFile(filename);

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value3;value2;value1");

    // Удаление файла после теста
    remove(filename.c_str());
}

// тест односвязного списка ------------------------------------------------------------------------------------------

TEST(ListSTest, CreateList) {
    ListS list;
    EXPECT_EQ(list.getHead(), nullptr);
    EXPECT_EQ(list.getTail(), nullptr);
}

// Тест на добавление элементов в начало списка
TEST(ListSTest, PushHead) {
    ListS list;
    list.pushh("value1");
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");

    list.pushh("value2");
    EXPECT_EQ(list.getHead()->data, "value2");
    EXPECT_EQ(list.getHead()->next->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на добавление элементов в конец списка
TEST(ListSTest, PushTail) {
    ListS list;
    list.pusht("value1");
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");

    list.pusht("value2");
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getHead()->next->data, "value2");
    EXPECT_EQ(list.getTail()->data, "value2");
}

// Тест на удаление элемента из головы списка
TEST(ListSTest, DeleteHead) {
    ListS list;
    list.pushh("value1");
    list.delh();
    EXPECT_EQ(list.getHead(), nullptr);
    EXPECT_EQ(list.getTail(), nullptr);

    list.pushh("value1");
    list.pushh("value2");
    list.delh();
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на удаление элемента из хвоста списка
TEST(ListSTest, DeleteTail) {
    ListS list;
    list.pusht("value1");
    list.delt();
    EXPECT_EQ(list.getHead(), nullptr);
    EXPECT_EQ(list.getTail(), nullptr);

    list.pusht("value1");
    list.pusht("value2");
    list.delt();
    EXPECT_EQ(list.getHead()->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на удаление элемента по значению
TEST(ListSTest, DeleteValue) {
    ListS list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");
    list.delv("value2");
    EXPECT_EQ(list.getHead()->data, "value3");
    EXPECT_EQ(list.getHead()->next->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");

    testing::internal::CaptureStdout();
    list.delv("value4"); // Элемент не найден
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Элемент не найден.\n");
    // Проверка на то, что список не изменился
    EXPECT_EQ(list.getHead()->data, "value3");
    EXPECT_EQ(list.getHead()->next->data, "value1");
    EXPECT_EQ(list.getTail()->data, "value1");
}

// Тест на сохранение списка в файл
TEST(ListSTest, SaveToFile) {
    ListS list;
    list.pushh("value1");
    list.pushh("value2");
    list.pushh("value3");

    string filename = "test_list.txt";
    list.saveToFile(filename);

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value3;value2;value1");

    // Удаление файла после теста
    remove(filename.c_str());
}

// тест очереди ------------------------------------------------------------------------------------------------------
TEST(QueueTest, CreateQueue) {
    Queue queue;
    EXPECT_EQ(queue.getHead(), nullptr);
    EXPECT_EQ(queue.getTail(), nullptr);
}

// Тест на добавление элементов в очередь
TEST(QueueTest, Push) {
    Queue queue;
    queue.push("value1");
    EXPECT_EQ(queue.getHead()->data, "value1");
    EXPECT_EQ(queue.getTail()->data, "value1");

    queue.push("value2");
    EXPECT_EQ(queue.getHead()->data, "value1");
    EXPECT_EQ(queue.getTail()->data, "value2");
    EXPECT_EQ(queue.getTail()->prev->data, "value1");
}

// Тест на удаление элементов из очереди
TEST(QueueTest, Delete) {
    Queue queue;
    queue.push("value1");
    queue.del();
    EXPECT_EQ(queue.getHead(), nullptr);
    EXPECT_EQ(queue.getTail(), nullptr);

    queue.push("value1");
    queue.push("value2");
    queue.del();
    EXPECT_EQ(queue.getHead()->data, "value2");
    EXPECT_EQ(queue.getTail()->data, "value2");
}

// Тест на сохранение очереди в файл
TEST(QueueTest, SaveToFile) {
    Queue queue;
    queue.push("value1");
    queue.push("value2");
    queue.push("value3");

    string filename = "test_queue.txt";
    queue.saveToFile(filename);

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value1;value2;value3");

    // Удаление файла после теста
    remove(filename.c_str());
}

// тест стека --------------------------------------------------------------------------------------------------------
TEST(StackTest, CreateStack) {
    Stack stack;
    EXPECT_EQ(stack.getTop(), nullptr);
}

// Тест на добавление элементов в стек
TEST(StackTest, Push) {
    Stack stack;
    stack.push("value1");
    EXPECT_EQ(stack.getTop()->data, "value1");

    stack.push("value2");
    EXPECT_EQ(stack.getTop()->data, "value2");
    EXPECT_EQ(stack.getTop()->next->data, "value1");
}

// Тест на удаление элементов из стека
TEST(StackTest, Delete) {
    Stack stack;
    stack.push("value1");
    stack.del();
    EXPECT_EQ(stack.getTop(), nullptr);

    stack.push("value1");
    stack.push("value2");
    stack.del();
    EXPECT_EQ(stack.getTop()->data, "value1");
}

// Тест на сохранение стека в файл
TEST(StackTest, SaveToFile) {
    Stack stack;
    stack.push("value1");
    stack.push("value2");
    stack.push("value3");

    string filename = "test_stack.txt";
    stack.saveToFile(filename);

    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();
    EXPECT_EQ(file_contents, "value3;value2;value1");

    // Удаление файла после теста
    remove(filename.c_str());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}