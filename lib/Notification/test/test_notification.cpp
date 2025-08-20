#include "Notification.h"
#include "unity.h"
#include <string>

/**
 * @file test_notification.cpp
 * @brief Unit tests for the Notification system
 */

static Notification* testNotification = nullptr;

void setUp(void) {
    testNotification = new Notification();
    TEST_ASSERT_NOT_NULL(testNotification);
}

void tearDown(void) {
    if (testNotification) {
        delete testNotification;
        testNotification = nullptr;
    }
}

void test_basic_send_consume_int() {
    int testValue = 42;
    bool sendResult = testNotification->send("test_int", testValue);
    TEST_ASSERT_TRUE(sendResult);
    
    int* received = testNotification->consume<int>("test_int");
    TEST_ASSERT_NOT_NULL(received);
    TEST_ASSERT_EQUAL_INT(testValue, *received);
    free(received);
}

void test_basic_send_consume_float() {
    float testValue = 3.14159f;
    bool sendResult = testNotification->send("test_float", testValue);
    TEST_ASSERT_TRUE(sendResult);
    
    float* received = testNotification->consume<float>("test_float");
    TEST_ASSERT_NOT_NULL(received);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, testValue, *received);
    free(received);
}

void test_string_notification() {
    std::string testString = "Hello World";
    bool sendResult = testNotification->send("test_string", 
                                           (void*)testString.c_str(), 
                                           testString.length() + 1);
    TEST_ASSERT_TRUE(sendResult);
    
    char* received = (char*)testNotification->consume("test_string");
    TEST_ASSERT_NOT_NULL(received);
    TEST_ASSERT_EQUAL_STRING(testString.c_str(), received);
    free(received);
}

void test_struct_notification() {
    struct TestStruct {
        int id;
        float value;
        char name[16];
    };
    
    TestStruct testData = {123, 45.67f, "test"};
    bool sendResult = testNotification->send("test_struct", testData);
    TEST_ASSERT_TRUE(sendResult);
    
    TestStruct* received = testNotification->consume<TestStruct>("test_struct");
    TEST_ASSERT_NOT_NULL(received);
    TEST_ASSERT_EQUAL_INT(testData.id, received->id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, testData.value, received->value);
    TEST_ASSERT_EQUAL_STRING(testData.name, received->name);
    free(received);
}

void test_has_notification() {
    int testValue = 99;
    testNotification->send("test_has", testValue);
    
    TEST_ASSERT_TRUE(testNotification->hasNotification("test_has"));
    TEST_ASSERT_FALSE(testNotification->hasNotification("nonexistent"));
    
    // Consume and check again
    int* received = testNotification->consume<int>("test_has");
    TEST_ASSERT_NOT_NULL(received);
    free(received);
    
    TEST_ASSERT_FALSE(testNotification->hasNotification("test_has"));
}

void test_notification_timeout() {
    // Try to consume non-existent notification with short timeout
    TickType_t startTime = xTaskGetTickCount();
    void* received = testNotification->consume("nonexistent", pdMS_TO_TICKS(100));
    TickType_t endTime = xTaskGetTickCount();
    
    TEST_ASSERT_NULL(received);
    
    // Check that timeout was approximately respected (allow some margin)
    TickType_t elapsed = endTime - startTime;
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(pdMS_TO_TICKS(90), elapsed);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(pdMS_TO_TICKS(150), elapsed);
}

void test_notification_count() {
    TEST_ASSERT_EQUAL_UINT32(0, testNotification->getNotificationCount());
    
    int value1 = 1, value2 = 2, value3 = 3;
    testNotification->send("key1", value1);
    testNotification->send("key2", value2);
    testNotification->send("key3", value3);
    
    TEST_ASSERT_EQUAL_UINT32(3, testNotification->getNotificationCount());
    
    // Consume one
    int* received = testNotification->consume<int>("key1");
    free(received);
    
    TEST_ASSERT_EQUAL_UINT32(2, testNotification->getNotificationCount());
    
    // Clear all
    testNotification->clear();
    TEST_ASSERT_EQUAL_UINT32(0, testNotification->getNotificationCount());
}

void test_remove_notification() {
    int testValue = 55;
    testNotification->send("test_remove", testValue);
    
    TEST_ASSERT_TRUE(testNotification->hasNotification("test_remove"));
    TEST_ASSERT_TRUE(testNotification->remove("test_remove"));
    TEST_ASSERT_FALSE(testNotification->hasNotification("test_remove"));
    TEST_ASSERT_FALSE(testNotification->remove("test_remove")); // Already removed
}

void test_notification_info() {
    float testValue = 2.718f;
    TickType_t beforeSend = xTaskGetTickCount();
    testNotification->send("test_info", testValue);
    TickType_t afterSend = xTaskGetTickCount();
    
    size_t size;
    TickType_t timestamp;
    bool hasInfo = testNotification->getNotificationInfo("test_info", &size, &timestamp);
    
    TEST_ASSERT_TRUE(hasInfo);
    TEST_ASSERT_EQUAL_UINT32(sizeof(float), size);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT32(beforeSend, timestamp);
    TEST_ASSERT_LESS_OR_EQUAL_UINT32(afterSend, timestamp);
    
    // Test with non-existent key
    TEST_ASSERT_FALSE(testNotification->getNotificationInfo("nonexistent", &size, &timestamp));
}

void test_overwrite_notification() {
    int value1 = 111;
    int value2 = 222;
    
    testNotification->send("overwrite_test", value1);
    TEST_ASSERT_EQUAL_UINT32(1, testNotification->getNotificationCount());
    
    // Send again with same key - should overwrite
    testNotification->send("overwrite_test", value2);
    TEST_ASSERT_EQUAL_UINT32(1, testNotification->getNotificationCount());
    
    // Consume should get the latest value
    int* received = testNotification->consume<int>("overwrite_test");
    TEST_ASSERT_NOT_NULL(received);
    TEST_ASSERT_EQUAL_INT(value2, *received);
    free(received);
}

void test_memory_ownership() {
    // Test copy_data = true (default)
    uint8_t* originalData = (uint8_t*)malloc(100);
    memset(originalData, 0xAA, 100);
    
    testNotification->send("copy_test", originalData, 100, true);
    
    // Modify original data
    memset(originalData, 0xBB, 100);
    
    // Consumed data should still have original values
    uint8_t* received = (uint8_t*)testNotification->consume("copy_test");
    TEST_ASSERT_NOT_NULL(received);
    TEST_ASSERT_EQUAL_HEX8(0xAA, received[0]);
    TEST_ASSERT_EQUAL_HEX8(0xAA, received[50]);
    TEST_ASSERT_EQUAL_HEX8(0xAA, received[99]);
    
    free(originalData);
    free(received);
}

// Test runner function
void runNotificationTests() {
    UNITY_BEGIN();
    
    RUN_TEST(test_basic_send_consume_int);
    RUN_TEST(test_basic_send_consume_float);
    RUN_TEST(test_string_notification);
    RUN_TEST(test_struct_notification);
    RUN_TEST(test_has_notification);
    RUN_TEST(test_notification_timeout);
    RUN_TEST(test_notification_count);
    RUN_TEST(test_remove_notification);
    RUN_TEST(test_notification_info);
    RUN_TEST(test_overwrite_notification);
    RUN_TEST(test_memory_ownership);
    
    UNITY_END();
}
