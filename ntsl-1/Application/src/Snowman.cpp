#include <Snowman.hpp>
#include <cmath>
#include <cstdio>
using namespace std;
Snowman::Snowman()
{
}

Snowman::~Snowman()
{
}

/* set default body position & other parts position & orientation wrt body */
void Snowman::SetDefaultPositions()
{
	defaultPosition = glm::vec3(0.0f, -2.0f, 0.0f);
	body->SetPosition(defaultPosition);

	head->SetPosition(glm::vec3(0.0f, headRadius + bodyRadius - 0.3f, 0.0f));

	wing_left->SetPosition(glm::vec3(-bodyRadius * sqrt(3) / 2, bodyRadius / 2, 0.0f));
	defaultLeftWingOrientation = glm::rotate(glm::mat4(1.0f), glm::radians(-120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wing_left->SetOrientation(defaultLeftWingOrientation);

	wing_right->SetPosition(glm::vec3(bodyRadius * sqrt(3) / 2, bodyRadius / 2, 0.0f));
	defaultRightWingOrientation = glm::rotate(glm::mat4(1.0f), glm::radians(-60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wing_right->SetOrientation(defaultRightWingOrientation);

	cone->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cone->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

/* flap wing twice during flying time */
float Snowman::getWingAddDegree(float flyingTime, float totalFlyingTime)
{
	float x = (flyingTime / totalFlyingTime) * 480;
	float y = -abs(x-240) + 240;
	return y <= 120 ? y : 240 - y;
}

void Snowman::Animate(float deltaTime)
{
	glm::vec3 curPosition = body->GetPosition();
	glm::vec3 nextPosition = curPosition;
	
	nextPosition.x += deltaTime * 1.0f * moveDirection;

	const float acceleration = 24.0f;
	const float initialVelocity = 6.0f;
	const float totalFlyingTime = 2 * initialVelocity / acceleration;

	static glm::vec3 flyDeltaPositions[4] = {
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f)
	};
	static int flyDeltaIndex = 0;

	if (flying) {
		flyingTime += deltaTime;
		// parabolic motion defined by acceleration & initialVelocity
		nextPosition.y = defaultPosition.y - acceleration / 2 * flyingTime * flyingTime + initialVelocity * flyingTime;
		nextPosition += flyDeltaPositions[flyDeltaIndex] * deltaTime / totalFlyingTime;
		if (flyingTime > totalFlyingTime) { // stop flying
			flying = false;
			nextPosition.y = defaultPosition.y;
			wing_left->SetOrientation(defaultLeftWingOrientation);
			wing_right->SetOrientation(defaultRightWingOrientation);
			flyDeltaIndex = flyDeltaIndex == 3 ? 0 : flyDeltaIndex + 1;
		}

		float wingAddDegree = getWingAddDegree(flyingTime, totalFlyingTime);
		wing_left->SetOrientation(glm::rotate(defaultLeftWingOrientation, glm::radians(-wingAddDegree), glm::vec3(0.0f, 0.0f, 1.0f)));
		wing_right->SetOrientation(glm::rotate(defaultRightWingOrientation, glm::radians(wingAddDegree), glm::vec3(0.0f, 0.0f, 1.0f)));
	} else {
		flyingTime = 0.0f;
	}

	body->SetPosition(nextPosition);
}