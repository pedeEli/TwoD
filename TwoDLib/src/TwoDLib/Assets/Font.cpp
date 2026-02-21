#include "tdpch.hpp"
#include "Font.hpp"

#undef INFINITE
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "MSDFData.hpp"

#include "SpriteAtlas.hpp"
#include "TwoD/Core/App.hpp"

namespace TwoD
{
	Font::Font() = default;
	Font::~Font() = default;

	void Font::Init(const std::filesystem::path& path, const Window& window)
	{
		auto filePath = path.parent_path() / file;

		auto* ft = msdfgen::initializeFreetype();
		auto* font = msdfgen::loadFont(ft, filePath.string().c_str());
	
		m_data = std::make_unique<MSDFData>();
		m_data->fontGeometry = msdf_atlas::FontGeometry(&m_data->glyphs);
		m_data->fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
		
		const double maxCornerAngle = 3.0;
		for (auto& glyph : m_data->glyphs)
		{
			glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
		}

		msdf_atlas::TightAtlasPacker packer;
		packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
		packer.setMinimumScale(24.0);
		packer.setPixelRange(2.0);
		packer.setMiterLimit(1.0);
		packer.setScale(40.0);
		packer.setPixelRange(4.0);
		packer.pack(m_data->glyphs.data(), static_cast<int>(m_data->glyphs.size()));
		packer.getDimensions(m_atlasSize.x, m_atlasSize.y);

		msdf_atlas::ImmediateAtlasGenerator<
			float,
			3,
			msdf_atlas::msdfGenerator,
			msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3>
		> generator(m_atlasSize.x, m_atlasSize.y);

		msdf_atlas::GeneratorAttributes attributes;
		generator.setAttributes(attributes);
		generator.setThreadCount(4);
		generator.generate(m_data->glyphs.data(), static_cast<int>(m_data->glyphs.size()));

		auto atlas = static_cast<msdfgen::BitmapConstRef<uint8_t, 3>>(generator.atlasStorage());

		SDL::SamplerInfo samplerInfo{
			.minFilter = SDL::Filter::LINEAR,
			.magFilter = SDL::Filter::LINEAR,
			.mipmapMode = SDL::SamplerMipmapMode::NEAREST,
			.addressModeU = SDL::SamplerAddressMode::CLAMP_TO_EDGE,
			.addressModeV = SDL::SamplerAddressMode::CLAMP_TO_EDGE,
			.addressModeW = SDL::SamplerAddressMode::CLAMP_TO_EDGE
		};
		m_sampler = window.CreateSampler(samplerInfo);

		SDL::TextureInfo textureInfo{
			.type = SDL::TextureType::TWO_D,
			.format = SDL::TextureFormat::R8G8B8A8_UNORM,
			.usage = SDL::TextureUsageFlags::SAMPLER,
			.width = static_cast<uint32_t>(atlas.width),
			.height = static_cast<uint32_t>(atlas.height),
			.layerCountOrDepth = 1,
			.numLevels = 1,
			.sampleCount = SDL::SampleCount::ONE
		};
		m_texture = window.CreateTexture(textureInfo);

		SDL::TransferBufferInfo transferInfo{
			.usage = SDL::TransferBufferUsage::UPLOAD,
			.size = static_cast<uint32_t>(atlas.width * atlas.height) * 4
		};
		auto transferBuffer = window.CreateTransferBuffer(transferInfo);
		auto* transferData = transferBuffer.Map<uint8_t>(false);
		size_t src = 0;
		size_t dst = 0;
		size_t max = atlas.width * atlas.height * 3;

		while (src < max)
		{
			transferData[dst++] = 255;
			transferData[dst++] = atlas.pixels[src++];
			transferData[dst++] = atlas.pixels[src++];
			transferData[dst++] = atlas.pixels[src++];
		}

		auto commandBuffer = window.AcquireCommandBuffer();
		auto copyPass = commandBuffer.BeginCopyPass();

		SDL::TextureTransferInfo source{
			.transferBuffer = &transferBuffer,
			.offset = 0
		};
		SDL::TextureRegion destination{
			.texture = &m_texture,
			.w = static_cast<uint32_t>(atlas.width),
			.h = static_cast<uint32_t>(atlas.height),
			.d = 1
		};
		copyPass.UploadToTexture(source, destination, false);
		copyPass.End();
		transferBuffer.Release();

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);

		binding.texture = &m_texture;
		binding.sampler = &m_sampler;
		commandBuffer.Submit().Release();
	}

	void Font::Destroy()
	{
		m_sampler.Release();
		m_texture.Release();
	}

	void Font::Bind(SDL::RenderPass* renderPass)
	{
		renderPass->BindFragmentSamplers(1, { { &m_texture, &m_sampler } });
	}
}