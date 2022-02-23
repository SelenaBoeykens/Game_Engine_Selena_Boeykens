#include "MiniginPCH.h"
#include "Components.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <regex>



dae::BaseComponent::BaseComponent(ComponentType type) : m_Type{ type }
{
}

dae::BaseComponent::~BaseComponent()
{
}

void dae::BaseComponent::SetType(ComponentType type)
{
	m_Type = type;
}

ComponentType dae::BaseComponent::GetType() const
{
	return m_Type;
}

void dae::BaseComponent::SendsMessage(Message message)
{
	if (m_pOwner != nullptr)
	{
	m_pOwner->AddMessage(message);
	}
}


void dae::BaseComponent::SetOwner(GameObject* owner)
{
	m_pOwner = owner;
}


dae::RenderComponent::RenderComponent()
	:BaseComponent{ ComponentType::render }, m_NeedsUpdate(false), m_Text(""), m_Texture(nullptr), m_IsSetToText(false)
{
}

dae::RenderComponent::RenderComponent(const std::string& text, const std::shared_ptr<Font>& font)
	: BaseComponent{ ComponentType::render }, m_NeedsUpdate(true), m_Text(text), m_Font(font), m_Texture(nullptr), m_IsSetToText(true)
{
}

dae::RenderComponent::~RenderComponent()
{
}

void dae::RenderComponent::Update(double deltaTime)
{
	deltaTime++;
	if (m_NeedsUpdate && m_IsSetToText)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_Texture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::RenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::RenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_IsSetToText = true;
	m_NeedsUpdate = true;
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_IsSetToText = false;
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetPosition(const float x, const float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::RenderComponent::ReceiveMessage(Message message)
{
	if (std::regex_match(message.text, std::regex("ren,.*")))
	{
		const std::string mes = "ren,kaas\n";
		std::cout << "rendering message received!\n";
		std::smatch match;
		std::regex reg{ "ren,(.*)," };
		if (std::regex_search(message.text.cbegin(), message.text.cend(), match, reg))
		{
			std::cout << match[1];
		}
		
		
	}
}

dae::FPSComponent::FPSComponent(RenderComponent* renderComponent) :BaseComponent(ComponentType::fps)
, m_RenderComponent{ renderComponent }
, m_TimePassed{}
, m_NrFrames{}
{
}

void dae::FPSComponent::Update(double deltaTime)
{
	m_NrFrames++;
	m_TimePassed += deltaTime;

	if (m_TimePassed >= 1)
	{
		m_RenderComponent->SetText(std::to_string(m_NrFrames) + " FPS");
		m_TimePassed -= 1;
		m_NrFrames = 0;
	}

}

void dae::FPSComponent::ReceiveMessage(Message message)
{
}
