#pragma once
#include "Transform.h"

enum class ComponentType
{
	render,
	fps
};

namespace dae
{
	class GameObject;
	struct Message;
	class BaseComponent
	{
	public:
		BaseComponent(ComponentType type, GameObject* owner); //moet ook gameobject meegeven wordennn!!
		virtual ~BaseComponent() = 0;
		virtual void Update(double deltaTime) = 0;
		void SetType(ComponentType type);
		ComponentType GetType() const;
		void SendsMessage(const Message& message);
		virtual void ReceiveMessage(const Message& message) =0;
		void SetOwner(GameObject* owner);
	protected:
		ComponentType m_Type;
		GameObject* m_pOwner = nullptr;
	};

	struct Message
	{
		BaseComponent* Sender = nullptr;
		//type
		std::string text;
	};


	class Font;
	class Texture2D;
	class RenderComponent final : public BaseComponent
	{
	public:
		void Update(double deltaTime) override;
		void Render() const;

		void SetText(const std::string& text);
		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		void ReceiveMessage(const Message& message);

		RenderComponent(GameObject* owner);
		RenderComponent(const std::string& text, const std::shared_ptr<Font>& font, GameObject* owner);
		~RenderComponent() override;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	private:
		bool m_NeedsUpdate;
		bool m_IsSetToText;
		std::string m_Text;
		Transform m_Transform;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_Texture = nullptr;
	};


	class FPSComponent : public BaseComponent
	{
	public:
		FPSComponent(RenderComponent* renderComponent, GameObject* owner);
		void Update(double deltaTime) override;
		void ReceiveMessage(const Message& message);
	private:
		RenderComponent* m_RenderComponent;
		int m_NrFrames;
		double m_TimePassed;
	};
}



