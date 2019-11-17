//
// Created by Cody on 11/12/2019.
//

#include <common.hpp>
#include <iostream>
#include <sstream>
#include <map>
#include "SaveData.hpp"

constexpr char leaderboardFilename[] = "leaderboard.vape";
constexpr char savegameFilename[] = "savegame.vape";

inline bool fileExists(const std::string &fileName) {
    if (FILE *file = fopen(fileName.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

inline void parseLeaderboard(std::multiset<leaderboardEntry>* leaderboard) {
    std::string leaderboardFile = std::string(leaderboardFilename);
    if (!fileExists(leaderboardFile))
        return;

    FILE* leaderboard_file = fopen(leaderboardFile.c_str(), "r");
    int ch = getc(leaderboard_file);
    while(ch != EOF) {
        unsigned int points;
        char name[32];
        fscanf(leaderboard_file, "%s : %d\n", name, &points);

        leaderboard->insert({name, points});

        ch = getc(leaderboard_file);
    }
    if (feof(leaderboard_file))
        std::cout << "End of file reached." << std::endl;
    else
        std::cout << "Something went wrong." << std::endl;
    fclose(leaderboard_file);
}


inline void saveLeaderBoard(const std::multiset<leaderboardEntry>& leaderboard) {
    std::string leaderboardFile = std::string(leaderboardFilename);
    FILE* leaderboard_file = fopen(leaderboardFile.c_str(), "w");

    int n = 0;
    for (auto it = leaderboard.rbegin(); it != leaderboard.rend(); ++it) {
        if (n > 10) break;
        std::ostringstream oss;
        oss << " " << it->name << " : " + std::to_string(it->points) +"\n";
        fputs(oss.str().c_str(), leaderboard_file);
        ++n;
    }
    fclose(leaderboard_file);
}

void saveScore(unsigned int points, const std::string&name) {
    std::multiset<leaderboardEntry> leaderboard = {};
    parseLeaderboard(&leaderboard);
    leaderboard.insert({name, points});

    saveLeaderBoard(leaderboard);
}

std::multiset<leaderboardEntry> getLeaderboard() {
    std::multiset<leaderboardEntry> leaderboard = {};
    parseLeaderboard(&leaderboard);

    return leaderboard;
}

void saveGameData(PlayerData data) {
    std::string savegameFile = std::string(savegameFilename);
    FILE* savegame_file = fopen(savegameFile.c_str(), "w"); // TODO encryption of savedata? Maybe include game version for determining compatability
    {
        std::ostringstream oss;
        oss << " lives" << " : " + std::to_string(data.lives) +"\n";
        fputs(oss.str().c_str(), savegame_file);
    }
    {
        std::ostringstream oss;
        oss << " points" << " : " + std::to_string(data.points) +"\n";
        fputs(oss.str().c_str(), savegame_file);
    }
    {
        std::ostringstream oss;
        oss << " levelId" << " : " + std::to_string(data.levelId) +"\n";
        fputs(oss.str().c_str(), savegame_file);
    }
    fclose(savegame_file);
}

PlayerData loadGameData() {
    std::string savegameFile = std::string(savegameFilename);
    if (!fileExists(savegameFile))
        return {0,0,0};

    std::map<std::string, unsigned int> datamap;
    FILE* savegame_file = fopen(savegameFile.c_str(), "r");
    int ch = getc(savegame_file);
    while(ch != EOF) {
        unsigned int val;
        char name[32];
        fscanf(savegame_file, "%s : %d\n", name, &val);

        datamap.insert({name, val});

        ch = getc(savegame_file);
    }
    if (feof(savegame_file))
        std::cout << "End of file reached." << std::endl;
    else
        std::cout << "Something went wrong." << std::endl;
    fclose(savegame_file);


    PlayerData data = {0,0,0};

    // TODO check to ensure all data is loaded? Or allow semi-corrupt data loading?
    if (datamap.find("points") != datamap.end())
        data.points = datamap["points"];
    if (datamap.find("lives") != datamap.end())
        data.lives = datamap["lives"];
    if (datamap.find("levelId") != datamap.end())
        data.levelId = datamap["levelId"];
    return data;
}
