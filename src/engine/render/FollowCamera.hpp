/**
 * @file FollowCamera.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of FollowCamera class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "render/Camera.hpp"

class Actor;

/**
 * @brief A class represents camera, that follow to player
 */
class FollowCamera : public Camera
{
	Actor* m_target = nullptr;

public:
	void setTarget(Actor* target) { m_target = target; }

	Actor* getTarget() const { return m_target; }

	void resetTarget() { m_target = nullptr; }

protected:
	virtual void updatePosition() override;
};