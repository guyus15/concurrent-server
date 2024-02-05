#include <common/level.h>

#include <common/assets/asset_manager.h>

template <>
AssetManager<Level> AssetManager<Level>::s_instance{};
