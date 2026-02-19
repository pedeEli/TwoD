#pragma once
#include "Defines.hpp"

namespace TwoD::SDL
{
    class CopyPass
    {
    public:
        CopyPass(const CommandBuffer* commandBuffer);
        ~CopyPass();
		CopyPass(const CopyPass& other) = delete;
		CopyPass(CopyPass&& other) = delete;
		CopyPass& operator=(const CopyPass& other) = delete;
		CopyPass& operator=(CopyPass&& other) = delete;

        void UploadToTexture(const TextureTransferInfo& source, const TextureRegion& destination, bool cycle) const;
        void UploadToBuffer(const TransferBufferLocation& source, const BufferRegion& destination, bool cycle) const;
    private:
        struct Raw;
        std::unique_ptr<Raw> m_raw;
    };
}
