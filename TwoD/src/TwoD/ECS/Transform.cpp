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
		CalculateMatrices();
	}

	void Transform::SetPosition(glm::fvec2 pos)
	{
		m_position = pos;
		CalculateMatrices();
	}
	void Transform::SetPosition(float x, float y)
	{
		SetPosition({ x, y });
	}
	void Transform::Translate(glm::fvec2 dpos)
	{
		m_position += dpos;
		CalculateMatrices();
	}
	void Transform::Translate(float dx, float dy)
	{
		Translate({ dx, dy });
	}

	void Transform::SetScale(glm::fvec2 scale)
	{
		m_scale = scale;
		CalculateMatrices();
	}
	void Transform::SetScale(float sx, float sy)
	{
		SetScale({ sx, sy });
	}
	void Transform::Scale(glm::fvec2 dscale)
	{
		m_scale *= dscale;
		CalculateMatrices();
	}
	void Transform::Scale(float dsx, float dsy)
	{
		Scale({ dsx, dsy });
	}

	void Transform::SetRotation(float r)
	{
		m_rotation = r;
		CalculateMatrices();
	}
	void Transform::Rotate(float dr)
	{
		m_rotation += dr;
		CalculateMatrices();
	}

	void Transform::SetLocalMatrix(const glm::fmat3x3& local)
	{
		m_localMatrix = local;
		UpdateParentAndChildren();
	}

	void Transform::CalculateMatrices()
	{
		float c = glm::cos(m_rotation);
		float s = glm::sin(m_rotation);

		m_localMatrix[0][0] =  c * m_scale.x;
		m_localMatrix[0][1] =  s * m_scale.x;
		m_localMatrix[1][0] = -s * m_scale.y;
		m_localMatrix[1][1] =  c * m_scale.y;
		m_localMatrix[2][0] =  m_position.x;
		m_localMatrix[2][1] =  m_position.y;

		UpdateParentAndChildren();
	}
	void Transform::UpdateParentAndChildren()
	{
		if (m_parent)
		{
			auto& parentMatrix = m_parent->GetTransform()->GetWorldMatrix();
			m_worldMatrix = parentMatrix * m_localMatrix;
		}
		else
		{
			m_worldMatrix = m_localMatrix;
		}
		m_inverseWorldMatrix = glm::inverse(m_worldMatrix);

		for (auto child : m_children)
		{
			child->GetTransform()->UpdateParentAndChildren();
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
		UpdateParentAndChildren();
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
		bool changed = false;
		changed |= Debuggable<glm::fvec2>::Draw(m_position, "position");
		changed |= Debuggable<float>::Draw(m_rotation, "rotation");
		changed |= Debuggable<glm::fvec2>::Draw(m_scale, "scale");
		if (changed)
		{
			CalculateMatrices();
		}
	}
#endif
}