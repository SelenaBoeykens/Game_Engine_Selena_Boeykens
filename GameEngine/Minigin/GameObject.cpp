#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject() : m_Transform{}, m_pParent{nullptr}
{
}

dae::GameObject::~GameObject()
{
	//cleaning up the components
	for (BaseComponent* comp : m_pComponents)
	{
		delete comp;
	}
}



void dae::GameObject::Update(double deltaTime)
{
	UpdateComponents(deltaTime);
	UpdateChildren();
	for (int i = 0; i < m_pChildren.size(); i++)
	{
		if (m_pChildren[i]->GetNeedsKilling() == true)
		{
			RemoveChild(i);
		}
	}
}

void dae::GameObject::UpdateComponents(double deltaTime)
{
	if (m_Messages.size() >= 0)
	{
	for (Message mes : m_Messages)
	{
		for (BaseComponent* comp : m_pComponents)
		{
			if (mes.Sender != comp)
			{
				comp->ReceiveMessage(mes);
			}
		}
	}
	m_Messages.clear();

	}
	for (BaseComponent* comp : m_pComponents)
	{
		comp->Update(deltaTime);
	}
}

void dae::GameObject::UpdateChildren()
{
	for (auto& child : m_pChildren)
	{
		child->SetPosition(m_Transform.GetPosition().x + child->m_Offset.x, m_Transform.GetPosition().x + child->m_Offset.y);
	}
}

void dae::GameObject::AddComponent(ComponentType type)
{
	if (type == ComponentType::render)
	{
		m_pComponents.push_back(new RenderComponent());

	}
}

void dae::GameObject::AddComponent(BaseComponent* comp)
{
	comp->SetOwner(this);
	m_pComponents.push_back(comp);
}

void dae::GameObject::RemoveComponent(BaseComponent* comp)
{
	m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), comp));
	delete(comp);
}

void dae::GameObject::Render() const
{
	for (BaseComponent* comp : m_pComponents)
	{
		if (comp->GetType() == ComponentType::render)
		{
			static_cast<RenderComponent*>(comp)->Render();
		}
	}
}



void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

glm::vec3 dae::GameObject::GetPosition() const
{
	return (glm::vec3(m_Transform.GetPosition().x, m_Transform.GetPosition().y,0.0f));
}

void dae::GameObject::SetParent(GameObject* parent)
{
	m_pParent = parent;
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

size_t dae::GameObject::GetChildCount() const
{
	return m_pChildren.size();
}

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	return m_pChildren[index];
}

void dae::GameObject::RemoveChild(int index)
{
	m_pChildren[index]->SetKill();
	m_pChildren.erase(m_pChildren.begin() + (index-1));
}

void dae::GameObject::AddChild(GameObject* go)
{
	m_pChildren.push_back(go);
}

void dae::GameObject::BecomeChild(GameObject* owner)
{
	m_Offset = m_Transform.GetPosition() - owner->GetPosition();
	m_IsChild = true;
}

void dae::GameObject::StopChilding()
{
	m_IsChild = false;
}

void dae::GameObject::SetKill()
{
	m_NeedsKilling = true;
}

bool dae::GameObject::GetNeedsKilling() const
{
	return m_NeedsKilling;
}

void dae::GameObject::AddMessage(Message mes)
{
	m_Messages.push_back(mes);
}
