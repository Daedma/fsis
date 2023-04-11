#pragma once

#include "render/Camera.hpp"

class Actor;

class FollowCamera: public Camera
{
	Actor* m_target = nullptr;

public:
	void setTarget(Actor* target) { m_target = target; }

	Actor* getTarget() const { return m_target; }

	void resetTarget() { m_target = nullptr; }

protected:
	virtual void updatePosition() override;
};