

#include <iostream>
#include <fstream>
#include <string>
#include <restc-cpp/restc-cpp.h>
#include "restc-cpp/SerializeJson.h"
#include "restc-cpp/RequestBuilder.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/adapted.hpp>



using namespace std;
using namespace restc_cpp;

struct MatchStats {
    int playerId;
    int score;
    int kills;
    int steps;
    int sculcks;
    int waves;
    int duration;
    int specialAttackKills;
};

struct Achievement {
    int achievementId;
    string title;
    string description;
    string reward;
};

struct UserData {
    int id;
};

BOOST_FUSION_ADAPT_STRUCT(
    MatchStats,
    (int, playerId)
    (int, score)
    (int, kills)
    (int, steps)
    (int, sculcks)
    (int, waves)
    (int, duration)
    (int, specialAttackKills)
)


BOOST_FUSION_ADAPT_STRUCT(
    Achievement,
    (int, achievementId)
    (string, title)
    (string, description)
    (string, reward)
)

bool sendMatchStatsToServer(const MatchStats& matchStats) {
    try {
        // Create a REST client
        auto rest_client = RestClient::Create();

        // Create a JSON payload using Boost.PropertyTree
        boost::property_tree::ptree payload;
        payload.put("playerId", matchStats.playerId);
        payload.put("score", matchStats.score);
        payload.put("kills", matchStats.kills);
        payload.put("steps", matchStats.steps);
        payload.put("sculcks", matchStats.sculcks);
        payload.put("waves", matchStats.waves);
        payload.put("duration", matchStats.duration);
        payload.put("specialAttackKills", matchStats.specialAttackKills);

        // Convert the payload to a JSON string
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, payload);
        std::string jsonPayload = oss.str();

        // Send the JSON payload to the server
        rest_client->ProcessWithPromise([&](Context& ctx) {
            auto reply = RequestBuilder(ctx)
                .Post("https://example.com/api/data/")
                .Execute();
        }).get();

        
        std::cout << "Match stats sent successfully!" << std::endl;
        return true;
    } catch (const std::exception& ex) {
        std::cout << "Exception occurred while sending match stats: " << ex.what() << std::endl;
        return false;
    }
}


std::vector<Achievement> getAchievementsFromServer() {
    try {
        // Create a REST client
        auto rest_client = RestClient::Create();

        // Send a GET request to retrieve achievements from the server
        rest_client->ProcessWithPromise([&](Context& ctx) {
            auto reply = RequestBuilder(ctx)
                .Get("https://example.com/api/achievements/")
                .Execute();
        }).get();

        

        // Parse the JSON response
        boost::property_tree::ptree response;
        boost::property_tree::json_parser::read_json("output_file.json", response);

        // Extract the achievements from the response
        std::vector<Achievement> achievements;
        for (const auto& achievementNode : response) {
            Achievement achievement;
            achievement.achievementId = achievementNode.second.get<int>("achievementId");
            achievement.title = achievementNode.second.get<std::string>("title");
            achievement.description = achievementNode.second.get<std::string>("description");
            achievement.reward = achievementNode.second.get<std::string>("reward");
            achievements.push_back(achievement);
        }

        return achievements;
    } catch (const std::exception& ex) {
        std::cout << "Exception occurred while getting achievements: " << ex.what() << std::endl;
        return {};
    }
}

int authorization(const std::string& login, const std::string& password, UserData& data) {
    try {
        // Create a REST client
        auto rest_client = RestClient::Create();

        // Create the URL with query parameters
        std::string url = "https://example.com/back/auth?username=" + login + "&password=" + password;

        // Send a GET request to perform authorization
        rest_client->ProcessWithPromise([&](Context& ctx) {
            auto reply = RequestBuilder(ctx)
                .Get(url)
                .Execute();
        }).get();

        
       

        // Parse the JSON response
        boost::property_tree::ptree response;
        boost::property_tree::json_parser::read_json("output1_file.json", response);

        // Extract the user ID from the response
        data.id = response.get<int>("id");

        return 0; // Successful authorization
    } catch (const std::exception& ex) {
        std::cout << "Exception occurred during authorization: " << ex.what() << std::endl;
        return -1; // Error occurred
    }
}

int main() {
    std::string login, password;
    std::ifstream file("info.txt");
     if (file.is_open()) {
        
        file >> login >> password;

    }
    UserData data;

    int errorCode = authorization(login, password, data);

    if (errorCode == 0) {
        std::cout << "Authorization successful. User ID: " << data.id << std::endl;
        // Continue with further processing
    } else {
        std::cout << "Authorization failed. Error code: " << errorCode << std::endl;
        // Handle the error
    }

    MatchStats stats = {1, 2000, 100, 500, 0, 4, 1, 3};
    bool success = sendMatchStatsToServer(stats);
    if (success) {
        // Match stats sent successfully
    } else {
        // Failed to send match stats
    }

     std::vector<Achievement> achievements = getAchievementsFromServer();
    if (!achievements.empty()) {
        // Process the received achievements
        for (const auto& achievement : achievements) {
            std::cout << "Achievement ID: " << achievement.achievementId << std::endl;
            std::cout << "Title: " << achievement.title << std::endl;
            std::cout << "Description: " << achievement.description << std::endl;
            std::cout << "Reward: " << achievement.reward << std::endl;
            std::cout << std::endl;
        }
    } else {
        // Failed to get achievements
    }


    return 0;
}
    

    
    
