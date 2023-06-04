/**
 * @file ClientApplication.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of ClientApplication class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "states/State.hpp"

/**
 * @brief A class represents client application
 */
class ClientApplication
{
public:
	ClientApplication();

	void run();

private:
	State* m_curState = nullptr;
};