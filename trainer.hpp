#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "db.hpp"
#include <string>
using namespace std;

class TrainerSystem {
public:
    static void addAvailability(Database& db);
    static void viewSchedule(Database& db);
    static void viewMemberProgress(Database& db, int memberId);
};

#endif
