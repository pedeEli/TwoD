#include "tdpch.hpp"
#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "TwoD/Core/App.hpp"

namespace TwoD
{
	void Transform::Load(const YAML::Node& node)
	{
		if (node["position"])
		{
			SetPosition(node["position"].as<glm::fvec3>());
		}
		if (node["rotation"])
		{
			SetRotation(node["rotation"].as<float>());
		}
		if (node["scale"])
		{
			SetScale(node["scale"].as<glm::fvec2>());
		}
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
		auto& ecs = App::Get<ECS>();
		if (m_parent != 0)
		{
			auto& parentMatrix = ecs.GetEntity(m_parent).GetComponent<Transform>().GetWorldMatrix();
			m_worldMatrix = parentMatrix * m_localMatrix;
		}
		else
		{
			m_worldMatrix = m_localMatrix;
		}
		m_inverseWorldMatrix = glm::inverse(m_worldMatrix);

		for (auto child : m_children)
		{
			ecs.GetEntity(child).GetComponent<Transform>().UpdateParentAndChildren();
		}
	}

	void Transform::SetParent(const Entity* parent)
	{
		if (parent != nullptr && parent->GetHandle() == m_parent)
		{
			return;
		}

		auto& ecs = App::Get<ECS>();
		auto& entity = GetEntity();
		if (m_parent != 0)
		{
			auto& children = ecs.GetEntity(m_parent).GetComponent<Transform>().m_children;
			children.erase(std::remove(children.begin(), children.end(), &entity));
		}
		m_parent = parent->GetHandle();
		if (parent)
		{
			parent->GetComponent<Transform>().m_children.push_back(&entity);
		}
		UpdateParentAndChildren();
	}
	const Entity* Transform::GetParent() const
	{
		return m_parent;
	}
	const std::vector<const Entity*>& Transform::GetChildren() const
	{
		return m_children;
	}
}