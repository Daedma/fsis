#pragma once

#include <vector>
#include <string>
#include "MatchStats.hpp"
#include <cpr/cpr.h>

class NetworkClient
{
public:
	struct Achievement
	{
		int32_t achievementId;
		std::string description;
		int32_t reward;
	};

	enum class Status
	{
		SUCCESS,
		FAILURE
	};

	int32_t getId() const
	{
		return m_userId;
	}

	void init(const std::string& filename);

	Status auth(const std::string& login, const std::string& pass);

	bool sendMatchStatsToServer(const MatchStats& stats);

	const std::vector<Achievement>& getAchievementsFromServer() const
	{
		return m_lastReceivedAchievements;
	}

	const cpr::Error& getLastError() const
	{
		return m_lastError;
	}

private:
	std::string m_authorizationURL;

	std::string m_achievementURL;

	cpr::Error m_lastError;

	int32_t m_userId;

	std::vector<Achievement> m_lastReceivedAchievements;
};