#include "NetworkClient.hpp"
#include <boost/json.hpp>
#include <fstream>

void NetworkClient::init(const std::string& filename)
{
	namespace json = boost::json;

	std::ifstream ifs(filename);
	json::object serverInfo = json::parse(ifs).as_object();

	m_url = serverInfo.at("url").as_string();
}

NetworkClient::Status NetworkClient::auth(const std::string& login, const std::string& pass)
{
	// send login and pass to server ...
	// get response ...
	// if it's good
	// m_userId = ...
	return Status::SUCCESS;
	// else
	return Status::FAILURE;
}

bool NetworkClient::sendMatchStatsToServer(const MatchStats& stats)
{
	// send stats to server ...
	// receive achievements ...
	// if ok
	// m_lastReceivedAchevement = ...
	return true;
	// else
	return false;
}