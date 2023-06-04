/**
 * @file NetworkClient.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of NetworkClient class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <vector>
#include <string>
#include "MatchStats.hpp"
#include "Achievement.hpp"
#include <cpr/cpr.h>

/**
 * @brief Provide functionality for netwotk interaction on client
 */
class NetworkClient
{
public:

	NetworkClient() = delete;

	enum class Status
	{
		SUCCESS,
		FAILURE
	};

	static int32_t getId()
	{
		return m_userId;
	}

	static void init(const std::string& filename);

	static Status auth(const std::string& login, const std::string& pass);

	static bool sendMatchStatsToServer(const MatchStats& stats);

	static const std::vector<Achievement>& getAchievementsFromServer()
	{
		return m_lastReceivedAchievements;
	}

	static const cpr::Error& getLastError()
	{
		return m_lastError;
	}

private:
	static std::string m_authorizationURL;

	static std::string m_achievementURL;

	static cpr::Error m_lastError;

	static int32_t m_userId;

	static std::vector<Achievement> m_lastReceivedAchievements;
};