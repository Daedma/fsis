#include "NetworkClient.hpp"
#include <boost/json.hpp>
#include <fstream>
#include <cpr/cpr.h>


  
void NetworkClient::init(const std::string& filename)
{
	namespace json = boost::json;

	std::ifstream ifs(filename);
	json::object serverInfo = json::parse(ifs).as_object();

	m_url = serverInfo.at("back/auth").as_string();
}


NetworkClient::Status NetworkClient::auth(const std::string& login, const std::string& pass)
{
	namespace json = boost::json;
    cpr::Parameters parameters{{"username", login},
         {"password", pass}};
    auto response = cpr::Get(cpr::Url{ url+m_url }, parameters);
  
    
   
    if (response.status_code == 200)
    {
        
        json::object jsonObject = json::parse(response.text).as_object();
        m_userId = static_cast<int32_t>(jsonObject);
        return Status::SUCCESS;
    }
    else
        return Status::FAILURE;
    

   
}

bool NetworkClient::sendMatchStatsToServer(const MatchStats& stats)
{
    namespace json = boost::json;
    cpr::Parameters parameters{{"score", stats.score},
        {"kills", stats.kills},
        {"steps", stats.steps},
        {"sculcks", stats.sculcks},
        {"waves", stats.waves},
        {"duration", stats.duration},
        {"specialAttackKills", stats.specialAttackKills}
     };
	auto response = cpr::Post(cpr::Url{ url+m_url }, parameters);

    if (response.status_code == 200)
    {   
        auto req = json::parse(response.text).as_object();
        auto& achievments = req["achievments"].as_object();
        m_lastReceivedAchievements.achievementId = static_cast<int32_t>(achievments["id"]);
        m_lastReceivedAchievements.description = achievments["description"].as_string();
        m_lastReceivedAchievements.reward = static_cast<int32_t>(achievments["reward"]);
        return true;
    }
    else
        return false;

}
    
