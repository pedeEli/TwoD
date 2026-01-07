#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ECS.hpp"

namespace TwoD
{
	class Transform : public Component
	{
		using Component::Component;
	public:
		void Load(const YAML::Node& node) override;

		void SetParent(const Ref<Entity>& parent);
		Ref<Entity> GetParent() const;
		const std::vector<Ref<Entity>>& GetChildren() const;

		void SetPosition(float x, float y);
		void SetPosition(glm::fvec2 pos);
		void Translate(float dx, float dy);
		void Translate(glm::fvec2 dpos);
		const glm::fvec2& GetPosition() const { return m_position; }

		void SetScale(glm::fvec2 scale);
		void SetScale(float sx, float sy);
		void Scale(glm::fvec2 dscale);
		void Scale(float dsx, float dsy);
		const glm::fvec2& GetScale() const { return m_scale; }

		void SetRotation(float r);
		void Rotate(float dr);
		float GetRotation() const { return m_rotation; }
		
		const glm::fmat3x3& GetWorldMatrix() const { return m_worldMatrix; }
		const glm::fmat3x3& GetLocalMatrix() const { return m_localMatrix; }
		const glm::fmat3x3& GetInverseWorldMatrix() const { return m_inverseWorldMatrix; }

	private:
		void CalculateMatrices();

	private:
		glm::fvec2 m_position = { 0.0f, 0.0f };
		float m_rotation = 0;
		glm::fvec2 m_scale = { 1.0f, 1.0f };

		glm::fmat3x3 m_localMatrix = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		glm::fmat3x3 m_worldMatrix = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		glm::fmat3x3 m_inverseWorldMatrix = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};

		Ref<Entity> m_parent;
		std::vector<Ref<Entity>> m_children;
	};
}