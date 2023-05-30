#include "NetworkClient.hpp"
#include <boost/json.hpp>
#include <fstream>
#include <cpr/cpr.h>

void NetworkClient::init(const std::string& filename)
{
	namespace json = boost::json;

	std::ifstream ifs(filename);
	json::object serverInfo = json::parse(ifs).as_object();

	m_authorizationURL = serverInfo.at("authorization").as_string();
	m_achievementURL = serverInfo.at("achievements").as_string();
}


NetworkClient::Status NetworkClient::auth(const std::string& login, const std::string& pass)
{
	namespace json = boost::json;
	cpr::Parameters parameters{ { "username", login },
		{ "password", pass } };
	cpr::Response response = cpr::Get(cpr::Url{ m_authorizationURL }, parameters);
	if (response.status_code == 401)
	{
		return Status::FAILURE;
	}
	else
	{
		json::object playerId = json::parse(response.text).as_object();
		m_userId = playerId.at("id").as_int64();
		return Status::SUCCESS;
	}
}

bool NetworkClient::sendMatchStatsToServer(const MatchStats& stats)
{
	namespace json = boost::json;

	json::object body;
	body["score"] = stats.score;
	body["kills"] = stats.kills;
	body["steps"] = stats.steps;
	body["sculcks"] = stats.sculcks;
	body["waves"] = stats.waves;
	body["duration"] = stats.duration;
	body["specialAttackKills"] = stats.specialAttackKills;

	cpr::Response response = cpr::Post(cpr::Url{ m_achievementURL },
		cpr::Parameters{ { "playerId", std::to_string(getId()) } },
		cpr::Body{ json::serialize(body) });

	if (response.status_code == 200)
	{
		m_lastReceivedAchievements.clear();
		json::array achievements = json::parse(response.text).as_object().at("achievements").as_array();
		for (const auto& i : achievements)
		{
			Achievement curAchievement;
			curAchievement.achievementId = i.as_object().at("id").as_int64();
			curAchievement.description = i.as_object().at("description").as_string();
			curAchievement.reward = i.as_object().at("reward").as_int64();
			m_lastReceivedAchievements.push_back(curAchievement);
		}
		return true;
	}
	else
	{
		return false;
	}

}

