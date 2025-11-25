#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "db.hpp"
#include <string>
using namespace std;

class AdminSystem {
public:
    // Room management
    static void viewRoom(Database& db, const string& roomName);
    static void bookRoom(Database& db, const string& roomName, int trainerId, int memberId, const string& start, const string& end);

    // Equipment management
    static void addEquipment(Database& db, const string& name);
    static void logMaintenance(Database& db, int equipmentId, const string& issue, const string& assignedTo);

    // Class management
    static void createClass(Database& db, const string& className, int trainerId, const string& classTime, int capacity);
    static void updateClassCapacity(Database& db, int classId, int newCapacity);
    static void cancelClass(Database& db, int classId);
};

#endif
