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
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
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
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
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
		}
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
	m_isGravityEnabled = false;
	m_pStormtrooper->getTransform()->position = glm::vec2(585.0f, fl);
	m_pBall->m_gravity = 9.8f;
	m_PPM = 1.0f;
	m_pBall->m_PPM = 1.0f;
	m_pBall->m_Atime = 0.016667f;
	m_pBall->m_angle = 0.0f;
	m_pBall->m_velocity = 0.0f;
	m_pBall->m_velocityX = 0.0f;
	m_pBall->m_velocityY = 0.0f;
	m_pBall->m_acceleration = glm::vec2(0.0f, 0.0f);
	m_pBall->throwSpeed = glm::vec2(0.0f, 0.0f);
	m_pBall->buttonPressed = false;
	m_pBall->isGravityEnabled = false;
	m_angleGiv = false;
	m_velocityGiv = false;
	m_distanceGiv = false;
	m_timeGiv = false;
	//m_velocityGiven = false;
	//m_angleGiven = false;
	m_pBall->getTransform()->position = m_pPlayer->getTransform()->position + glm::vec2(20.0f, 0.0f);
}
void PlayScene::start()
{
	//load background from texture.
	TextureManager::Instance()->load("../Assets/textures/bg.png", "bg");
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	//// Ball Sprite
	m_pBall = new Target();
	addChild(m_pBall);

	//// Ball Sprite
	m_pStormtrooper = new Ship();
	addChild(m_pStormtrooper);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 570.0f);
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
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 570.0f);
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
	
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 590.0f);

	addChild(m_pInstructionsLabel);
	
	/*m_pMass = new Label("Mass: 2.2Kg", "Consolas", 20, white);
	m_pMass->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.9f, 10.0f);
	addChild(m_pMass);
	
	m_pVel = new Label("Velocity: 95m/s", "Consolas", 20, white);
	m_pVel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, 30.0f);
	addChild(m_pVel);
	
	m_pAng = new Label("Angle: 45degree", "Consolas", 20, white);
	m_pAng->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, 50.0f);
	addChild(m_pAng);
	m_pDist = new Label("total distance: 485m", "Consolas", 20, white);
	m_pDist->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.8f, 70.0f);
	addChild(m_pDist);
	m_pTime = new Label("total Time: 5sec", "Consolas", 20, white);
	m_pTime->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 90.0f);
	addChild(m_pTime);
	m_pHeight = new Label("Height: 50m", "Consolas", 20, white);
	m_pHeight->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f,110.0f);
	addChild(m_pHeight);*/
	reset();

}
void PlayScene::changeLabel() {
	std::string text = "Velocity: " + std::to_string(m_pBall->m_velocity) + " m/s";
	m_pVel->setText(text);
	text = "Angle: " + std::to_string(m_pBall->m_angle) + " degree";//Angle: 45degree
	m_pAng->setText(text);
	//text = "distance: " + std::to_string(m_pStormtrooper->getTransform()->position.x- m_pBall->getTransform()->position.x) + " m";//Angle: 45degree
	text = "distance: " + std::to_string(m_distance) + " m";
	m_pDist->setText(text);
	text = "height: " + std::to_string(m_height) + " m";
	m_pHeight->setText(text);
	text = "Time: " + std::to_string(m_totalTime) + " sec";//Angle: 45degree
	m_pTime->setText(text);
}
void PlayScene::distanceVelKnown() {

	m_pBall->m_angle = Util::Rad2Deg *0.5f *asin(m_pBall->m_gravity*m_distance*m_PPM/((m_pBall->m_velocity * m_PPM)* (m_pBall->m_velocity * m_PPM)));
	//m_pBall->throwPosition = glm::vec2(m_pPlayer->getTransform()->position.x + 20.0f, m_pPlayer->getTransform()->position.y);
	// velocity components
	m_pBall->m_velocityX = (m_pBall->m_velocity * m_PPM) * cos(m_pBall->m_angle * Util::Deg2Rad);
	m_pBall->m_velocityY = (m_pBall->m_velocity * m_PPM) * -sin(m_pBall->m_angle * Util::Deg2Rad);
	// final velocity vector
	glm::vec2 velocity_vector = glm::vec2(m_pBall->m_velocityX, m_pBall->m_velocityY);

	m_pBall->throwSpeed = velocity_vector;
	//m_distance = (m_pBall->m_velocity * m_PPM) * (m_pBall->m_velocity * m_PPM) * sin(2 * m_pBall->m_angle * Util::Deg2Rad) / m_pBall->m_gravity;
	m_height= (m_pBall->m_velocity * m_PPM) * (m_pBall->m_velocity * m_PPM) * sin( m_pBall->m_angle * Util::Deg2Rad)* sin(m_pBall->m_angle * Util::Deg2Rad) /(2* m_pBall->m_gravity);
	m_totalTime = (m_pBall->m_velocity * m_PPM) * sin(m_pBall->m_angle * Util::Deg2Rad) / m_pBall->m_gravity;
	//set stormtrooper
	//m_pStormtrooper->getTransform()->position = glm::vec2(m_distance * m_PPM, fl);
}
void PlayScene::angleVelKnown() {
	//m_pBall->throwPosition = glm::vec2(m_pPlayer->getTransform()->position.x + 20.0f, m_pPlayer->getTransform()->position.y);
	// velocity components
	m_pBall->m_velocityX = (m_pBall->m_velocity * m_PPM) * cos(m_pBall->m_angle * Util::Deg2Rad);
	m_pBall->m_velocityY = (m_pBall->m_velocity * m_PPM) * -sin(m_pBall->m_angle * Util::Deg2Rad);
	// final velocity vector
	glm::vec2 velocity_vector = glm::vec2(m_pBall->m_velocityX, m_pBall->m_velocityY);

	m_pBall->throwSpeed = velocity_vector;
	m_distance = (m_pBall->m_velocity * m_PPM) * (m_pBall->m_velocity * m_PPM) * sin(2 * m_pBall->m_angle * Util::Deg2Rad) / m_pBall->m_gravity;
	m_height = (m_pBall->m_velocity * m_PPM) * (m_pBall->m_velocity * m_PPM) * sin(m_pBall->m_angle * Util::Deg2Rad) * sin(m_pBall->m_angle * Util::Deg2Rad) / (2 * m_pBall->m_gravity);
	m_totalTime = (m_pBall->m_velocity * m_PPM) * sin(m_pBall->m_angle * Util::Deg2Rad) / m_pBall->m_gravity;
	//set stormtrooper
	m_pStormtrooper->getTransform()->position = glm::vec2(m_distance * m_PPM+ m_pPlayer->getTransform()->position.x, fl) ;
}
void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Simulation Control", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	
	ImGui::Separator();
	bool buttonPressed = ImGui::Button("Throw");
	if (buttonPressed)
	{
		m_pBall->getTransform()->position = glm::vec2(m_pPlayer->getTransform()->position.x + 40.0f, fl);
		m_pBall->throwPosition = glm::vec2(m_pPlayer->getTransform()->position.x + 40.0f, fl);
		m_pBall->doThrow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset All"))
	{
		reset();
	}
	ImGui::Separator();
	
	//ImGui::Checkbox("Gravity", m_isGravityEnabledPtr);
	////if ((*m_isGravityEnabledPtr))
	//	//std::cout << "T" << std::endl;
	//	m_pBall->isGravityEnabled = *m_isGravityEnabledPtr;
	////else
	//	//std::cout << "F" << std::endl;
	ImGui::Separator();
	static int xPlayerPos = 150;
	if (ImGui::SliderInt("Player x position", &xPlayerPos, 0, Config::SCREEN_WIDTH))
	{
		m_pPlayer->getTransform()->position = glm::vec2(xPlayerPos, fl);
		m_pBall->getTransform()->position= glm::vec2(xPlayerPos + 40.0f, fl);
		m_pBall->throwPosition = glm::vec2(xPlayerPos + 40.0f, fl);
		
	}
	static int rampWidth=150;
	glm::vec2 point = glm::vec2(150, 400);
	if (ImGui::SliderInt("Ramp Width", &rampWidth, 0, Config::SCREEN_WIDTH - 200));
	static int rampHeight = 150;
	if (ImGui::SliderInt("Ramp Height", &rampHeight, 0, Config::SCREEN_HEIGHT - 250));
	
	Util::DrawLine(point, glm::vec2(point.x, point.y - rampHeight), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	Util::DrawLine(point, glm::vec2(point.x + rampWidth, point.y), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	Util::DrawLine(glm::vec2(point.x + rampWidth, point.y), glm::vec2(point.x, point.y - rampHeight), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	//if (ImGui::SliderFloat("Pixels Per Meter", &m_PPM, 0.1f, 30.0f, "%.1f"))
	//{
	//	m_pBall->m_PPM = m_PPM;
	//}
	//ImGui::Separator();
	// 
	//if (ImGui::Checkbox("Kicking angle", m_angleGiven))
	//{
	//	m_pBall->m_angleGiv = m_angleGiven;
	//}
	//ImGui::SameLine();
	//if (ImGui::SliderFloat(" degree", &m_pBall->m_angle, 0.0f, 90.0f, "%.1f"))
	//{
	//	if (m_angleGiven && m_velocityGiven)
	//	{
	//		//// velocity components
	//		//m_pBall->m_velocityX = (m_pBall->m_velocity * m_PPM) * cos(m_pBall->m_angle * Util::Deg2Rad);
	//		//m_pBall->m_velocityY = (m_pBall->m_velocity * m_PPM) * -sin(m_pBall->m_angle * Util::Deg2Rad);
	//		//// final velocity vector
	//		//glm::vec2 velocity_vector = glm::vec2(m_pBall->m_velocityX, m_pBall->m_velocityY);

	//		//m_pBall->throwSpeed = velocity_vector;
	//		angleVelKnown();
	//		changeLabel();
	//	}
	//}
	//if (ImGui::Checkbox("velocity", m_velocityGiven))
	//{
	//	m_pBall->m_velocityGiv = m_velocityGiven;
	//}
	//ImGui::SameLine();
	//if (ImGui::SliderFloat("m/s", &m_pBall->m_velocity, 0.0f, 200.0f, "%.1f"))
	//{
	//	if(m_angleGiven && m_velocityGiven)
	//		{
	//			
	//			angleVelKnown();
	//			changeLabel();
	//		}

	//	//m_pBall->throwSpeed = glm::vec2(50.0f, -50.0f);
	//}
	//if (ImGui::Checkbox("Stormtrooper Distance", m_distanceGiven))
	//{
	//}
	//ImGui::SameLine();
	//if (ImGui::SliderInt("m", &m_distance, 1, 1400))
	//{
	//	//distance in meter need to transfer to pixel
	//	if (m_distanceGiven)
	//	{
	//		m_pStormtrooper->getTransform()->position = glm::vec2(m_distance * m_PPM + m_pPlayer->getTransform()->position.x, fl);
	//		if (m_distanceGiven && m_velocityGiven)
	//		{
	//			distanceVelKnown();
	//			changeLabel();
	//		}
	//	}
	//}

	//if (ImGui::Checkbox("Total time", m_timeGiven))
	//{
	//}
	//ImGui::SameLine();
	//if (ImGui::SliderFloat("sec", &m_totalTime, 0.0f, 20.0f, "%.1f"))
	//{

	//}
	//static float throwSpeed[2] = { 0,0 };
	//static float xThrowSpeed = 0.0f;
	/*if (ImGui::InputFloat("Throw Speed x", &xThrowSpeed)) {
		m_pBall->throwSpeed.x = xThrowSpeed;
	}*/	
	//static float yThrowSpeed = 0.0f;
	/*if (ImGui::InputFloat("Throw Speed y", &yThrowSpeed)) {
		m_pBall->throwSpeed.y = yThrowSpeed;
	}*/
	/*if(ImGui::SliderFloat2("Throw Speed", throwSpeed, -100.0f, 100.0f))
	{
		m_pBall->throwSpeed = glm::vec2(throwSpeed[0], throwSpeed[1]);
	}*/
	/*if (ImGui::InputFloat2("ThrowSpeed", velocity)) {
		m_pBall->throwSpeed = glm::vec2(velocity[0], velocity[1]);
	}*/
	//static float float3[3] = { 0.0f, 1.0f, 1.5f };
	/*if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}*/
	/*static float throwSpeed = 0.0f;
	if (ImGui::SliderFloat("Throw velocity in m/s", &throwSpeed, -100.0f, 100.0f))
	{
		if(*m_angleGiven && *m_velocityGiven)
			m_pBall->throwSpeed = glm::vec2( glm::sthrowSpeed[0], throwSpeed[1]);
	}*/
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
