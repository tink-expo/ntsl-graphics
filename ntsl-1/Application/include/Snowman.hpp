#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <RenderObject.hpp>
#include <PickableObject.hpp>

class Snowman
{
private:
	glm::vec3 defaultPosition;
	glm::mat4 defaultLeftWingOrientation;
	glm::mat4 defaultRightWingOrientation;
	float flyingTime;
	float getWingAddDegree(float flyingTime, float totalFlyingTime);
public:
    Snowman();
    ~Snowman();
	PickableObject *body;
	PickableObject *head;
	Engine::RenderObject *wing_left;
	Engine::RenderObject *wing_right;
	PickableObject *cone;
	void SetDefaultPositions();
	void Animate(float deltaTime);
	void SetConeOrientation(glm::mat4 coneOrientation) { cone->SetOrientation(coneOrientation); }
	glm::mat4 GetConeOrientation() { return cone->GetOrientation(); }
	glm::vec3 GetBodyPosition() { return body->GetPosition(); }

	const float headRadius = 1.0f;
	const float bodyRadius = 1.3f;

	bool hasHead = false;
	bool hasCone = false;
	int numWings = 0;
	
	bool flying = false;
	int moveDirection = 0; // -1, 0, 1
};