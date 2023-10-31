#include "asset_manager.h"

#include "rendering/shader.h"
#include "rendering/texture2d.h"

AssetManager<Shader> AssetManager<Shader>::s_instance{};
AssetManager<Texture2d> AssetManager<Texture2d>::s_instance{};