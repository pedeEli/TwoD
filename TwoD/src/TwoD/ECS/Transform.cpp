#include "tdpch.hpp"
#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void Transform::CreateLoadData(internal_load_data* loadData, const Deserializer& deserializer)
	{
		if (deserializer["position"])
		{
			if (!deserializer["position"].As<glm::fvec2>(loadData->position))
			{
				TD_CORE_ASSERT(false, "failed to load transform");
			}
		}
		if (deserializer["rotation"])
		{
			if (!deserializer["rotation"].As<float>(loadData->rotation))
			{
				TD_CORE_ASSERT(false, "failed to load transform");
			}
		}
		if (deserializer["scale"])
		{
			if (!deserializer["scale"].As<glm::fvec2>(loadData->scale))
			{
				TD_CORE_ASSERT(false, "failed to load transform");
			}
		}
	}
	void Transform::Load(const void* data)
	{
		auto* loadData = static_cast<const internal_load_data*>(data);
		m_position = loadData->position;
		m_rotation = loadData->rotation;
		m_scale = loadData->scale;
	}

	void Transform::SetPosition(glm::fvec2 pos)
	{
		m_position = pos;
	}
	void Transform::SetPosition(float x, float y)
	{
		SetPosition({ x, y });
	}
	void Transform::Translate(glm::fvec2 dpos)
	{
		m_position += dpos;
	}
	void Transform::Translate(float dx, float dy)
	{
		Translate({ dx, dy });
	}

	void Transform::SetScale(glm::fvec2 scale)
	{
		m_scale = scale;
	}
	void Transform::SetScale(float sx, float sy)
	{
		SetScale({ sx, sy });
	}
	void Transform::Scale(glm::fvec2 dscale)
	{
		m_scale *= dscale;
	}
	void Transform::Scale(float dsx, float dsy)
	{
		Scale({ dsx, dsy });
	}

	void Transform::SetRotation(float r)
	{
		m_rotation = r;
	}
	void Transform::Rotate(float dr)
	{
		m_rotation += dr;
	}

	void Transform::SetLocalMatrix(const glm::fmat3x3& local)
	{
		m_position = local[2];

		auto a = m_localMatrix[0][0];
		auto b = m_localMatrix[1][0];
		auto c = m_localMatrix[0][1];
		auto d = m_localMatrix[1][1];

		m_scale = { std::sqrt(a * a + c * c), std::sqrt(b * b + d * d) };
		m_rotation = std::atan2(c / m_scale.x, a / m_scale.x);

		if (a * d - b * c < 0)
		{
			m_scale.y *= -1;
		}

		m_localMatrix = local;
	}

	void Transform::UpdateMatrix(const std::vector<PropagationCallback>& callbacks, glm::fmat3x3* parent)
	{
		float c = glm::cos(m_rotation);
		float s = glm::sin(m_rotation);

		m_localMatrix[0][0] = c * m_scale.x;
		m_localMatrix[0][1] = s * m_scale.x;
		m_localMatrix[1][0] = -s * m_scale.y;
		m_localMatrix[1][1] = c * m_scale.y;
		m_localMatrix[2][0] = m_position.x;
		m_localMatrix[2][1] = m_position.y;

		if (parent)
		{
			m_worldMatrix = *parent * m_localMatrix;
		}
		else
		{
			m_worldMatrix = m_localMatrix;
		}
		m_inverseWorldMatrix = glm::inverse(m_worldMatrix);

		for (auto& callback : callbacks)
		{
			callback(*this);
		}

		for (auto child : m_children)
		{
			child->GetTransform()->UpdateMatrix(callbacks, &m_worldMatrix);
		}
	}

	void Transform::Destroy()
	{
		SetParent(EntityHandle::None);
		for (auto child : m_children)
		{
			child->GetTransform()->m_parent = EntityHandle::None;
			child->Destroy();
		}
	}

	void Transform::SetParent(EntityHandle parent)
	{
		if (parent == m_parent)
		{
			return;
		}

		auto& entity = GetEntity();
		if (m_parent)
		{
			auto& children = m_parent->GetTransform()->m_children;
			children.erase(std::remove(children.begin(), children.end(), entity));
		}
		m_parent = parent;
		if (parent)
		{
			parent->GetTransform()->m_children.push_back(entity);
		}
	}
	EntityHandle Transform::GetParent() const
	{
		return m_parent;
	}
	void Transform::AddChild(EntityHandle child)
	{
		TD_CORE_ASSERT(std::all_of(m_children.begin(), m_children.end(), [child](auto a) {
				return child != a;
			}));
		child->GetTransform()->SetParent(GetEntity());
	}
	const std::vector<EntityHandle>& Transform::GetChildren() const
	{
		return m_children;
	}

#ifdef TD_CREATE_DEBUGGER
	void Transform::Debug()
	{
		Debuggable<glm::fvec2>::Draw(m_position, "position");
		Debuggable<float>::Draw(m_rotation, "rotation");
		Debuggable<glm::fvec2>::Draw(m_scale, "scale");
	}
#endif
}