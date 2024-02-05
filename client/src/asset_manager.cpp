#include "rendering/shader.h"
#include "rendering/texture2d.h"

#include <common/assets/asset_manager.h>

template <>
AssetManager<Shader> AssetManager<Shader>::s_instance{};

template <>
AssetManager<Texture2d> AssetManager<Texture2d>::s_instance{};
