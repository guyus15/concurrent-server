#define CLOVE_SUITE_NAME CollisionTests
#include <clove-unit.h>

#include <physics/collision.h>

// Test 1
CLOVE_TEST(TestCollisionByDistance)
{
    /**
     * This test ensures that when two positions are given within the specified distance,
     * the ByDistance() method returns true.
     */

    const glm::vec2 p1 = { 0.0f, 0.0f };
    const glm::vec2 p2 = { 0.0f, 4.0f };

    CLOVE_IS_TRUE(Collision::ByDistance(p1, p2, 5.0f));
}

// Test 2
CLOVE_TEST(TestNoCollisionByDistance)
{
    /**
     * This test ensures that when two positions are given beyond the specified distance,
     * the ByDistance() method returns false.
     */

    const glm::vec2 p1 = { 0.0f, 0.0f };
    const glm::vec2 p2 = { 10.0f, 0.0f };

    CLOVE_IS_FALSE(Collision::ByDistance(p1, p2, 5.0f));
}
