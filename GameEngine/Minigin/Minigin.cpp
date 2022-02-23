#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include <chrono>
#include <SDL.h>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"

using namespace std;
using namespace std::chrono;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	//go->AddComponent(ComponentType::render);
	RenderComponent* renderComp = new RenderComponent();
	renderComp->SetTexture("background.jpg");
	go->AddComponent(renderComp);
	go->AddMessage({ nullptr,"ren,fancybw.png," });
	scene.Add(go);

	go = std::make_shared<GameObject>();
	RenderComponent* renderComp2 = new RenderComponent();
	renderComp2->SetTexture("logo.png");
	renderComp2->SetPosition(216, 180);
	go->AddComponent(renderComp2);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	go = std::make_shared<GameObject>();
	RenderComponent* renderCompFPS = new RenderComponent("0 FPS", font);
	renderCompFPS->SetPosition(0, 0);
	go->AddComponent(renderCompFPS);
	
	FPSComponent* fpsComp = new FPSComponent(renderCompFPS);
	go->AddComponent(fpsComp);
	scene.Add(go);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto lastTime = high_resolution_clock::now();
		double lag = 0;
		// todo: this update loop could use some work.
		bool doContinue = true;
		while (doContinue)
		{
			/*doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();*/
			while (doContinue)
			{
				auto currentTime = high_resolution_clock::now();
				const double deltaTime = duration_cast<duration<double>>(currentTime - lastTime).count();

				lastTime = currentTime;
				lag += deltaTime;

				doContinue = input.ProcessInput();

				while (lag >= MsPerFrame)
				{
					//SceneManager.FixedUpdate();
					lag -= MsPerFrame;
				}
				sceneManager.Update(deltaTime);
				renderer.Render();

				auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
				this_thread::sleep_for(sleepTime);
			}
		}
	}

	Cleanup();
}
