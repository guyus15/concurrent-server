#define CLOVE_SUITE_NAME PlayerTests
#include <clove-unit.h>

#include <player.h>

// Test 1
CLOVE_TEST(TestLowerBoundHealthClamp)
{
    /**
     * This test ensures that if the player's health should be decreased to a value lower than
     * the zero, it is clamped to zero.
     */

    Player player{};

    // Lower health to zero
    player.RemoveHealth(100);

    // Try removing another 10 health, the health value should be clamped to zero.
    player.RemoveHealth(10);

    CLOVE_INT_EQ(0, player.GetCurrentHealth());
}

// Test 2
CLOVE_TEST(TestUpperBoundHealthClamp)
{
    /**
     * This test ensures that if the player's health should be increased to a value greater than
     * the maximum value, it is clamped to this maximum value.
     */

    Player player{};

    // Remove 10 health.
    player.RemoveHealth(10);

    // Try adding 20 health, the health value should be clamped to the maximum value.
    player.AddHealth(20);

    CLOVE_INT_EQ(100, player.GetCurrentHealth());
}
