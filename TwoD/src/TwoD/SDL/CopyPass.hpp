#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
    class CopyPass
    {
    public:
        CopyPass(const CommandBuffer* commandBuffer);
        ~CopyPass();

        void UploadToTexture(const TextureTransferInfo& source, const TextureRegion& destination, bool cycle) const;
        void UploadToBuffer(const TransferBufferLocation& source, const BufferRegion& destination, bool cycle) const;
    private:
        struct Raw;
        std::unique_ptr<Raw> m_raw;
    };
}
