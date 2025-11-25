#ifndef MEMBER_HPP
#define MEMBER_HPP

#include "db.hpp"
#include <string>
using namespace std;

class MemberSystem {
public:
    static void registerMember(Database& db);
    static void addMetric(Database& db);
    static void setGoal(Database& db);
    static void schedulePT(Database& db);
};

#endif
