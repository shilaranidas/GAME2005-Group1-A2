#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"



class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	void doThrow();

	bool isGravityEnabled = false;
	bool m_velocityGiv = false;
	bool m_angleGiv = false;

	glm::vec2 throwSpeed = glm::vec2(0.0f, 0.0f);
	glm::vec2 throwPosition = glm::vec2(0.0f, 0.0f);
	float m_PPM = 1.0f; // pixels per meter - scale
	float m_angle = 45.0f; // the "kicking" angle
	float m_velocity =0.0f;
	float m_velocityX = 0.0f;
	float m_velocityY = 0.0f;
	float m_gravity = 9.8f;
	float m_time = 0.016667f; // approximately 16 ms (1/60)
	float m_Atime = 0.016667f; // accumulated time  (1/60)
	glm::vec2 m_acceleration = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_finalPosition = glm::vec2(0.0f, 0.0f);
	float m_mass = 10.0f;

	bool buttonPressed = false;

private:
	void m_move();
	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */