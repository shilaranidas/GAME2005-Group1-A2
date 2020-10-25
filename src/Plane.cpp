#include "Plane.h"
#include "TextureManager.h"

Plane::Plane()
{

	TextureManager::Instance()->load("../Assets/textures/protoman-idle-0.png", "enemy");

	const auto size = TextureManager::Instance()->getTextureSize("enemy");


	/*TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));
	
	*/


	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);

	getTransform()->position = glm::vec2(750.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);


	
}

Plane::~Plane()
= default;

void Plane::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	TextureManager::Instance()->draw("enemy", x, y, 0, 255, true);
		//"spritesheet", getAnimation("plane"),
		//x, y, 0.5f, 0, 255, true);
}

void Plane::update()
{
}

void Plane::clean()
{
}

void Plane::m_buildAnimations()
{
	Animation planeAnimation = Animation();

	planeAnimation.name = "protoman";
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("protoman-idle-0"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("protoman-idle-1"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("protoman-idle-2"));

	setAnimation(planeAnimation);
}