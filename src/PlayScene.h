#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Target.h"
#include "ship.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	void changeLabel();
	void angleVelKnown();
	void distanceVelKnown();
	void reset();
	
private:
	// IMGUI Function
	void GUI_Function() ;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	//Plane* m_pPlaneSprite;
	//Player* m_pPlayer;
	Target* m_pLoot;
	Ship* m_pStormtrooper;
	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	
	
	
	
	Label* m_pInstructionsLabel;
	//mass, position, velocity, acceleration
	Label* m_pMass;
	Label* m_pRise;
	Label* m_pVel;
	Label* m_pAng;
	Label* m_pHeight;
	Label* m_pRun;
	int fl = 510.0f;


	// Physics Variables
	bool m_isGravityEnabled = false;
	bool* m_isGravityEnabledPtr = &m_isGravityEnabled;
	

	float m_PPM = 5.0f; // pixels per meter - scale
	float m_friction = 0.0f;

	bool m_distanceGiv = false;
	bool* m_distanceGiven = &m_distanceGiv;
	int m_distance = 485;
	float m_height = 0.0f;
	
	int rampWidthMeter = 4;
	int rampHeightMeter = 3;
	bool m_velocityGiv = false;
	bool* m_velocityGiven = &m_velocityGiv;
	


	

	


	bool m_angleGiv = false;
	bool* m_angleGiven = &m_angleGiv;
	


	bool m_timeGiv = false;
	bool* m_timeGiven = &m_timeGiv;
	float m_totalTime=0.0f;
};

#endif /* defined (__PLAY_SCENE__) */