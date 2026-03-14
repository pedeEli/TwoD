#include "tdpch.hpp"
#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void Transform::SetLocalMatrix(const glm::fmat3x3& local)
	{
		position = local[2];

		auto a = m_localMatrix[0][0];
		auto b = m_localMatrix[1][0];
		auto c = m_localMatrix[0][1];
		auto d = m_localMatrix[1][1];

		scale = { std::sqrt(a * a + c * c), std::sqrt(b * b + d * d) };
		rotation = std::atan2(c / scale.x, a / scale.x);

		if (a * d - b * c < 0)
		{
			scale.y *= -1;
		}

		m_localMatrix = local;
	}

	void Transform::UpdateMatrix(const std::vector<PropagationCallback>& callbacks, glm::fmat3x3* parent)
	{
		CalcMatrix(parent);

		for (auto& callback : callbacks)
		{
			callback(*this);
		}

		for (auto child : m_children)
		{
			child->GetComponent<Transform>().UpdateMatrix(callbacks, &m_worldMatrix);
		}
	}
	void Transform::CalcMatrix(glm::fmat3x3* parent)
	{
		float c = glm::cos(rotation);
		float s = glm::sin(rotation);

		m_localMatrix[0][0] = c * scale.x;
		m_localMatrix[0][1] = s * scale.x;
		m_localMatrix[1][0] = -s * scale.y;
		m_localMatrix[1][1] = c * scale.y;
		m_localMatrix[2][0] = position.x;
		m_localMatrix[2][1] = position.y;

		if (parent)
		{
			m_worldMatrix = *parent * m_localMatrix;
		}
		else
		{
			m_worldMatrix = m_localMatrix;
		}
		m_inverseWorldMatrix = glm::inverse(m_worldMatrix);
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
}