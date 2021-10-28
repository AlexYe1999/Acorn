#pragma once
#include"RenderItem.hpp"
#include"Camera.hpp"
#include"Material.hpp"
#include"Mesh.hpp"
#include"Texture.hpp"
#include<vector>
#include<array>
#include<memory>
namespace Acorn{
    struct Scene{
        using RenderItems = std::vector<RenderItem*>;

        Camera MainCamera;

        std::vector<std::unique_ptr<RenderItem>> AllRenderItems;
        std::array<RenderItems, static_cast<uint16_t>(RenderLayer::LayerCount)> RenderLayers;

        std::unordered_map<std::string, std::unique_ptr<Mesh>> Meshes;
        std::unordered_map<std::string, std::unique_ptr<Mesh>> DynamicMeshes;
        std::unordered_map<std::string, std::unique_ptr<Material>> Materials;
        std::unordered_map<std::string, std::unique_ptr<Texture>> Textures;
    };

}