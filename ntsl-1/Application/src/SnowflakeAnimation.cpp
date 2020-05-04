#include <SnowflakeAnimation.hpp>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;

SnowflakeAnimation::SnowflakeAnimation()
{
    _snowflakes = vector<Snowflake*>();
    _camera = NULL;
}

SnowflakeAnimation::~SnowflakeAnimation()
{
    _snowflakes.clear();
}

void SnowflakeAnimation::AddSnowflake(Snowflake* snowflake)
{
    _snowflakes.push_back(snowflake);
}

void SnowflakeAnimation::dropMove(Snowflake* snowflake, float deltaTime, int idx)
{
    glm::vec3 curPosition = snowflake->GetPosition();
    glm::vec3 nextPosition = curPosition + deltaTime * glm::vec3(0.0f, -0.5f -0.03f * idx, 0.0f);
    // falling speed depends on its index
    // since snowflake's initial x position is set randomly, 
    // the result looks like each snowflake's falling speed is decided randomly
    snowflake->SetPosition(nextPosition);
    glm::mat4 rotMat = snowflake->GetOrientation();
    float rotationRate = idx - (int)_snowflakes.size() / 2;
    rotationRate += rotationRate > 0 ? 0.5f : -0.5f;

    rotMat = glm::rotate(rotMat, glm::radians(10.0f * rotationRate * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
    snowflake->SetOrientation(rotMat);
}

bool SnowflakeAnimation::shouldStack(const glm::vec3 &positionDelta, float bodyRadius)
{
    float dx = positionDelta.x;
    float dy = positionDelta.y;
    float dR = sqrt(dx * dx + dy * dy) - bodyRadius;
    if (!(-0.05f < dR && dR < 0.05f)) return false;
    return bodyRadius / 2 < dy && dy <= bodyRadius;
}

void SnowflakeAnimation::Animate(float deltaTime, Snowman *snowman)
{
    glm::vec3 snowmanPosition = snowman->GetBodyPosition();
    for (int i=0; i<_snowflakes.size(); ++i) {
        Snowflake* snowflake = _snowflakes[i];
        if (!snowflake->stacked) {
            glm::vec3 positionDelta = snowflake->GetPosition() - snowman->GetBodyPosition();
            if (shouldStack(positionDelta, snowman->bodyRadius) && stackedCount < MAX_STACKED_COUNT) {
                // stack snowflake on snowman body
                snowflake->stacked = true;
                snowflake->SetOrientation(glm::rotate(snowflake->GetOrientation(), glm::radians(-70.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                snowflake->AddParent(snowman->body);
                snowflake->SetPosition(positionDelta);
                stackedCount += 1;

                if (stackedCount == MAX_STACKED_COUNT) {
                    snowman->hasHead = true;
                }
            } else if (snowflake->GetPosition().y < -4.5f) {
                // not stacked snowflake falls from top again when it has reached below the screen
                snowflake->SetPosition(snowflake->initialPosition);
            } else {
                // normal falling snowflake => perform dropMove
               dropMove(snowflake, deltaTime, i);
            }
        }
        if (!snowman->hasHead || !snowflake->stacked) {
            snowflake->Render(_camera);
        } 
    }
}