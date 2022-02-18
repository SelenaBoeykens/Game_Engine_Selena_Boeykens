#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Components.h"

namespace dae
{
	class Texture2D;
	class GameObject
	{
	public:
		void Update(double deltaTime);
		void UpdateComponents(double deltaTime);
		void AddComponent(ComponentType type);
		void AddComponent(BaseComponent* comp);
		void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;
		std::vector<BaseComponent*> m_pComponents;
	};
}
