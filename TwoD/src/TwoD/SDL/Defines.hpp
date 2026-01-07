#pragma once
#include "yaml-cpp/yaml.h"

namespace TwoD
{
    class Window;
}

namespace TwoD::SDL
{
	// *****************************************************
	// Declerations ****************************************
	// *****************************************************

    class Surface;
	class Texture;
	class Sampler;
	class Buffer;
	class CommandBuffer;
	class CopyPass;
	class GraphicsPipeline;
	class RenderPass;
	class Shader;
	class TransferBuffer;

    class Font;
	
    // *****************************************************
    // Misc ************************************************
    // *****************************************************
   
    enum class Filter
    {
        NEAREST,
        LINEAR
    };
    enum class CompareOp
    {
        INVALID,
        NEVER,
        LESS,
        EQUAL,
        LESS_OR_EQUAL,
        GREATER,
        NOT_EQUAL,
        GREATER_OR_EQUAL,
        ALWAYS
    };
    enum class BlendFactor
    {
        INVALID,
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        SRC_ALPHA_SATURATE
    };
    enum class BlendOp
    {
        INVALID,
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX
    };
    enum class SampleCount
    {
        ONE,
        TWO,
        FOUR,
        EIGHT,
    };
    struct Rect
    {
        uint32_t x, y;
        uint32_t w, h;
    };

	// *****************************************************
	// Buffer **********************************************
	// *****************************************************

	enum class BufferUsage : uint32_t
	{
		VERTEX = (1u << 0),
		INDEX = (1u << 1),
		INDIRECT = (1u << 2),
		GRAPHICS_STORAGE_READ = (1u << 3),
		COMPUTE_STORAGE_READ = (1u << 4),
		COMPUTE_STORAGE_WRITE = (1u << 5)
	};
	struct BufferInfo
	{
		BufferUsage usage;
		uint32_t size;
	};

	// *****************************************************
	// TransferBuffer **************************************
	// *****************************************************

	enum class TransferBufferUsage
	{
		UPLOAD,
		DOWNLOAD
	};
	struct TransferBufferInfo
	{
		TransferBufferUsage usage;
		uint32_t size;
	};

    // *****************************************************
    // Texture *********************************************
    // *****************************************************

    enum class TextureFormat
    {
        INVALID,
        /* Unsigned Normalized Float Color Formats */
        A8_UNORM,
        R8_UNORM,
        R8G8_UNORM,
        R8G8B8A8_UNORM,
        R16_UNORM,
        R16G16_UNORM,
        R16G16B16A16_UNORM,
        R10G10B10A2_UNORM,
        B5G6R5_UNORM,
        B5G5R5A1_UNORM,
        B4G4R4A4_UNORM,
        B8G8R8A8_UNORM,
        /* Compressed Unsigned Normalized Float Color Formats */
        BC1_RGBA_UNORM,
        BC2_RGBA_UNORM,
        BC3_RGBA_UNORM,
        BC4_R_UNORM,
        BC5_RG_UNORM,
        BC7_RGBA_UNORM,
        /* Compressed Signed Float Color Formats */
        BC6H_RGB_FLOAT,
        /* Compressed Unsigned Float Color Formats */
        BC6H_RGB_UFLOAT,
        /* Signed Normalized Float Color Formats  */
        R8_SNORM,
        R8G8_SNORM,
        R8G8B8A8_SNORM,
        R16_SNORM,
        R16G16_SNORM,
        R16G16B16A16_SNORM,
        /* Signed Float Color Formats */
        R16_FLOAT,
        R16G16_FLOAT,
        R16G16B16A16_FLOAT,
        R32_FLOAT,
        R32G32_FLOAT,
        R32G32B32A32_FLOAT,
        /* Unsigned Float Color Formats */
        R11G11B10_UFLOAT,
        /* Unsigned Integer Color Formats */
        R8_UINT,
        R8G8_UINT,
        R8G8B8A8_UINT,
        R16_UINT,
        R16G16_UINT,
        R16G16B16A16_UINT,
        R32_UINT,
        R32G32_UINT,
        R32G32B32A32_UINT,
        /* Signed Integer Color Formats */
        R8_INT,
        R8G8_INT,
        R8G8B8A8_INT,
        R16_INT,
        R16G16_INT,
        R16G16B16A16_INT,
        R32_INT,
        R32G32_INT,
        R32G32B32A32_INT,
        /* SRGB Unsigned Normalized Color Formats */
        R8G8B8A8_UNORM_SRGB,
        B8G8R8A8_UNORM_SRGB,
        /* Compressed SRGB Unsigned Normalized Color Formats */
        BC1_RGBA_UNORM_SRGB,
        BC2_RGBA_UNORM_SRGB,
        BC3_RGBA_UNORM_SRGB,
        BC7_RGBA_UNORM_SRGB,
        /* Depth Formats */
        D16_UNORM,
        D24_UNORM,
        D32_FLOAT,
        D24_UNORM_S8_UINT,
        D32_FLOAT_S8_UINT,
        /* Compressed ASTC Normalized Float Color Formats*/
        ASTC_4x4_UNORM,
        ASTC_5x4_UNORM,
        ASTC_5x5_UNORM,
        ASTC_6x5_UNORM,
        ASTC_6x6_UNORM,
        ASTC_8x5_UNORM,
        ASTC_8x6_UNORM,
        ASTC_8x8_UNORM,
        ASTC_10x5_UNORM,
        ASTC_10x6_UNORM,
        ASTC_10x8_UNORM,
        ASTC_10x10_UNORM,
        ASTC_12x10_UNORM,
        ASTC_12x12_UNORM,
        /* Compressed SRGB ASTC Normalized Float Color Formats*/
        ASTC_4x4_UNORM_SRGB,
        ASTC_5x4_UNORM_SRGB,
        ASTC_5x5_UNORM_SRGB,
        ASTC_6x5_UNORM_SRGB,
        ASTC_6x6_UNORM_SRGB,
        ASTC_8x5_UNORM_SRGB,
        ASTC_8x6_UNORM_SRGB,
        ASTC_8x8_UNORM_SRGB,
        ASTC_10x5_UNORM_SRGB,
        ASTC_10x6_UNORM_SRGB,
        ASTC_10x8_UNORM_SRGB,
        ASTC_10x10_UNORM_SRGB,
        ASTC_12x10_UNORM_SRGB,
        ASTC_12x12_UNORM_SRGB,
        /* Compressed ASTC Signed Float Color Formats*/
        ASTC_4x4_FLOAT,
        ASTC_5x4_FLOAT,
        ASTC_5x5_FLOAT,
        ASTC_6x5_FLOAT,
        ASTC_6x6_FLOAT,
        ASTC_8x5_FLOAT,
        ASTC_8x6_FLOAT,
        ASTC_8x8_FLOAT,
        ASTC_10x5_FLOAT,
        ASTC_10x6_FLOAT,
        ASTC_10x8_FLOAT,
        ASTC_10x10_FLOAT,
        ASTC_12x10_FLOAT,
        ASTC_12x12_FLOAT
    };
    enum class TextureType
    {
        TWO_D,
        TWO_D_ARRAY,
        THREE_D,
        CUBE,
        CUBE_ARRAY
    };
    enum class TextureUsageFlags : uint32_t
    {
        SAMPLER = (1u << 0),
        COLOR_TARGET = (1u << 1),
        DEPTH_STENCIL_TARGET = (1u << 2),
        GRAPHICS_STORAGE_READ = (1u << 3),
        COMPUTE_STORAGE_READ = (1u << 4),
        COMPUTE_STORAGE_WRITE = (1u << 5),
        COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE = (1u << 6)
    };
    struct TextureInfo
    {
        TextureType type;
        TextureFormat format;
        TextureUsageFlags usage;
        uint32_t width;
        uint32_t height;
        uint32_t layerCountOrDepth;
        uint32_t numLevels;
        SampleCount sampleCount;
    };

