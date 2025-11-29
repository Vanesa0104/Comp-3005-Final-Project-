#include "admin.hpp"
#include <iostream>
using namespace std;

// View room schedule function
void AdminSystem::viewRoom(Database& db, const string& roomName) {
    // Takes Room Name from user input ^^ and selects it from PT session table
    string q = "SELECT session_id, trainer_id, member_id, start_time, end_time "
               "FROM PTSession WHERE room='" + roomName + "' ORDER BY start_time;";
    // Result of ^^^ and checks invalid
    PGresult* r = db.exec(q);
    if (!r) return;

    // Displays the info of ^^^ to user
    int rows = PQntuples(r);
    for (int i = 0; i < rows; i++) {
        cout << "Session: " << PQgetvalue(r, i, 0)
             << " Trainer: " << PQgetvalue(r, i, 1)
             << " Member: " << PQgetvalue(r, i, 2)
             << " Start: " << PQgetvalue(r, i, 3)
             << " End: " << PQgetvalue(r, i, 4) << "\n";
    }
    // Clears r
    PQclear(r);
}

// Book a room function
void AdminSystem::bookRoom(Database& db, const string& roomName, int trainerId, int memberId, const string& start, const string& end) {
    // Select room from PT session table using room name from ^
    string check = "SELECT * FROM PTSession WHERE room='" + roomName + "' "
                   "AND start_time < '" + end + "' AND end_time > '" + start + "';";
    // Result of ^^ 
    PGresult* r = db.exec(check);

    // Invalid message, room conflict 
    if (r && PQntuples(r) > 0) {
        cout << "FAILED: Room conflict!\n";
        PQclear(r);
        return;
    }
    // If all good, then clear r
    if(r) PQclear(r);

    // Insert the now valid checked booking into PT session table
    string q = "INSERT INTO PTSession (member_id, trainer_id, start_time, end_time, room) "
               "VALUES (" + to_string(memberId) + "," + to_string(trainerId) +
               ",'" + start + "','" + end + "','" + roomName + "');";
    db.exec(q);

    // Lets user know booking successful
    cout << "Room booked successfully.\n";
}

// Adding equipment function
void AdminSystem::addEquipment(Database& db, const string& name) {
    // Insert the equipment inot equipment table
    string q = "INSERT INTO Equipment (name) VALUES ('" + name + "');";
    db.exec(q);

    // Let user know alls good
    cout << "Equipment added.\n";
}

// Log the equipmentance maintenance function
void AdminSystem::logMaintenance(Database& db, int equipmentId, const string& issue, const string& assignedTo) {
    // Insert log into log maintenance table
    string q = "INSERT INTO MaintenanceLog (equipment_id, issue_description, assigned_to) "
               "VALUES (" + to_string(equipmentId) + ", '" + issue + "', '" + assignedTo + "');";
    db.exec(q);

    // Let user it was logged
    cout << "Maintenance logged.\n";
}

// Update an existing maintenance record function
void AdminSystem::updateMaintenance(Database& db, int maintenanceId, const string& status, const string& notes) {
    // Selects the log based off input ^^^ and updates it in maintenance log table
    string q = "UPDATE MaintenanceLog SET status = '" + status + "', notes = '" + notes + "' "
               "WHERE maintenance_id = " + to_string(maintenanceId) + ";";
    db.exec(q);

    // Lets user update successful
    cout << "Maintenance log updated successfully.\n";
}



// Class creation function
void AdminSystem::createClass(Database& db, const string& className, int trainerId, const string& classTime, int capacity) {
    // Insert the class into Group class table
    string q = "INSERT INTO GroupClass (class_name, trainer_id, class_time, capacity) "
               "VALUES ('" + className + "'," + to_string(trainerId) + ",'" + classTime + "'," + to_string(capacity) + ");";
    db.exec(q);

    // Lets user class was created
    cout << "Class created.\n";
}

// Update class capacity function
void AdminSystem::updateClassCapacity(Database& db, int classId, int newCapacity) {
    // Select class from group class table based on ^^^ and updates it
    string q = "UPDATE GroupClass SET capacity=" + to_string(newCapacity) + " WHERE class_id=" + to_string(classId) + ";";
    db.exec(q);

    // Lets user know class was updated
    cout << "Class capacity updated.\n";
}


// Deleting a class function
void AdminSystem::cancelClass(Database& db, int classId) {
    // Delete class choosen based off of ^^^ and deltes it from Group Class table
    string q = "DELETE FROM GroupClass WHERE class_id=" + to_string(classId) + ";";
    db.exec(q);

    // Lets user know class was cancelled
    cout << "Class canceled.\n";
}

