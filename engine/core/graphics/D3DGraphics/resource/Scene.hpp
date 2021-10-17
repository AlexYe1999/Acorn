#pragma once
#include"RenderItem.hpp"
#include"Camera.hpp"
#include"Material.hpp"
#include"Mesh.hpp"
#include"vector"
#include<memory>
namespace Acorn{
    struct Scene{

        Camera MainCamera;

        std::vector<std::unique_ptr<RenderItem>> AllRenderItems;

        std::vector<RenderItem*> OpaqueRenderItems;
        std::vector<RenderItem*> TransparentRenderItems;

        std::unordered_map<std::string, std::unique_ptr<Mesh>> Meshes;
        std::unordered_map<std::string, std::unique_ptr<Mesh>> DynamicMeshes;
        std::unordered_map<std::string, std::unique_ptr<Material>> Materials;
    };

}