    constexpr TextureUsageFlags operator|(TextureUsageFlags a, TextureUsageFlags b) noexcept
    {
        return static_cast<TextureUsageFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    // *****************************************************
    // Sampler *********************************************
    // *****************************************************

    enum class SamplerMipmapMode
    {
        NEAREST,
        LINEAR
    };
    enum class SamplerAddressMode
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE
    };
    struct SamplerInfo
    {
        Filter minFilter;
        Filter magFilter;
        SamplerMipmapMode mipmapMode;
        SamplerAddressMode addressModeU;
        SamplerAddressMode addressModeV;
        SamplerAddressMode addressModeW;
        float mipLodBias;
        float maxAnisotropy;
        CompareOp compareOp;
        float minLod;
        float maxLod;
        bool enableAnisotropy;
        bool enableCompare;
    };

    // *****************************************************
    // Shader **********************************************
    // *****************************************************

    enum class ShaderFormat : uint32_t
    {
        INVALID  = 0,
        PRIVATE  = (1u << 0),
        SPIRV    = (1u << 1),
        DXBC     = (1u << 2),
        DXIL     = (1u << 3),
        MSL      = (1u << 4),
        METALLIB = (1u << 5)
    };
    enum class ShaderStage
    {
        VERTEX,
        FRAGMENT
    };
    struct ShaderInfo
    {
        uint32_t codeSize;
        uint8_t* code;
        std::string entrypoint;
        ShaderFormat format;
        ShaderStage stage;
        uint32_t numSamplers;
        uint32_t numStorageTextures;
        uint32_t numStorageBuffers;
        uint32_t numUniformBuffers;
    };

