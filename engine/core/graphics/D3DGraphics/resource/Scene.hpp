#pragma once
#include"RenderItem.hpp"
#include"Camera.hpp"
#include"vector"
#include<memory>
namespace Acorn{
    struct Scene{

        Camera m_MainCamera;

        std::unique_ptr<RenderItem[]> m_pAllRenderItems = nullptr;

        std::vector<RenderItem*> m_pOpaqueRenderItems;
        std::vector<RenderItem*> m_pTransparentRenderItems;
    };

}