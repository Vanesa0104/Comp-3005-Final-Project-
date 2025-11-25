#include "admin.hpp"
#include <iostream>
using namespace std;

// View room schedule
void AdminSystem::viewRoom(Database& db, const string& roomName) {
    string q = "SELECT session_id, trainer_id, member_id, start_time, end_time "
               "FROM PTSession WHERE room='" + roomName + "' ORDER BY start_time;";
    PGresult* r = db.exec(q);
    if (!r) return;

    int rows = PQntuples(r);
    for (int i = 0; i < rows; i++) {
        cout << "Session: " << PQgetvalue(r, i, 0)
             << " Trainer: " << PQgetvalue(r, i, 1)
             << " Member: " << PQgetvalue(r, i, 2)
             << " Start: " << PQgetvalue(r, i, 3)
             << " End: " << PQgetvalue(r, i, 4) << "\n";
    }
    PQclear(r);
}

// Book a room
void AdminSystem::bookRoom(Database& db, const string& roomName, int trainerId, int memberId, const string& start, const string& end) {
    string check = "SELECT * FROM PTSession WHERE room='" + roomName + "' "
                   "AND start_time < '" + end + "' AND end_time > '" + start + "';";
    PGresult* r = db.exec(check);
    if (r && PQntuples(r) > 0) {
        cout << "FAILED: Room conflict!\n";
        PQclear(r);
        return;
    }
    if(r) PQclear(r);

    string q = "INSERT INTO PTSession (member_id, trainer_id, start_time, end_time, room) "
               "VALUES (" + to_string(memberId) + "," + to_string(trainerId) +
               ",'" + start + "','" + end + "','" + roomName + "');";
    db.exec(q);
    cout << "Room booked successfully.\n";
}

// Equipment management
void AdminSystem::addEquipment(Database& db, const string& name) {
    string q = "INSERT INTO Equipment (name) VALUES ('" + name + "');";
    db.exec(q);
    cout << "Equipment added.\n";
}

void AdminSystem::logMaintenance(Database& db, int equipmentId, const string& issue, const string& assignedTo) {
    string q = "INSERT INTO MaintenanceLog (equipment_id, issue_description, assigned_to) "
               "VALUES (" + to_string(equipmentId) + ", '" + issue + "', '" + assignedTo + "');";
    db.exec(q);
    cout << "Maintenance logged.\n";
}


// Class management
void AdminSystem::createClass(Database& db, const string& className, int trainerId, const string& classTime, int capacity) {
    string q = "INSERT INTO GroupClass (class_name, trainer_id, class_time, capacity) "
               "VALUES ('" + className + "'," + to_string(trainerId) + ",'" + classTime + "'," + to_string(capacity) + ");";
    db.exec(q);
    cout << "Class created.\n";
}

void AdminSystem::updateClassCapacity(Database& db, int classId, int newCapacity) {
    string q = "UPDATE GroupClass SET capacity=" + to_string(newCapacity) + " WHERE class_id=" + to_string(classId) + ";";
    db.exec(q);
    cout << "Class capacity updated.\n";
}

void AdminSystem::cancelClass(Database& db, int classId) {
    string q = "DELETE FROM GroupClass WHERE class_id=" + to_string(classId) + ";";
    db.exec(q);
    cout << "Class canceled.\n";
}

