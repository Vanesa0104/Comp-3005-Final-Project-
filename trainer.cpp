#include "trainer.hpp"
#include <iostream>

using namespace std;

// Add availability without overlapping
void TrainerSystem::addAvailability(Database& db) {
    int tId;
    string s, e;
    cout << "Trainer ID: ";
    cin >> tId;
    cin.ignore();

    cout << "Start time (YYYY-MM-DD HH:MM): ";
    getline(cin, s);
    cout << "End time: ";
    getline(cin, e);

    // Check for overlap
    string check = "SELECT * FROM TrainerAvailability "
                   "WHERE trainer_id=" + to_string(tId) +
                   " AND start_time < '" + e + "' AND end_time > '" + s + "';";
    PGresult* res = db.exec(check);
    if (res && PQntuples(res) > 0) {
        cout << "FAILED: Overlaps with existing slot.\n";
        PQclear(res);
        return;
    }
    if(res) PQclear(res);

    string q = "INSERT INTO TrainerAvailability (trainer_id, start_time, end_time) "
               "VALUES (" + to_string(tId) + ", '" + s + "', '" + e + "');";
    db.exec(q);
    cout << "Availability added.\n";
}

// View trainer's upcoming sessions
void TrainerSystem::viewSchedule(Database& db) {
    int tId;
    cout << "Trainer ID: ";
    cin >> tId;
    cin.ignore();

    string q = "SELECT session_id, member_id, start_time, end_time, room "
               "FROM PTSession WHERE trainer_id=" + to_string(tId) + " "
               "ORDER BY start_time;";
    PGresult* r = db.exec(q);
    if (!r) return;

    int rows = PQntuples(r);
    cout << "\nTrainer Schedule:\n";
    for (int i = 0; i < rows; i++) {
        cout << "Session: " << PQgetvalue(r, i, 0)
             << " Member: " << PQgetvalue(r, i, 1)
             << " Start: " << PQgetvalue(r, i, 2)
             << " End: " << PQgetvalue(r, i, 3)
             << " Room: " << PQgetvalue(r, i, 4) << "\n";
    }
    PQclear(r);
}

// View member's health metrics and fitness goals
void TrainerSystem::viewMemberProgress(Database& db, int memberId) {
    string q = "SELECT m.name, h.weight, h.height, h.heart_rate, g.target_weight, g.target_body_fat "
               "FROM Member m "
               "LEFT JOIN HealthMetric h ON m.member_id = h.member_id "
               "LEFT JOIN FitnessGoal g ON m.member_id = g.member_id "
               "WHERE m.member_id = " + to_string(memberId) + ";";
    PGresult* r = db.exec(q);
    if (!r) return;

    int rows = PQntuples(r);
    for (int i = 0; i < rows; i++) {
        cout << "Name: " << PQgetvalue(r, i, 0)
             << ", Weight: " << PQgetvalue(r, i, 1)
             << ", Height: " << PQgetvalue(r, i, 2)
             << ", Heart Rate: " << PQgetvalue(r, i, 3)
             << ", Target Weight: " << PQgetvalue(r, i, 4)
             << ", Target Body Fat: " << PQgetvalue(r, i, 5) << "\n";
    }
    PQclear(r);
}
