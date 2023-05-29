#include "NetworkClient.hpp"
#include <boost/json.hpp>
#include <fstream>
#include <cpr/cpr.h>


  
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
    cpr::Parameters parameters{{"username", login},
         {"password", pass}};
    auto response = cpr::Get(cpr::Url{ "http://localhost:8081/back/auth" }, parameters);
    if (response.status_code == 200)
    {
        
        m_userId = response.text;
        return Status::SUCCESS;
    }
    else
    {
        
        std::cout << "Ошибка авторизации. Код ошибки: " << response.status_code << std::endl;
        return Status::FAILURE;
    }

   
}

bool NetworkClient::sendMatchStatsToServer(const MatchStats& stats)
{
    cpr::Parameters parameters{{"score", stats.score},
        {"kills", stats.kills},
        {"steps", stats.steps},
        {"sculcks", stats.sculcks},
        {"waves", stats.waves},
        {"duration", stats.duration},
        {"specialAttackKills", stats.specialAttackKills}
     };
	auto response = cpr::Post(cpr::Url{ "http://localhost:8081/back/auth" }, parameters);

    if (response.status_code == 200)
    {
        
        m_lastReceivedAchievements = response.text;
        return true;
    }
    else
    {
        
        std::cout << "Ошибка авторизации. Код ошибки: " << response.status_code << std::endl;
        return false;
    }
}
    