    constexpr ShaderFormat operator|(ShaderFormat a, ShaderFormat b) noexcept
    {
        return static_cast<ShaderFormat>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }
    constexpr ShaderFormat operator&(ShaderFormat a, ShaderFormat b) noexcept
    {
        return static_cast<ShaderFormat>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
    constexpr bool operator==(ShaderFormat a, ShaderFormat b) noexcept
    {
        return static_cast<uint32_t>(a) == static_cast<uint32_t>(b);
    }
    constexpr bool operator!=(ShaderFormat a, ShaderFormat b) noexcept
    {
        return static_cast<uint32_t>(a) != static_cast<uint32_t>(b);
    }

    // *****************************************************
    // RenderPass ******************************************
    // *****************************************************

    enum class IndexElementSize
    {
        SIXTEEN_BIT,
        THIRTY_TWO_BIT
    };
    struct TextureSamplerBinding
    {
        Texture* texture;
        Sampler* sampler;
    };
    struct BufferBinding
    {
        Buffer* buffer;
        uint32_t offset;
    };

    // *****************************************************
    // CopyPass ********************************************
    // *****************************************************

    struct TextureTransferInfo
    {
        TransferBuffer* transferBuffer;
        uint32_t offset;
        uint32_t pixelsPerRow;
        uint32_t rowsPerLayer;
    };
    struct TextureRegion
    {
        Texture* texture;
        uint32_t mipLevel;
        uint32_t layer;
        uint32_t x;
        uint32_t y;
        uint32_t z;
        uint32_t w;
        uint32_t h;
        uint32_t d;
    };
    struct TransferBufferLocation
    {
        const TransferBuffer* transferBuffer;
        uint32_t offset;
    };
    struct BufferRegion
    {
        const Buffer* buffer;
        uint32_t offset;
        uint32_t size;
    };

    // *****************************************************
    // GraphicsPipeline ************************************
    // *****************************************************

    enum class VertexElementFormat
    {
        INVALID,
        /* 32-bit Signed Integers */
        INT,
        INT2,
        INT3,
        INT4,
        /* 32-bit Unsigned Integers */
        UINT,
        UINT2,
        UINT3,
        UINT4,
        /* 32-bit Floats */
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        /* 8-bit Signed Integers */
        BYTE2,
        BYTE4,
        /* 8-bit Unsigned Integers */
        UBYTE2,
        UBYTE4,
        /* 8-bit Signed Normalized */
        BYTE2_NORM,
        BYTE4_NORM,
        /* 8-bit Unsigned Normalized */
        UBYTE2_NORM,
        UBYTE4_NORM,
        /* 16-bit Signed Integers */
        SHORT2,
        SHORT4,
        /* 16-bit Unsigned Integers */
        USHORT2,
        USHORT4,
        /* 16-bit Signed Normalized */
        SHORT2_NORM,
        SHORT4_NORM,
        /* 16-bit Unsigned Normalized */
        USHORT2_NORM,
        USHORT4_NORM,
        /* 16-bit Floats */
        HALF2,
        HALF4
    };
    enum class VertexInputRate
    {
        VERTEX,
        INSTANCE
    };
    enum class PrimitiveType
    {
        TRIANGLELIST,
        TRIANGLESTRIP,
        LINELIST,
        LINESTRIP,
        POINTLIST
    };
    enum class FrontFace
    {
        COUNTER_CLOCKWISE,
        CLOCKWISE
    };
    enum class CullMode
    {
        NONE,
        FRONT,
        BACK
    };
    enum class FillMode
    {
        FILL,
        LINE
    };
    enum class StencilOp
    {
        INVALID,
        KEEP,
        ZERO,
        REPLACE,
        INCREMENT_AND_CLAMP,
        DECREMENT_AND_CLAMP,
        INVERT,
        INCREMENT_AND_WRAP,
        DECREMENT_AND_WRAP
    };
    enum class ColorComponentFlags : uint8_t
    {
        R = (1u << 0),
        G = (1u << 1),
        B = (1u << 2),
        A = (1u << 3)
    };
    struct ColorTargetBlendState
    {
        BlendFactor srcColorBlendfactor;
        BlendFactor dstColorBlendfactor;
        BlendOp colorBlendOp;
        BlendFactor srcAlphaBlendfactor;
        BlendFactor dstAlphaBlendfactor;
        BlendOp alphaBlendOp;
        ColorComponentFlags colorWriteMask;
        bool enableBlend;
        bool enableColorWriteMask;
    };
    struct VertexBufferDescription
    {
        uint32_t slot;
        uint32_t pitch;
        VertexInputRate inputRate;
    };
    struct VertexAttribute
    {
        uint32_t location;
        uint32_t bufferSlot;
        VertexElementFormat format;
        uint32_t offset;
    };
    struct StencilOpState
    {
        StencilOp failOp;
        StencilOp passOp;
        StencilOp depthFailOp;
        CompareOp compareOp;
    };
    struct ColorTargetDescription
    {
        TextureFormat format;
        ColorTargetBlendState blendState;
    };
    struct VertexInputState
    {
        std::vector<VertexBufferDescription> vertexBufferDescriptions;
        std::vector<VertexAttribute> vertexAttributes;
    };
    struct RasterizerState
    {
        FillMode fillMode;
        CullMode cullMode;
        FrontFace frontFace;
        float depthBiasConstantFactor;
        float depthBiasClamp;
        float depthBiasSlopeFactor;
        bool enableDepthBias;
        bool enableDepthClip;
    };
    struct MultisampleState
    {
        SampleCount sampleCount;
        uint32_t sampleMask;
        bool enableMask;
    };
    struct DepthStencilState
    {
        CompareOp compareOp;
        StencilOpState backStencilState;
        StencilOpState frontStencilState;
        uint8_t compareMask;
        uint8_t writeMask;
        bool enableDepthTest;
        bool enableDepthWrite;
        bool enableStencilTest;
    };
    struct GraphicsPipelineTargetInfo
    {
        std::vector<ColorTargetDescription> colorTargetDescriptions;
        TextureFormat depthStencilFormat;
        bool hasDepthStencilTarget;
    };
    struct GraphicsPipelineInfo
    {
        Shader* vertexShader;
        Shader* fragmentShader;
        VertexInputState vertexInputState;
        PrimitiveType primitiveType;
        RasterizerState rasterizerState;
        MultisampleState multisampleState;
        DepthStencilState depthStencilState;
        GraphicsPipelineTargetInfo targetInfo;
    };

    constexpr ColorComponentFlags operator|(ColorComponentFlags a, ColorComponentFlags b) noexcept
    {
        return static_cast<ColorComponentFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    // *****************************************************
    // Surface *********************************************
    // *****************************************************

    enum class PixelFormat
    {
        UNKNOWN = 0,
        INDEX1LSB = 0x11100100u,
        INDEX1MSB = 0x11200100u,
        INDEX2LSB = 0x1c100200u,
        INDEX2MSB = 0x1c200200u,
        INDEX4LSB = 0x12100400u,
        INDEX4MSB = 0x12200400u,
        INDEX8 = 0x13000801u,
        RGB332 = 0x14110801u,
        XRGB4444 = 0x15120c02u,
        XBGR4444 = 0x15520c02u,
        XRGB1555 = 0x15130f02u,
        XBGR1555 = 0x15530f02u,
        ARGB4444 = 0x15321002u,
        RGBA4444 = 0x15421002u,
        ABGR4444 = 0x15721002u,
        BGRA4444 = 0x15821002u,
        ARGB1555 = 0x15331002u,
        RGBA5551 = 0x15441002u,
        ABGR1555 = 0x15731002u,
        BGRA5551 = 0x15841002u,
        RGB565 = 0x15151002u,
        BGR565 = 0x15551002u,
        RGB24 = 0x17101803u,
        BGR24 = 0x17401803u,
        XRGB8888 = 0x16161804u,
        RGBX8888 = 0x16261804u,
        XBGR8888 = 0x16561804u,
        BGRX8888 = 0x16661804u,
        ARGB8888 = 0x16362004u,
        RGBA8888 = 0x16462004u,
        ABGR8888 = 0x16762004u,
        BGRA8888 = 0x16862004u,
        XRGB2101010 = 0x16172004u,
        XBGR2101010 = 0x16572004u,
        ARGB2101010 = 0x16372004u,
        ABGR2101010 = 0x16772004u,
        RGB48 = 0x18103006u,
        BGR48 = 0x18403006u,
        RGBA64 = 0x18204008u,
        ARGB64 = 0x18304008u,
        BGRA64 = 0x18504008u,
        ABGR64 = 0x18604008u,
        RGB48_FLOAT = 0x1a103006u,
        BGR48_FLOAT = 0x1a403006u,
        RGBA64_FLOAT = 0x1a204008u,
        ARGB64_FLOAT = 0x1a304008u,
        BGRA64_FLOAT = 0x1a504008u,
        ABGR64_FLOAT = 0x1a604008u,
        RGB96_FLOAT = 0x1b10600cu,
        BGR96_FLOAT = 0x1b40600cu,
        RGBA128_FLOAT = 0x1b208010u,
        ARGB128_FLOAT = 0x1b308010u,
        BGRA128_FLOAT = 0x1b508010u,
        ABGR128_FLOAT = 0x1b608010u,
        YV12 = 0x32315659,
        IYUV = 0x56555949,
        YUY2 = 0x32595559,
        UYVY = 0x59565955,
        YVYU = 0x55595659,
        NV12 = 0x3231564e,
        NV21 = 0x3132564e,
        P010 = 0x30313050,
        EXTERNAL_OES = 0x2053454f,
        MJPG = 0x47504a4d,
    };
}

namespace YAML
{
    template<>
    struct convert<TwoD::SDL::Filter>
    {
        static Node encode(const TwoD::SDL::Filter rhs)
        {
            if (rhs == TwoD::SDL::Filter::NEAREST)
            {
                return Node("nearest");
            }
            else
            {
                return Node("linear");
            }
        }
        static bool decode(const Node& node, TwoD::SDL::Filter& rhs)
        {
            auto str = node.as<std::string>();
            if (str == "nearest")
            {
                rhs = TwoD::SDL::Filter::NEAREST;
                return true;
            }
            if (str == "linear")
            {
                rhs = TwoD::SDL::Filter::LINEAR;
                return true;
            }
            return false;
        }
    };

    template<>
    struct convert<TwoD::SDL::TextureFormat>
    {
        static Node encode(const TwoD::SDL::TextureFormat rhs)
        {
            switch(rhs)
            {
            case TwoD::SDL::TextureFormat::A8_UNORM:
                return Node("a8-unorm");
            case TwoD::SDL::TextureFormat::R8_UNORM:
                return Node("r8-unorm");
            case TwoD::SDL::TextureFormat::R8G8_UNORM:
                return Node("r8g8-unorm");
            case TwoD::SDL::TextureFormat::R8G8B8A8_UNORM:
                return Node("r8g8b8a8-unorm");
            case TwoD::SDL::TextureFormat::R16_UNORM:
                return Node("r16-unorm");
            case TwoD::SDL::TextureFormat::R16G16_UNORM:
                return Node("r16b16-unorm");
            case TwoD::SDL::TextureFormat::R16G16B16A16_UNORM:
                return Node("r16g16b16a16-unorm");
            case TwoD::SDL::TextureFormat::R10G10B10A2_UNORM:
                return Node("r10g10b10a1-unorm");
            case TwoD::SDL::TextureFormat::B5G6R5_UNORM:
                return Node("b5g6r5-unorm");
            case TwoD::SDL::TextureFormat::B5G5R5A1_UNORM:
                return Node("b5g5r5a1-unorm");
            case TwoD::SDL::TextureFormat::B4G4R4A4_UNORM:
                return Node("b4g4r4a4-unorm");
            case TwoD::SDL::TextureFormat::B8G8R8A8_UNORM:
                return Node("b8g8r8a8-unorm");
            case TwoD::SDL::TextureFormat::BC1_RGBA_UNORM:
                return Node("bc1-rgba-unorm");
            case TwoD::SDL::TextureFormat::BC2_RGBA_UNORM:
                return Node("bc2-rgba-unorm");
            case TwoD::SDL::TextureFormat::BC3_RGBA_UNORM:
                return Node("bc3-rgba-unorm");
            case TwoD::SDL::TextureFormat::BC4_R_UNORM:
                return Node("bc4-r-unorm");
            case TwoD::SDL::TextureFormat::BC5_RG_UNORM:
                return Node("bc5-rg-unorm");
            case TwoD::SDL::TextureFormat::BC7_RGBA_UNORM:
                return Node("bc7-rgba-unorm");
            case TwoD::SDL::TextureFormat::BC6H_RGB_FLOAT:
                return Node("bc6h-rgb-float");
            case TwoD::SDL::TextureFormat::BC6H_RGB_UFLOAT:
                return Node("bc6h-rgb-ufloat");
            case TwoD::SDL::TextureFormat::R8_SNORM:
                return Node("r8-snorm");
            case TwoD::SDL::TextureFormat::R8G8_SNORM:
                return Node("r8g8-snorm");
            case TwoD::SDL::TextureFormat::R8G8B8A8_SNORM:
                return Node("r8g8b8a8-snorm");
            case TwoD::SDL::TextureFormat::R16_SNORM:
                return Node("r16-snorm");
            case TwoD::SDL::TextureFormat::R16G16_SNORM:
                return Node("r16g16-snorm");
            case TwoD::SDL::TextureFormat::R16G16B16A16_SNORM:
                return Node("r16g16b16a16-snorm");
            case TwoD::SDL::TextureFormat::R16_FLOAT:
                return Node("r16-float");
            case TwoD::SDL::TextureFormat::R16G16_FLOAT:
                return Node("r16g16-float");
            case TwoD::SDL::TextureFormat::R16G16B16A16_FLOAT:
                return Node("r16g16b16a16-float");
            case TwoD::SDL::TextureFormat::R32_FLOAT:
                return Node("r32-float");
            case TwoD::SDL::TextureFormat::R32G32_FLOAT:
                return Node("r32g32-float");
            case TwoD::SDL::TextureFormat::R32G32B32A32_FLOAT:
                return Node("r32g32b32a32-float");
            case TwoD::SDL::TextureFormat::R11G11B10_UFLOAT:
                return Node("r11g11b10-ufloat");
            case TwoD::SDL::TextureFormat::R8_UINT:
                return Node("r8-uint");
            case TwoD::SDL::TextureFormat::R8G8_UINT:
                return Node("r8g8-uint");
            case TwoD::SDL::TextureFormat::R8G8B8A8_UINT:
                return Node("r8g8b8a8-uint");
            case TwoD::SDL::TextureFormat::R16_UINT:
                return Node("r16-uint");
            case TwoD::SDL::TextureFormat::R16G16_UINT:
                return Node("r16g16-uint");
            case TwoD::SDL::TextureFormat::R16G16B16A16_UINT:
                return Node("r16g16b16a16-uint");
            case TwoD::SDL::TextureFormat::R32_UINT:
                return Node("r32-uint");
            case TwoD::SDL::TextureFormat::R32G32_UINT:
                return Node("r32g32-uint");
            case TwoD::SDL::TextureFormat::R32G32B32A32_UINT:
                return Node("r32g32b32a32-uint");
            case TwoD::SDL::TextureFormat::R8_INT:
                return Node("r8-int");
            case TwoD::SDL::TextureFormat::R8G8_INT:
                return Node("r8g8-int");
            case TwoD::SDL::TextureFormat::R8G8B8A8_INT:
                return Node("r8g8b8a8-int");
            case TwoD::SDL::TextureFormat::R16_INT:
                return Node("r16-int");
            case TwoD::SDL::TextureFormat::R16G16_INT:
                return Node("r16g16-int");
            case TwoD::SDL::TextureFormat::R16G16B16A16_INT:
                return Node("r16g16b16a16-int");
            case TwoD::SDL::TextureFormat::R32_INT:
                return Node("r32-int");
            case TwoD::SDL::TextureFormat::R32G32_INT:
                return Node("r32g32-int");
            case TwoD::SDL::TextureFormat::R32G32B32A32_INT:
                return Node("r32g32b32a32-int");
            case TwoD::SDL::TextureFormat::R8G8B8A8_UNORM_SRGB:
                return Node("r8g8b8a8-unorm-srgb");
            case TwoD::SDL::TextureFormat::B8G8R8A8_UNORM_SRGB:
                return Node("b8g8r8a8-unorm-srgb");
            case TwoD::SDL::TextureFormat::BC1_RGBA_UNORM_SRGB:
                return Node("bc1-rgba-unorm-srgb");
            case TwoD::SDL::TextureFormat::BC2_RGBA_UNORM_SRGB:
                return Node("bc2-rgba-unorm-srgb");
            case TwoD::SDL::TextureFormat::BC3_RGBA_UNORM_SRGB:
                return Node("bc3-rgba-unorm-srgb");
            case TwoD::SDL::TextureFormat::BC7_RGBA_UNORM_SRGB:
                return Node("bc7-rgba-unorm-srgb");
            case TwoD::SDL::TextureFormat::D16_UNORM:
                return Node("d16-unorm");
            case TwoD::SDL::TextureFormat::D24_UNORM:
                return Node("d24-unorm");
            case TwoD::SDL::TextureFormat::D32_FLOAT:
                return Node("d32-float");
            case TwoD::SDL::TextureFormat::D24_UNORM_S8_UINT:
                return Node("d24-unorm-s8-uint");
            case TwoD::SDL::TextureFormat::D32_FLOAT_S8_UINT:
                return Node("d32-float-s8-uint");
            case TwoD::SDL::TextureFormat::ASTC_4x4_UNORM:
                return Node("astc-4x4-unorm");
            case TwoD::SDL::TextureFormat::ASTC_5x4_UNORM:
                return Node("astc-5x4-unorm");
            case TwoD::SDL::TextureFormat::ASTC_5x5_UNORM:
                return Node("astc-5x5-unorm");
            case TwoD::SDL::TextureFormat::ASTC_6x5_UNORM:
                return Node("astc-6x5-unorm");
            case TwoD::SDL::TextureFormat::ASTC_6x6_UNORM:
                return Node("astc-6x6-unorm");
            case TwoD::SDL::TextureFormat::ASTC_8x5_UNORM:
                return Node("astc-8x5-unorm");
            case TwoD::SDL::TextureFormat::ASTC_8x6_UNORM:
                return Node("astc-8x6-unorm");
            case TwoD::SDL::TextureFormat::ASTC_8x8_UNORM:
                return Node("astc-8x8-unorm");
            case TwoD::SDL::TextureFormat::ASTC_10x5_UNORM:
                return Node("astc-10x5-unorm");
            case TwoD::SDL::TextureFormat::ASTC_10x6_UNORM:
                return Node("astc-10x6-unorm");
            case TwoD::SDL::TextureFormat::ASTC_10x8_UNORM:
                return Node("astc-10x8-unorm");
            case TwoD::SDL::TextureFormat::ASTC_10x10_UNORM:
                return Node("astc-10x10-unorm");
            case TwoD::SDL::TextureFormat::ASTC_12x10_UNORM:
                return Node("astc-12x10-unorm");
            case TwoD::SDL::TextureFormat::ASTC_12x12_UNORM:
                return Node("astc-12x12-unorm");
            case TwoD::SDL::TextureFormat::ASTC_4x4_UNORM_SRGB:
                return Node("astc-4x4-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_5x4_UNORM_SRGB:
                return Node("astc-5x4-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_5x5_UNORM_SRGB:
                return Node("astc-5x5-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_6x5_UNORM_SRGB:
                return Node("astc-6x5-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_6x6_UNORM_SRGB:
                return Node("astc-6x6-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_8x5_UNORM_SRGB:
                return Node("astc-8x5-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_8x6_UNORM_SRGB:
                return Node("astc-8x6-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_8x8_UNORM_SRGB:
                return Node("astc-8x8-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_10x5_UNORM_SRGB:
                return Node("astc-10x5-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_10x6_UNORM_SRGB:
                return Node("astc-10x6-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_10x8_UNORM_SRGB:
                return Node("astc-10x8-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_10x10_UNORM_SRGB:
                return Node("astc-10x10-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_12x10_UNORM_SRGB:
                return Node("astc-12x10-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_12x12_UNORM_SRGB:
                return Node("astc-12x12-unorm-srgb");
            case TwoD::SDL::TextureFormat::ASTC_4x4_FLOAT:
                return Node("astc-4x4-float");
            case TwoD::SDL::TextureFormat::ASTC_5x4_FLOAT:
                return Node("astc-5x4-float");
            case TwoD::SDL::TextureFormat::ASTC_5x5_FLOAT:
                return Node("astc-5x5-float");
            case TwoD::SDL::TextureFormat::ASTC_6x5_FLOAT:
                return Node("astc-6x5-float");
            case TwoD::SDL::TextureFormat::ASTC_6x6_FLOAT:
                return Node("astc-6x6-float");
            case TwoD::SDL::TextureFormat::ASTC_8x5_FLOAT:
                return Node("astc-8x5-float");
            case TwoD::SDL::TextureFormat::ASTC_8x6_FLOAT:
                return Node("astc-8x6-float");
            case TwoD::SDL::TextureFormat::ASTC_8x8_FLOAT:
                return Node("astc-8x8-float");
            case TwoD::SDL::TextureFormat::ASTC_10x5_FLOAT:
                return Node("astc-10x5-float");
            case TwoD::SDL::TextureFormat::ASTC_10x6_FLOAT:
                return Node("astc-10x6-float");
            case TwoD::SDL::TextureFormat::ASTC_10x8_FLOAT:
                return Node("astc-10x8-float");
            case TwoD::SDL::TextureFormat::ASTC_10x10_FLOAT:
                return Node("astc-10x10-float");
            case TwoD::SDL::TextureFormat::ASTC_12x10_FLOAT:
                return Node("astc-12x10-float");
            case TwoD::SDL::TextureFormat::ASTC_12x12_FLOAT:
                return Node("astc-12x12-float");
            }
        }
        static bool decode(const Node& node, TwoD::SDL::TextureFormat& rhs)
        {
            auto str = node.as<std::string>();
            
            if (str == "a8-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::A8_UNORM;
                return true;
            }
            if (str == "r8-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::R8_UNORM;
                return true;
            }
            if (str == "r8g8-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8_UNORM;
                return true;
            }
            if (str == "r8g8b8a8-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8B8A8_UNORM;
                return true;
            }
            if (str == "r16-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::R16_UNORM;
                return true;
            }
            if (str == "r16b16-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16_UNORM;
                return true;
            }
            if (str == "r16g16b16a16-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16B16A16_UNORM;
                return true;
            }
            if (str == "r10g10b10a1-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::R10G10B10A2_UNORM;
                return true;
            }
            if (str == "b5g6r5-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::B5G6R5_UNORM;
                return true;
            }
            if (str == "b5g5r5a1-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::B5G5R5A1_UNORM;
                return true;
            }
            if (str == "b4g4r4a4-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::B4G4R4A4_UNORM;
                return true;
            }
            if (str == "b8g8r8a8-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::B8G8R8A8_UNORM;
                return true;
            }
            if (str == "bc1-rgba-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::BC1_RGBA_UNORM;
                return true;
            }
            if (str == "bc2-rgba-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::BC2_RGBA_UNORM;
                return true;
            }
            if (str == "bc3-rgba-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::BC3_RGBA_UNORM;
                return true;
            }
            if (str == "bc4-r-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::BC4_R_UNORM;
                return true;
            }
            if (str == "bc5-rg-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::BC5_RG_UNORM;
                return true;
            }
            if (str == "bc7-rgba-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::BC7_RGBA_UNORM;
                return true;
            }
            if (str == "bc6h-rgb-float")
            {
                rhs = TwoD::SDL::TextureFormat::BC6H_RGB_FLOAT;
                return true;
            }
            if (str == "bc6h-rgb-ufloat")
            {
                rhs = TwoD::SDL::TextureFormat::BC6H_RGB_UFLOAT;
                return true;
            }
            if (str == "r8-snorm")
            {
                rhs = TwoD::SDL::TextureFormat::R8_SNORM;
                return true;
            }
            if (str == "r8g8-snorm")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8_SNORM;
                return true;
            }
            if (str == "r8g8b8a8-snorm")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8B8A8_SNORM;
                return true;
            }
            if (str == "r16-snorm")
            {
                rhs = TwoD::SDL::TextureFormat::R16_SNORM;
                return true;
            }
            if (str == "r16g16-snorm")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16_SNORM;
                return true;
            }
            if (str == "r16g16b16a16-snorm")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16B16A16_SNORM;
                return true;
            }
            if (str == "r16-float")
            {
                rhs = TwoD::SDL::TextureFormat::R16_FLOAT;
                return true;
            }
            if (str == "r16g16-float")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16_FLOAT;
                return true;
            }
            if (str == "r16g16b16a16-float")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16B16A16_FLOAT;
                return true;
            }
            if (str == "r32-float")
            {
                rhs = TwoD::SDL::TextureFormat::R32_FLOAT;
                return true;
            }
            if (str == "r32g32-float")
            {
                rhs = TwoD::SDL::TextureFormat::R32G32_FLOAT;
                return true;
            }
            if (str == "r32g32b32a32-float")
            {
                rhs = TwoD::SDL::TextureFormat::R32G32B32A32_FLOAT;
                return true;
            }
            if (str == "r11g11b10-ufloat")
            {
                rhs = TwoD::SDL::TextureFormat::R11G11B10_UFLOAT;
                return true;
            }
            if (str == "r8-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R8_UINT;
                return true;
            }
            if (str == "r8g8-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8_UINT;
                return true;
            }
            if (str == "r8g8b8a8-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8B8A8_UINT;
                return true;
            }
            if (str == "r16-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R16_UINT;
                return true;
            }
            if (str == "r16g16-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16_UINT;
                return true;
            }
            if (str == "r16g16b16a16-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16B16A16_UINT;
                return true;
            }
            if (str == "r32-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R32_UINT;
                return true;
            }
            if (str == "r32g32-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R32G32_UINT;
                return true;
            }
            if (str == "r32g32b32a32-uint")
            {
                rhs = TwoD::SDL::TextureFormat::R32G32B32A32_UINT;
                return true;
            }
            if (str == "r8-int")
            {
                rhs = TwoD::SDL::TextureFormat::R8_INT;
                return true;
            }
            if (str == "r8g8-int")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8_INT;
                return true;
            }
            if (str == "r8g8b8a8-int")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8B8A8_INT;
                return true;
            }
            if (str == "r16-int")
            {
                rhs = TwoD::SDL::TextureFormat::R16_INT;
                return true;
            }
            if (str == "r16g16-int")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16_INT;
                return true;
            }
            if (str == "r16g16b16a16-int")
            {
                rhs = TwoD::SDL::TextureFormat::R16G16B16A16_INT;
                return true;
            }
            if (str == "r32-int")
            {
                rhs = TwoD::SDL::TextureFormat::R32_INT;
                return true;
            }
            if (str == "r32g32-int")
            {
                rhs = TwoD::SDL::TextureFormat::R32G32_INT;
                return true;
            }
            if (str == "r32g32b32a32-int")
            {
                rhs = TwoD::SDL::TextureFormat::R32G32B32A32_INT;
                return true;
            }
            if (str == "r8g8b8a8-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::R8G8B8A8_UNORM_SRGB;
                return true;
            }
            if (str == "b8g8r8a8-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::B8G8R8A8_UNORM_SRGB;
                return true;
            }
            if (str == "bc1-rgba-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::BC1_RGBA_UNORM_SRGB;
                return true;
            }
            if (str == "bc2-rgba-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::BC2_RGBA_UNORM_SRGB;
                return true;
            }
            if (str == "bc3-rgba-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::BC3_RGBA_UNORM_SRGB;
                return true;
            }
            if (str == "bc7-rgba-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::BC7_RGBA_UNORM_SRGB;
                return true;
            }
            if (str == "d16-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::D16_UNORM;
                return true;
            }
            if (str == "d24-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::D24_UNORM;
                return true;
            }
            if (str == "d32-float")
            {
                rhs = TwoD::SDL::TextureFormat::D32_FLOAT;
                return true;
            }
            if (str == "d24-unorm-s8-uint")
            {
                rhs = TwoD::SDL::TextureFormat::D24_UNORM_S8_UINT;
                return true;
            }
            if (str == "d32-float-s8-uint")
            {
                rhs = TwoD::SDL::TextureFormat::D32_FLOAT_S8_UINT;
                return true;
            }
            if (str == "astc-4x4-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_4x4_UNORM;
                return true;
            }
            if (str == "astc-5x4-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_5x4_UNORM;
                return true;
            }
            if (str == "astc-5x5-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_5x5_UNORM;
                return true;
            }
            if (str == "astc-6x5-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_6x5_UNORM;
                return true;
            }
            if (str == "astc-6x6-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_6x6_UNORM;
                return true;
            }
            if (str == "astc-8x5-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x5_UNORM;
                return true;
            }
            if (str == "astc-8x6-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x6_UNORM;
                return true;
            }
            if (str == "astc-8x8-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x8_UNORM;
                return true;
            }
            if (str == "astc-10x5-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x5_UNORM;
                return true;
            }
            if (str == "astc-10x6-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x6_UNORM;
                return true;
            }
            if (str == "astc-10x8-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x8_UNORM;
                return true;
            }
            if (str == "astc-10x10-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x10_UNORM;
                return true;
            }
            if (str == "astc-12x10-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_12x10_UNORM;
                return true;
            }
            if (str == "astc-12x12-unorm")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_12x12_UNORM;
                return true;
            }
            if (str == "astc-4x4-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_4x4_UNORM_SRGB;
                return true;
            }
            if (str == "astc-5x4-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_5x4_UNORM_SRGB;
                return true;
            }
            if (str == "astc-5x5-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_5x5_UNORM_SRGB;
                return true;
            }
            if (str == "astc-6x5-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_6x5_UNORM_SRGB;
                return true;
            }
            if (str == "astc-6x6-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_6x6_UNORM_SRGB;
                return true;
            }
            if (str == "astc-8x5-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x5_UNORM_SRGB;
                return true;
            }
            if (str == "astc-8x6-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x6_UNORM_SRGB;
                return true;
            }
            if (str == "astc-8x8-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x8_UNORM_SRGB;
                return true;
            }
            if (str == "astc-10x5-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x5_UNORM_SRGB;
                return true;
            }
            if (str == "astc-10x6-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x6_UNORM_SRGB;
                return true;
            }
            if (str == "astc-10x8-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x8_UNORM_SRGB;
                return true;
            }
            if (str == "astc-10x10-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x10_UNORM_SRGB;
                return true;
            }
            if (str == "astc-12x10-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_12x10_UNORM_SRGB;
                return true;
            }
            if (str == "astc-12x12-unorm-srgb")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_12x12_UNORM_SRGB;
                return true;
            }
            if (str == "astc-4x4-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_4x4_FLOAT;
                return true;
            }
            if (str == "astc-5x4-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_5x4_FLOAT;
                return true;
            }
            if (str == "astc-5x5-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_5x5_FLOAT;
                return true;
            }
            if (str == "astc-6x5-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_6x5_FLOAT;
                return true;
            }
            if (str == "astc-6x6-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_6x6_FLOAT;
                return true;
            }
            if (str == "astc-8x5-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x5_FLOAT;
                return true;
            }
            if (str == "astc-8x6-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x6_FLOAT;
                return true;
            }
            if (str == "astc-8x8-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_8x8_FLOAT;
                return true;
            }
            if (str == "astc-10x5-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x5_FLOAT;
                return true;
            }
            if (str == "astc-10x6-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x6_FLOAT;
                return true;
            }
            if (str == "astc-10x8-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x8_FLOAT;
                return true;
            }
            if (str == "astc-10x10-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_10x10_FLOAT;
                return true;
            }
            if (str == "astc-12x10-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_12x10_FLOAT;
                return true;
            }
            if (str == "astc-12x12-float")
            {
                rhs = TwoD::SDL::TextureFormat::ASTC_12x12_FLOAT;
                return true;
            }
            return false;
        }
    };
}