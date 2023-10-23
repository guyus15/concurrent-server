#include "asset_manager.h"
#include "rendering/shader.h"

#define CLOVE_SUITE_NAME AssetManagerTests
#include <clove-unit.h>

CLOVE_TEST(FirstTest)
{
    const Shader& shader = AssetManager<Shader>::LoadOrRetrieve("E:/Programming/C++/fyp/client/resources/vertex.glsl",
                                                                "E:/Programming/C++/fyp/client/resources/fragment.glsl");

    CLOVE_IS_TRUE(shader.GetAssetId() != 0)
}
