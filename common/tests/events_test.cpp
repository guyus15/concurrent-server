#define CLOVE_SUITE_NAME EventsTests
#include <clove-unit.h>

#include <common/events/event_manager.h>

struct TestEvent final : GameEvent
{
};

struct SecondTestEvent final : GameEvent
{
};

struct TestEventWithParams final : GameEvent
{
    bool test_bool{};
    int test_int{};
};

static bool test_listener1_ran, test_listener2_ran, test_listener3_ran;

void TestListener1([[maybe_unused]] GameEvent& evt)
{
    test_listener1_ran = true;
}

void TestListener2([[maybe_unused]] GameEvent& evt)
{
    test_listener2_ran = true;
}

void TestListener3([[maybe_unused]] GameEvent& evt)
{
    test_listener3_ran = true;
}

static bool global_test_bool = false;
static int global_test_int = 0;

void TestListenerWithEventParams(GameEvent& evt)
{
    const auto test_evt = dynamic_cast<TestEventWithParams&>(evt);

    global_test_bool = test_evt.test_bool;
    global_test_int = test_evt.test_int;
}

CLOVE_SUITE_SETUP()
{
    test_listener1_ran = test_listener2_ran = test_listener3_ran = false;
    EventManager::ClearAllListeners();
}

// Test 1
CLOVE_TEST(TestAddListener)
{
    /**
     * This test ensures that calling AddListener<T>() successfully
     * binds a listener to an event.
     */

    EventManager::AddListener<TestEvent>(TestListener1);

    TestEvent test_evt{};
    EventManager::Broadcast(test_evt);

    CLOVE_IS_TRUE(test_listener1_ran);
}

// Test 2
CLOVE_TEST(TestAddMultipleListeners)
{
    /**
     * This test ensures that calling AddListener<T>() multiple times
     * successfully binds each listener to an event.
     */

    EventManager::AddListener<TestEvent>(TestListener1);
    EventManager::AddListener<TestEvent>(TestListener2);
    EventManager::AddListener<TestEvent>(TestListener3);

    TestEvent test_evt{};
    EventManager::Broadcast(test_evt);

    CLOVE_IS_TRUE(test_listener1_ran);
    CLOVE_IS_TRUE(test_listener2_ran);
    CLOVE_IS_TRUE(test_listener3_ran);
}

// Test 3
CLOVE_TEST(TestRemoveListener)
{
    /**
     * This test ensures that calling RemoveListener<T>() successfully
     * removes the binding from the specified listener to the given event.
     */

    EventManager::AddListener<TestEvent>(TestListener1);
    EventManager::RemoveListener<TestEvent>(TestListener1);

    TestEvent test_evt{};
    EventManager::Broadcast(test_evt);

    CLOVE_IS_FALSE(test_listener1_ran);
}

// Test 4
CLOVE_TEST(TestRemoveOneListener)
{
    /**
     * This test ensures that if we add multiple listeners to an event
     * and remove one, all listeners but theone we removed will be called.
     */

    EventManager::AddListener<TestEvent>(TestListener1);
    EventManager::AddListener<TestEvent>(TestListener2);
    EventManager::AddListener<TestEvent>(TestListener3);

    EventManager::RemoveListener<TestEvent>(TestListener2);

    TestEvent test_evt{};
    EventManager::Broadcast(test_evt);

    CLOVE_IS_TRUE(test_listener1_ran);
    CLOVE_IS_FALSE(test_listener2_ran);
    CLOVE_IS_TRUE(test_listener3_ran);
}

// Test 5
CLOVE_TEST(TestClearListeners)
{
    /**
     * This test ensures that after calling ClearListeners<T>(),
     * all listeners bound to event T will be removed.
     */

    EventManager::AddListener<TestEvent>(TestListener1);
    EventManager::AddListener<TestEvent>(TestListener2);
    EventManager::AddListener<TestEvent>(TestListener3);

    EventManager::ClearListeners<TestEvent>();

    TestEvent test_evt{};
    EventManager::Broadcast(test_evt);

    CLOVE_IS_FALSE(test_listener1_ran);
    CLOVE_IS_FALSE(test_listener2_ran);
    CLOVE_IS_FALSE(test_listener3_ran);
}

// Test 6
CLOVE_TEST(TestClearAllListeners)
{
    /**
     * This test ensures that after calling ClearAllListeners(),
     * the event manager will clear all bound listeners from every
     * event.
     */

    EventManager::AddListener<TestEvent>(TestListener1);
    EventManager::AddListener<TestEvent>(TestListener2);
    EventManager::AddListener<TestEvent>(TestListener2);

    EventManager::AddListener<SecondTestEvent>(TestListener1);
    EventManager::AddListener<SecondTestEvent>(TestListener2);
    EventManager::AddListener<SecondTestEvent>(TestListener2);

    EventManager::ClearAllListeners();

    TestEvent test_evt{};
    EventManager::Broadcast(test_evt);

    CLOVE_IS_FALSE(test_listener1_ran);
    CLOVE_IS_FALSE(test_listener2_ran);
    CLOVE_IS_FALSE(test_listener3_ran);

    SecondTestEvent second_test_evt{};
    EventManager::Broadcast(second_test_evt);

    CLOVE_IS_FALSE(test_listener1_ran);
    CLOVE_IS_FALSE(test_listener2_ran);
    CLOVE_IS_FALSE(test_listener3_ran);
}

// Test 7
CLOVE_TEST(TestEventWithParams)
{
    /**
     * This test ensures that if an event contains parameters,
     * they are passed successfully to the listener.
     */

    EventManager::AddListener<TestEventWithParams>(TestListenerWithEventParams);

    TestEventWithParams test_evt{};
    test_evt.test_bool = true;
    test_evt.test_int = 10;

    EventManager::Broadcast(test_evt);

    CLOVE_IS_TRUE(global_test_bool);
    CLOVE_INT_EQ(10, global_test_int);
}

// Test 8
CLOVE_TEST(TestRemoveNonExistentListener)
{
    /**
     * This test ensures that if RemoveListener<T>() is called when
     * either no listeners exist for the given event, or the specified
     * listener is not found for the current event, any bound listeners
     * will behave normally.
     */

    EventManager::AddListener<TestEvent>(TestListener1);
    EventManager::AddListener<TestEvent>(TestListener2);
    EventManager::AddListener<TestEvent>(TestListener3);

    // Trying to remove a listener which hasn't been bound to this event.
    EventManager::RemoveListener<TestEvent>(TestListenerWithEventParams);

    TestEvent test_evt{};
    EventManager::Broadcast(test_evt);

    CLOVE_IS_TRUE(test_listener1_ran);
    CLOVE_IS_TRUE(test_listener2_ran);
    CLOVE_IS_TRUE(test_listener3_ran);
}
