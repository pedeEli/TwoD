#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ECS.hpp"

TD_COMPONENT(
	(TD_NAME(Transform), TD_NAMESPACE(TwoD)),
	(
		TD_COMPONENT_FIELD(glm::fvec2, position, TD_INIT({ 0.0f, 0.0f })),
		TD_COMPONENT_FIELD(float, rotation, TD_INIT(0)),
		TD_COMPONENT_FIELD(glm::fvec2, scale, TD_INIT({ 1.0f, 1.0f }))
	)
)
	public:
		using PropagationCallback = std::function<void(Transform&)>;
	public:
		void Destroy() override;

		void UpdateMatrix(const std::vector<PropagationCallback>& callbacks, glm::fmat3x3* parent = nullptr);

		void SetParent(EntityHandle parent);
		EntityHandle GetParent() const;
		void AddChild(EntityHandle child);
		const std::vector<EntityHandle>& GetChildren() const;


		void SetLocalMatrix(const glm::fmat3x3& local);
		
		const glm::fmat3x3& GetWorldMatrix() const { return m_worldMatrix; }
		const glm::fmat3x3& GetLocalMatrix() const { return m_localMatrix; }
		const glm::fmat3x3& GetInverseWorldMatrix() const { return m_inverseWorldMatrix; }
	
	protected:
		void CalcMatrix(glm::fmat3x3* parent);

	protected:
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