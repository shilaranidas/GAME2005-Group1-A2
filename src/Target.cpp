#include "Target.h"
#include "TextureManager.h"
#include "Util.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/ball.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(230.0f, 510.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move()
{
	//float deltaTime = 1.0f / 60.0f;
	//glm::vec2 gravity = glm::vec2(0, 9.8);
	getRigidBody()->acceleration = glm::vec2(0, 9.8) * m_PPM;
	//getRigidBody()->acceleration = gravity*deltaTime;
	//getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * deltaTime;
	getRigidBody()->velocity += getRigidBody()->acceleration * m_time;
	if (!isGravityEnabled)  getRigidBody()->velocity.y = 0;
	getTransform()->position += getRigidBody()->velocity * m_time;


	
	
	/*getRigidBody()->acceleration =( glm::vec2(0, m_gravity))* *m_PPM;
	
	getRigidBody()->velocity += getRigidBody()->acceleration * m_time;
	if (!isGravityEnabled)  getRigidBody()->velocity.y = 0;
	getTransform()->position += getRigidBody()->velocity * m_time;*/


	std::cout << "speed" << getRigidBody()->velocity.x<<","<< getRigidBody()->velocity.y<<std::endl;

//	//
//	// physics equation
//	if(buttonPressed && isGravityEnabled)
//		m_acceleration = glm::vec2(0.0f, m_gravity) * m_PPM;
//	else if (!isGravityEnabled && buttonPressed) {
//		getRigidBody()->velocity.y = 0; 
//		m_acceleration=glm::vec2(0.0f, 0.0f);
//	}
//	 
//		
//	m_finalPosition = getTransform()->position +
//		(getRigidBody()->velocity * m_time) +
//		((m_acceleration * 0.5f) * (m_Atime * m_Atime));
//
//	m_Atime += m_time;
//	this->getTransform()->position = m_finalPosition;
//	//m_pShip->setPosition(m_finalPosition);
}

void Target::m_checkBounds()
{
	if (getTransform()->position.x > 800)
	{
		getTransform()->position=(glm::vec2(800.0f, getTransform()->position.y));
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position =(glm::vec2(0.0f, getTransform()->position.y));
	}

	if (getTransform()->position.y > 600)
	{
		getTransform()->position=(glm::vec2(getTransform()->position.x, 600.0f));
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position=(glm::vec2(getTransform()->position.x, 0.0f));
	}
}

void Target::m_reset()
{
}
void Target::doThrow() {
	getTransform()->position = throwPosition;
	getRigidBody()->velocity = throwSpeed;
	//buttonPressed=true;
	//if(m_angleGiv && m_velocityGiv)
	//{
	//	// velocity components
	//	m_velocityX = (m_velocity * m_PPM) * cos(m_angle * Util::Deg2Rad);
	//	m_velocityY = (m_velocity * m_PPM) * -sin(m_angle * Util::Deg2Rad);
	//	// final velocity vector
	//	glm::vec2 velocity_vector = glm::vec2(m_velocityX, m_velocityY);

	//	getRigidBody()->velocity = velocity_vector;
	//}
	//
}
