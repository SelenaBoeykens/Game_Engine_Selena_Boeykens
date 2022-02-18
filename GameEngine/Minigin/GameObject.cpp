#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

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
}

void dae::GameObject::UpdateComponents(double deltaTime)
{
	for (BaseComponent* comp : m_pComponents)
	{
		comp->Update(deltaTime);
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
	m_pComponents.push_back(comp);
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
	/*const auto pos = m_Transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);*/
}



void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
