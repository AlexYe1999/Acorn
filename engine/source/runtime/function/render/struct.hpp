#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>

namespace Acorn
{
    enum class INPUT_PRIMITIVE_TOPOLOGY : std::uint8_t
    {
        NONE = 0,
        POINT_LIST,

        LINE_LIST,
        LINE_STRIP,

        TRIANGLE_LIST,
        TRIANGLE_STRIP,
    };

    enum class SHADER_TYPE : std::uint8_t
    {
        NONE_SHADER = 0,
        VERTEX_SHADER, 
        FRAGMENT_SHADER,
        COMPUTE_SHADER
    };

    enum class RASTER_MODE : std::uint8_t
    {
        NONE = 0,
        FILL,
        LINE,
        POINT
    };

    enum class RASTER_CULL_MODE : std::uint8_t
    {
        NONE = 0,
        FRONT,
        BACK,
        ALL
    };

    enum class RENDER_TARGET_WRITE_MASK : std::uint8_t
    {
        NONE = 0x00,
        R    = 0x01,
        G    = 0x02,
        B    = 0x04,
        A    = 0x08,
        RG   = R | G,
        RB   = R | B,
        RA   = R | A,
        GB   = G | B,
        GA   = G | A,
        BA   = B | A,
        RGB  = R | G | B,
        RGA  = R | G | A, 
        RBA  = R | B | A,
        GBA  = G | B | A,
        RGBA = R | G | B | A
    };

    enum class COLOR_BLEND_FACTOR : std::uint8_t
    {
        NONE = 0,
        ZERO,
        ONE
    };

    enum class COLOR_BLEND_OP : std::uint8_t
    {
        NONE = 0,
        ADD
    };

    enum class DYNAMIC_STATE
    {
        NONE,
        VIEW_PORT,
        STATE_LINE_WIDTH
    };

    enum class RENDER_TARGET_FORMAT
    {

    };

    enum class RENDER_TARGET_SAMPLE_COUNT
    {

    };

    enum class RENDER_TARGET_CLEAR_OP
    {
        NONE = 0,
        CLEAR
    };

    enum class RENDER_TARGET_WRITE_OP
    {
        NONE = 0,
        WRITE 
    };

    enum class COMMAND_LIST_TYPE
    {
        NONE = 0,
        GRAPHICS
    };

    enum class COLOR
    {
        NONE = 0,
        R8G8B8A8,
        R16G16B16A16,
        R32G32B32A32
    };

    struct InputAssemblyDescriptor
    {
        INPUT_PRIMITIVE_TOPOLOGY primitive_topology;
        bool                     primitive_restart;
    };
    static_assert(sizeof(InputAssemblyDescriptor) == 2);

    struct ShaderDescriptor
    {
        SHADER_TYPE shader_type;
        std::string shader_path;
        std::string shader_entry;
    };

    struct RasterizerDescriptor
    {
        RASTER_MODE raster_mode;
        RASTER_CULL_MODE raster_cull_mode;
        bool  depth_clip_enable;
        bool  depth_bias_enable;
        bool  output_discard_enable;
        bool  front_face_clockwise;
        float depth_bias_constant_factor;
        float depth_bias_clamp;
        float depth_bias_slope_factor;
    };
    static_assert(sizeof(RasterizerDescriptor) == 20);

    struct MultisampleDescriptor
    {

    };

    struct DepthStencilDescriptor
    {

    };

    struct OutputMergementDescriptor
    {
        bool                     color_blend_enable;
        COLOR_BLEND_FACTOR       blend_factor_new_color;
        COLOR_BLEND_FACTOR       blend_factor_old_color;
        COLOR_BLEND_OP           blend_op_color;
        COLOR_BLEND_FACTOR       blend_factor_new_alpha;
        COLOR_BLEND_FACTOR       blend_factor_old_alpha;
        COLOR_BLEND_OP           blend_op_alpha;
        RENDER_TARGET_WRITE_MASK render_target_write_mask;
    };
    static_assert(sizeof(OutputMergementDescriptor) == 8);

    struct RenderTargetDescriptor
    {

    };

    struct Viewport
    {
        float x;
        float y;
        float width;
        float height;
        float depth_min;
        float depth_max;
    };

    struct ScissorsRect
    {

    };


    struct GraphicsPipelineStateDescriptor
    {
        using ShaderDescriptors = std::vector<ShaderDescriptor>;
        using DynamicStates     = std::vector<DYNAMIC_STATE>;

        InputAssemblyDescriptor   input_assembly_descriptor;
        ShaderDescriptors         shader_descriptors;
        RasterizerDescriptor      rasterizer_descriptor;
        MultisampleDescriptor     multisample_descriptor;
        DepthStencilDescriptor    depth_stencil_descriptor;
        OutputMergementDescriptor output_mergement_descriptor;
        DynamicStates             dynamic_states;
    };


}