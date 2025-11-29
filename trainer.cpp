#include "trainer.hpp"

using namespace std;


// Add weekly recurring availability function
void TrainerSystem::addWeeklyAvailability(Database& db, int trainerId) {
    string dayOfWeek, startTime, endTime;
    // Take user input
    cout << "Enter day of week: ";
    getline(cin, dayOfWeek);
    cout << "Start time (HH:MM): ";
    getline(cin, startTime);
    cout << "End time (HH:MM): ";
    getline(cin, endTime);

    map<string, int> days = {
        {"Sunday",0},{"Monday",1},{"Tuesday",2},{"Wednesday",3},
        {"Thursday",4},{"Friday",5},{"Saturday",6}
    };

    if (days.find(dayOfWeek) == days.end()) {
        cout << "Invalid day of week!\n";
        return;
    }

    int targetDow = days[dayOfWeek];

    // Repeat for next 4 weeks
    time_t now = time(0);
    tm* ltm = localtime(&now);

    for (int i = 0; i < 4; i++) {
        // Find next date matching target day of week
        tm nextDate = *ltm;
        int currentDow = nextDate.tm_wday;
        int diff = (targetDow - currentDow + 7) % 7 + i*7; // offset by week
        nextDate.tm_mday += diff;
        mktime(&nextDate); // normalize date

        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &nextDate);

        string startDatetime = string(dateStr) + " " + startTime;
        string endDatetime   = string(dateStr) + " " + endTime;

        string q = "INSERT INTO TrainerAvailability (trainer_id, start_time, end_time) "
                   "SELECT " + to_string(trainerId) + ", '" + startDatetime + "', '" + endDatetime + "' "
                   "WHERE NOT EXISTS (SELECT 1 FROM TrainerAvailability "
                   "WHERE trainer_id=" + to_string(trainerId) +
                   " AND (start_time < '" + endDatetime + "' AND end_time > '" + startDatetime + "'));";

        PGresult* r = db.exec(q);
        PQclear(r);
    }

    cout << "Weekly recurring availability added for the next 4 weeks.\n";
}

// Add individual availability function
void TrainerSystem::addIndividualAvailability(Database& db, int trainerId) {
    string start, end;
    // Take user input
    cout << "Start date and time (YYYY-MM-DD HH:MM): ";
    getline(cin, start);
    cout << "End date and time (YYYY-MM-DD HH:MM): ";
    getline(cin, end);

    // Insert into trainer aval. table
    string q = "INSERT INTO TrainerAvailability (trainer_id, start_time, end_time) "
               "SELECT " + to_string(trainerId) + ", '" + start + "', '" + end + "' "
               "WHERE NOT EXISTS (SELECT 1 FROM TrainerAvailability "
               "WHERE trainer_id=" + to_string(trainerId) +
               " AND (start_time < '" + end + "' AND end_time > '" + start + "'));";
    // Result of ^^^ and check if invalid
    PGresult* r = db.exec(q);
    if (!r) {
        cout << "Failed: Overlapping availability detected!\n";
        return;
    }
    // Update user and clear r
    PQclear(r);
    cout << "Individual availability added.\n";
}

// Update aval function
void TrainerSystem::updateAvailability(Database& db) {
    int trainerId;

    // User input for user id
    cout << "Enter Trainer ID: ";
    cin >> trainerId;
    cin.ignore();

    // Show existing slots
    string q = "SELECT availability_id, start_time, end_time FROM TrainerAvailability WHERE trainer_id=" + to_string(trainerId) + " ORDER BY start_time;";
    // Get result of ^^^
    PGresult* r = db.exec(q);
    // If ^^^ fails....
    if (!r) return;

    // No availability found 
    int rows = PQntuples(r);
    if (rows == 0) {
        cout << "No availability slots found.\n";
        PQclear(r);
        return;
    }
    
    // YAY! Aval. found
    cout << "Existing availability slots:\n";
    for (int i = 0; i < rows; i++) {
        cout << PQgetvalue(r, i, 0) << ": " << PQgetvalue(r, i, 1) << " to " << PQgetvalue(r, i, 2) << "\n";
    }

    // Which one to update? take input
    int slotId;
    cout << "Enter slot ID to update: ";
    cin >> slotId;
    cin.ignore();

    // takes new info input 
    string newStart, newEnd;
    cout << "New start datetime (YYYY-MM-DD HH:MM): ";
    getline(cin, newStart);
    cout << "New end datetime (YYYY-MM-DD HH:MM): ";
    getline(cin, newEnd);

    // Uodates trainer aval. table
    string updateQuery = "UPDATE TrainerAvailability SET start_time='" + newStart + "', end_time='" + newEnd + "' "
                         "WHERE availability_id=" + to_string(slotId) + " "
                         "AND NOT EXISTS (SELECT 1 FROM TrainerAvailability "
                         "WHERE trainer_id=" + to_string(trainerId) +
                         " AND availability_id <> " + to_string(slotId) +
                         " AND ((start_time < '" + newEnd + "' AND end_time > '" + newStart + "')));";

    // Takes result of ^^^
    PGresult* u = db.exec(updateQuery);

    // If ^^ invalid/error
    if (!u) {
        cout << "Failed: Overlapping availability detected!\n";
        return;
    }

    // clear u and let user know all good
    PQclear(u);
    cout << "Availability updated successfully.\n";
}

