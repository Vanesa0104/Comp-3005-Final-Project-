#ifndef MEMBER_HPP
#define MEMBER_HPP

#include "db.hpp"
#include <string>
using namespace std;

// Header for member.cpp

// Note: All times should be 24 hour format (e.g 16:00 = 4pm)

class MemberSystem {
public:
    // Functons I will use

    // Register/update members
    static void registerMember(Database& db);
    static void updateProfile(Database& db, int memberId, const string& newName, const string& newEmail, const string& newContact);
    
    // Health metric and goals, basically progress
    static void addMetric(Database& db);
    static void setGoal(Database& db);

    // PT sessions and class registeration
    static void schedulePT(Database& db);
    static void registerForClass(Database& db);
     
};

#endif
