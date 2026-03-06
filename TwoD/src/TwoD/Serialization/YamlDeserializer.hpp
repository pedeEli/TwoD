#include <yaml-cpp/yaml.h>

namespace TwoD
{
	struct Deserializer::Raw
	{
		const std::string name;
		YAML::Node node;
	};

	template<typename T>
	static bool Convert(const Deserializer::Raw& raw, T& value)
	{
		try
		{
			value = raw.node.as<T>();
			return true;
		}
		catch (const YAML::InvalidNode& error)
		{
			TD_CORE_ERROR("{}: {}", raw.name, error.msg);
			return false;
		}
		catch (const YAML::Exception& error)
		{
			TD_CORE_ERROR("{}:{}:{}: {}", raw.name, error.mark.line, error.mark.column, error.msg);
			return false;
		}
	}

	Deserializer::~Deserializer()= default;
	Deserializer::Deserializer(const Deserializer& other) noexcept = default;
	Deserializer::Deserializer(Deserializer&& other) noexcept = default;
	Deserializer& Deserializer::operator=(Deserializer&& other) noexcept = default;
	Deserializer& Deserializer::operator=(const Deserializer& other) noexcept = default;

	Deserializer::Deserializer(const std::string& name, const char* input)
	{
		try {
			m_raw = std::make_shared<Raw>(name, YAML::Load(input));
		}
		catch (const YAML::ParserException& error)
		{
			TD_CORE_ASSERT(false, std::format("{}:{}:{}: {}", name, error.mark.line, error.mark.column, error.msg));
		}
	}
	Deserializer::Deserializer(const std::string& name)
	{
		m_raw = std::make_shared<Raw>(name);
	}

	Deserializer Deserializer::operator[](const std::string_view key) const
	{
		auto d = Deserializer(m_raw->name);
		d.m_raw->node = m_raw->node[key];
		return d;
	}
	Deserializer Deserializer::operator[](const char* key) const
	{
		auto d = Deserializer(m_raw->name);
		d.m_raw->node = m_raw->node[key];
		return d;
	}
	Deserializer Deserializer::operator[](size_t index) const
	{
		auto d = Deserializer(m_raw->name);
		d.m_raw->node = m_raw->node[index];
		return d;
	}
	Deserializer::operator bool() const
	{
		return static_cast<bool>(m_raw->node);
	}

	bool Deserializer::IsNull() const
	{
		return m_raw->node.IsNull();
	}
	bool Deserializer::IsSequence() const
	{
		return m_raw->node.IsSequence();
	}
	size_t Deserializer::GetSize() const
	{
		return m_raw->node.size();
	}

	bool Deserializer::GetInt8(int8_t& value) const
	{
		return Convert<int8_t>(*m_raw, value);
	}
	bool Deserializer::GetInt16(int16_t& value) const
	{
		return Convert<int16_t>(*m_raw, value);
	}
	bool Deserializer::GetInt32(int32_t& value) const
	{
		return Convert<int32_t>(*m_raw, value);
	}
	bool Deserializer::GetInt64(int64_t& value) const
	{
		return Convert<int64_t>(*m_raw, value);
	}

	bool Deserializer::GetUInt8(uint8_t& value) const
	{
		return Convert<uint8_t>(*m_raw, value);
	}
	bool Deserializer::GetUInt16(uint16_t& value) const
	{
		return Convert<uint16_t>(*m_raw, value);
	}
	bool Deserializer::GetUInt32(uint32_t& value) const
	{
		return Convert<uint32_t>(*m_raw, value);
	}
	bool Deserializer::GetUInt64(uint64_t& value) const
	{
		return Convert<uint64_t>(*m_raw, value);
	}

	bool Deserializer::GetFloat(float& value) const
	{
		return Convert<float>(*m_raw, value);
	}
	bool Deserializer::GetDouble(double& value) const
	{
		return Convert<double>(*m_raw, value);
	}

	bool Deserializer::GetBool(bool& value) const
	{
		return Convert<bool>(*m_raw, value);
	}

	bool Deserializer::GetString(std::string& value) const
	{
		return Convert<std::string>(*m_raw, value);
	}
}