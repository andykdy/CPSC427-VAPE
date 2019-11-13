//
// Created by Cody on 11/12/2019.
//

#include <common.hpp>
#include <iostream>
#include <sstream>
#include "SaveData.hpp"

inline bool fileExists(const std::string &fileName) {
    if (FILE *file = fopen(fileName.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

inline void  parse(const std::string&fileName, std::multiset<leaderboardEntry>* leaderboard) {
    FILE* leaderboard_file = fopen(fileName.c_str(), "r");
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


inline void saveLeaderBoard(const std::string&fileName, const std::multiset<leaderboardEntry>& leaderboard) {
    FILE* leaderboard_file = fopen(fileName.c_str(), "w");

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
    std::string fileName = "leaderboard.vape";

    std::multiset<leaderboardEntry> leaderboard = {};
    if (fileExists(fileName)) {
        parse(fileName, &leaderboard);
    }
    leaderboard.insert({name, points});

    saveLeaderBoard(fileName, leaderboard);
}

std::multiset<leaderboardEntry> getLeaderboard() {
    std::string fileName = "leaderboard.vape";

    std::multiset<leaderboardEntry> leaderboard = {};
    if (fileExists(fileName)) {
        parse(fileName, &leaderboard);
    }
    return leaderboard;
}
