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
		struct internal_load_data
		{
			glm::fvec2 position = { 0.0f, 0.0f };
			float rotation = 0;
			glm::fvec2 scale = { 1.0f, 1.0f };
		};
		static void CreateLoadData(internal_load_data* loadData, const YAML::Node& node);
		void Load(const void* data) override;
		void Destroy() override;
#ifdef TD_IMGUI
		void Debug() override;
#endif

		void SetParent(EntityHandle parent);
		EntityHandle GetParent() const;
		void AddChild(EntityHandle child);
		const std::vector<EntityHandle>& GetChildren() const;

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

		/**
		* Note that this function only sets the local matrix and updates its own
		* world matrix and its children. It does not extract the position, scale and rotation
		* data out of the given matrix applying it to it self.
		*/
		void SetLocalMatrix(const glm::fmat3x3& local);
		
		const glm::fmat3x3& GetWorldMatrix() const { return m_worldMatrix; }
		const glm::fmat3x3& GetLocalMatrix() const { return m_localMatrix; }
		const glm::fmat3x3& GetInverseWorldMatrix() const { return m_inverseWorldMatrix; }

	protected:
		void CalculateMatrices();
		void UpdateParentAndChildren();

	protected:
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

		EntityHandle m_parent;
		std::vector<EntityHandle> m_children;
	};
}