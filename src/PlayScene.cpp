#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include <string>

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("bg", Config::SCREEN_WIDTH/2, Config::SCREEN_HEIGHT/2, 0, 255, true);

	
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			/*if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}*/
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		/*if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}*/
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}
void PlayScene::reset() {
	m_isFrictionSurface = false;
	//m_pStormtrooper->getTransform()->position = glm::vec2(585.0f, fl);
	m_pLoot->m_gravity = 9.8f;
	m_PPM = 1.0f;
	rampHeightMeter = 150;
	rampWidthMeter = 150;
	m_pLoot->m_mass = 12.5f;
	m_pLoot->m_PPM = 1.0f;
	m_pLoot->m_Atime = 0.016667f;
	m_pLoot->m_angle = 0.0f;
	m_pLoot->m_velocity = 0.0f;
	m_pLoot->m_velocityX = 0.0f;
	m_pLoot->m_velocityY = 0.0f;
	m_pLoot->m_acceleration = glm::vec2(0.0f, 0.0f);
	m_pLoot->throwSpeed = glm::vec2(0.0f, 0.0f);
	m_pLoot->buttonPressed = false;
	m_pLoot->isGravityEnabled = false;
	
}
void PlayScene::start()
{
	//load background from texture.
	TextureManager::Instance()->load("../Assets/textures/bg.png", "bg");
	// Set GUI Title
	m_guiTitle = "Play Scene";
	//// Ball Sprite
	m_pLoot = new Target();
	addChild(m_pLoot);

	

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 450.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 450.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);
	const SDL_Color white = { 255, 255, 255, 255 };
	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Menu", "Consolas",15,white);
	
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 490.0f);

	addChild(m_pInstructionsLabel);
	
	m_pMass = new Label("Mass: 12.8Kg", "Consolas", 20, white);
	m_pMass->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.9f, 10.0f);
	addChild(m_pMass);
	
	m_pVel = new Label("Velocity: 95m/s", "Consolas", 20, white);
	m_pVel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, 30.0f);
	addChild(m_pVel);
	
	m_pAng = new Label("Angle: 45degree", "Consolas", 20, white);
	m_pAng->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, 50.0f);
	addChild(m_pAng);
	m_pRise = new Label("Ramp Rise: 3m", "Consolas", 20, white);
	m_pRise->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, 70.0f);
	addChild(m_pRise);
	m_pRun = new Label("Ramp Run: 4m", "Consolas", 20, white);
	m_pRun->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 90.0f);
	addChild(m_pRun);
	m_pForce = new Label("Force: 50N", "Consolas", 20, white);
	m_pForce->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f,110.0f);
	addChild(m_pForce);
	m_pFriction = new Label("Coefficient of friction: 0.42", "Consolas", 20, white);
	m_pFriction->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.75f, 130.0f);
	addChild(m_pFriction);
	m_pAcce = new Label("Acceleration: 3m/s^2", "Consolas", 20, white);
	m_pAcce->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, 150.0f);
	addChild(m_pAcce);
	reset();

}
void PlayScene::changeLabel() {
	std::string text = "Velocity: " + std::to_string(m_pLoot->m_velocity) + " m/s";
	m_pVel->setText(text);
	text = "Mass: " + std::to_string(m_pLoot->m_mass) + " Kg";//Mass: 12.8Kg
	m_pAng->setText(text);
	text = "Angle: " + std::to_string(m_pLoot->m_angle) + " degree";//Angle: 45degree
	m_pAng->setText(text);
	//text = "distance: " + std::to_string(m_pStormtrooper->getTransform()->position.x- m_pBall->getTransform()->position.x) + " m";//Angle: 45degree
	text = "Rise: " + std::to_string(rampHeightMeter) + " m";
	m_pRise->setText(text);
	text = "Force: " + std::to_string(m_pLoot->m_pF) + " N";
	m_pForce->setText(text);
	text = "Run: " + std::to_string(rampWidthMeter) + " m";
	m_pRun->setText(text);
	text = "Coefficient of friction: " + std::to_string(m_friction) ;//Angle: 45degree
	m_pFriction->setText(text);
	std::cout << "labl" <<  m_pLoot->m_accelerationVal << std::endl;
	text = "Acceleration: " + std::to_string(m_pLoot->m_accelerationVal);//Angle: 45degree
	m_pAcce->setText(text);
}
void PlayScene::Calculation() {
	m_pLoot->m_angle = Util::Rad2Deg * atan((rampHeightMeter * m_PPM) / (rampWidthMeter * m_PPM));
	if (!m_isFrictionSurface)
	{
		//force at time 0
		m_pLoot->m_pF = m_pLoot->m_mass * m_pLoot->m_gravity * sin(Util::Deg2Rad * m_pLoot->m_angle);
	}
	else
	{
		m_pLoot->m_pF = m_pLoot->m_mass * m_pLoot->m_gravity * m_friction;
	}
	m_pLoot->m_accelerationVal = m_pLoot->m_pF/m_pLoot->m_mass;
	//intitial velocity components//v_i=√2gh
	m_pLoot->m_velocity = sqrt(2* m_pLoot->m_gravity*rampHeightMeter);
	m_pLoot->m_velocityX = (m_pLoot->m_velocity * m_PPM) * cos(m_pLoot->m_angle * Util::Deg2Rad);
	m_pLoot->m_velocityY = (m_pLoot->m_velocity * m_PPM) * sin(m_pLoot->m_angle * Util::Deg2Rad);
	// initial velocity vector
	//std::cout << "st" << m_pLoot->m_velocityX << "," << m_pLoot->m_velocityY << std::endl;
	glm::vec2 velocity_vector = glm::vec2(m_pLoot->m_velocityX, m_pLoot->m_velocityY);
	m_pLoot->m_acceleration = glm::vec2(m_pLoot->m_accelerationVal * cos(m_pLoot->m_angle * Util::Deg2Rad) * m_PPM,
		m_pLoot->m_accelerationVal  * sin(m_pLoot->m_angle * Util::Deg2Rad) * m_PPM);
	std::cout << "a" << m_pLoot->m_acceleration.x << "," << m_pLoot->m_acceleration.y << ", " << m_pLoot->m_accelerationVal << std::endl;
	m_pLoot->throwSpeed = velocity_vector;
	
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Simulation Control", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	
	ImGui::Separator();
	bool buttonPressed = ImGui::Button("Start Simulation");
	if (buttonPressed)
	{
		
		m_pLoot->doThrow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset All"))
	{
		reset();
	}
	ImGui::Separator();
	
	
	ImGui::Separator();
	static int xPlayerPos = 150;
	
	if (ImGui::SliderFloat("Pixels Per Meter", &m_PPM, 0.1f, 30.0f, "%.1f"))
	{
		m_pLoot->m_PPM = m_PPM;
	}
	if (ImGui::SliderFloat("Mass of crate (Kg)", &m_pLoot->m_mass, 0.1f, 30.0f, "%.1f"));
	if (ImGui::SliderFloat("force of friction", &m_friction, 0.03f, 1.0f, "%.3f"));
	
	
	
	glm::vec2 point = glm::vec2(150, 400);
	if (ImGui::SliderInt("Ramp Width(m)", &rampWidthMeter, 0, (Config::SCREEN_WIDTH - 200)/m_PPM));
	
	if (ImGui::SliderInt("Ramp Height(m)", &rampHeightMeter, 0, (Config::SCREEN_HEIGHT - 250)/m_PPM));
	

	Util::DrawLine(point, glm::vec2(point.x, point.y - rampHeightMeter * m_PPM), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	Util::DrawLine(point, glm::vec2(point.x + rampWidthMeter * m_PPM, point.y), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	Util::DrawLine(glm::vec2(point.x + rampWidthMeter * m_PPM, point.y), glm::vec2(point.x, point.y - rampHeightMeter * m_PPM), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	m_pLoot->getTransform()->position = glm::vec2(point.x,point.y - rampHeightMeter * m_PPM -20);//glm::vec2(xPlayerPos + 40.0f, fl);
	Calculation();
	changeLabel();
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