// View trainer's upcoming sessions
void TrainerSystem::viewSchedule(Database& db) {
    int tId;

    // Which trainer to view? takes input
    cout << "Trainer ID: ";
    cin >> tId;
    cin.ignore();

    // Select pt session table
    string q = "SELECT session_id, member_id, start_time, end_time, room "
               "FROM PTSession WHERE trainer_id=" + to_string(tId) + " "
               "ORDER BY start_time;";
    // Takes result of ^^^
    PGresult* r = db.exec(q);
    // Invalid 0-0?
    if (!r) return;

    // Displays sch. info
    int rows = PQntuples(r);
    cout << "\nTrainer Schedule:\n";
    for (int i = 0; i < rows; i++) {
        cout << "Session: " << PQgetvalue(r, i, 0)
             << " Member: " << PQgetvalue(r, i, 1)
             << " Start: " << PQgetvalue(r, i, 2)
             << " End: " << PQgetvalue(r, i, 3)
             << " Room: " << PQgetvalue(r, i, 4) << "\n";
    }
    // Clear r
    PQclear(r);
}

// View member progress by name (case-sensitive)
void TrainerSystem::viewMemberProgress(Database& db, const string& memberName) {
    // Selects health metrics and fitness goals
    string q1 = "SELECT m.member_id, m.name, h.weight, h.height, h.heart_rate, "
                "g.target_weight, g.target_body_fat "
                "FROM Member m "
                "LEFT JOIN HealthMetric h ON m.member_id = h.member_id "
                "LEFT JOIN FitnessGoal g ON m.member_id = g.member_id "
                "WHERE m.name = '" + memberName + "';";
    // Result of ^^^, check if valid
    PGresult* r1 = db.exec(q1);
    if (!r1) return;

    // Oi, no member found
    int rows = PQntuples(r1);
    if (rows == 0) {
        cout << "No member found with the exact name: " << memberName << "\n";
        PQclear(r1);
        return;
    }

    // Display health metrics and fitness goals
    int memberId = stoi(PQgetvalue(r1, 0, 0));
    cout << "Name: " << PQgetvalue(r1, 0, 1)
         << ", Weight: " << PQgetvalue(r1, 0, 2)
         << ", Height: " << PQgetvalue(r1, 0, 3)
         << ", Heart Rate: " << PQgetvalue(r1, 0, 4)
         << ", Target Weight: " << PQgetvalue(r1, 0, 5)
         << ", Target Body Fat: " << PQgetvalue(r1, 0, 6) << "\n";
    // Clear r1
    PQclear(r1);

    // Query for class attendance
    string q2 = "SELECT gc.class_name, gc.class_time, t.name AS trainer_name "
                "FROM GroupClass gc "
                "JOIN Trainer t ON gc.trainer_id = t.trainer_id "
                "JOIN MemberClass mc ON gc.class_id = mc.class_id "
                "WHERE mc.member_id = " + to_string(memberId) + " "
                "ORDER BY gc.class_time;";
    // Result of ^^^, check if invalid
    PGresult* r2 = db.exec(q2);
    if (!r2) return;

    // Member has no classes (lazy? lol no)
    int classRows = PQntuples(r2);
    if (classRows == 0) {
        cout << "No class attendance records found for this member.\n";
    } else {
        // Displays info
        cout << "\nClass Attendance:\n";
        for (int i = 0; i < classRows; i++) {
            cout << "Class: " << PQgetvalue(r2, i, 0)
                 << ", Time: " << PQgetvalue(r2, i, 1)
                 << ", Trainer: " << PQgetvalue(r2, i, 2) << "\n";
        }
    }
    // Clear r2
    PQclear(r2);
}

