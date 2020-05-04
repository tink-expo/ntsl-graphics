#pragma once

#include <vector>
#include <Snowflake.hpp>
#include <Snowman.hpp>

/*
 * Animation class supports continuous animation by changing RenderObject's transform with elasped time in seconds
 */

class SnowflakeAnimation
{
private:
    std::vector<Snowflake*> _snowflakes;
    void dropMove(Snowflake* snowflake, float deltaTime, int idx);
    Engine::Camera *_camera;
    int stackedCount = 0;
    const int MAX_STACKED_COUNT = 7;
    bool shouldStack(const glm::vec3 &positionDelta, float bodyRadius);
public:
    SnowflakeAnimation();
    ~SnowflakeAnimation();
    void AddSnowflake(Snowflake* snowflake);
    void Animate(float deltaTime, Snowman *snowman);
    void SetCamera(Engine::Camera *camera) { _camera = camera; }
};