#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Components.h"

enum class ComponentType;
namespace dae
{
	class Texture2D;
	class BaseComponent;
	struct Message;
	class GameObject
	{
	public:
		void Update(double deltaTime);
		void UpdateComponents(double deltaTime);
		void AddComponent(ComponentType type);
		void AddComponent(BaseComponent* comp);
		void RemoveComponent(BaseComponent* comp);
		void Render() const;

		void SetPosition(float x, float y);

		void SetParent(GameObject* parent);
		GameObject* GetParent() const;
		size_t GetChildCount() const;
		GameObject* GetChildAt(int index) const;
		void RemoveChild(int index);
		void AddChild(GameObject* go);

		void SetKill();
		bool GetNeedsKilling() const;

		void AddMessage(Message mes);


		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		dae::Transform m_Transform;
		std::vector<dae::BaseComponent*> m_pComponents;
		std::vector<GameObject*> m_pChildren;
		GameObject* m_pParent;
		bool m_NeedsKilling = false;
		std::vector<Message> m_Messages;
	};
}

