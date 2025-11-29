#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "db.hpp"
#include <string>
#include <libpq-fe.h>
#include <iostream>
#include <map>
#include <ctime>

using namespace std;

// Header for trainer.cpp

class TrainerSystem {
public:
    // Functions i will use for trainer

    // Availabilty
    static void addWeeklyAvailability(Database& db, int trainerId);
    static void addIndividualAvailability(Database& db, int trainerId);
    
    static void addAvailability(Database& db);
    static void updateAvailability(Database& db);

    // Scheldule
    static void viewSchedule(Database& db);

    // Members progress
    static void viewMemberProgress(Database& db, const string& memberName);
};

#endif
