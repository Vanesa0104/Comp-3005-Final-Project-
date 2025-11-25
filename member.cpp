#include "member.hpp"
#include <iostream>
using namespace std;

void MemberSystem::registerMember(Database& db) {
    string name, email, dob, gender, contact;
    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter date of birth (YYYY-MM-DD): ";
    getline(cin, dob);
    cout << "Enter gender: ";
    getline(cin, gender);
    cout << "Enter contact info: ";
    getline(cin, contact);

    string q =
        "INSERT INTO Member (name, email, date_of_birth, gender, contact) VALUES ('" +
        name + "','" + email + "','" + dob + "','" + gender + "','" + contact + "');";

    PGresult* r = db.exec(q);
    if (!r) return;

    cout << "Member registered successfully.\n";
    PQclear(r);
}

void MemberSystem::addMetric(Database& db) {
    int id, hr;
    double w, h;

    cout << "Enter member ID: ";
    cin >> id;
    cout << "Weight: ";
    cin >> w;
    cout << "Height: ";
    cin >> h;
    cout << "Heart rate: ";
    cin >> hr;
    cin.ignore();

    string q =
        "INSERT INTO HealthMetric (member_id, weight, height, heart_rate) VALUES (" +
        to_string(id) + "," + to_string(w) + "," + to_string(h) + "," + to_string(hr) + ");";

    PGresult* r = db.exec(q);
    if (!r) return;

    cout << "Metric logged successfully.\n";
    PQclear(r);
}

void MemberSystem::setGoal(Database& db) {
    int id;
    double tw, tf;

    cout << "Enter member ID: ";
    cin >> id;
    cout << "Target weight: ";
    cin >> tw;
    cout << "Target body fat: ";
    cin >> tf;
    cin.ignore();

    string q =
        "INSERT INTO FitnessGoal (member_id, target_weight, target_body_fat, start_date, end_date) "
        "VALUES (" + to_string(id) + "," + to_string(tw) + "," + to_string(tf) +
        ", CURRENT_DATE, CURRENT_DATE + INTERVAL '30 day');";

    PGresult* r = db.exec(q);
    if (!r) return;

    cout << "Goal set successfully.\n";
    PQclear(r);
}

void MemberSystem::schedulePT(Database& db) {
    int mId, tId;
    string start, end, room;

    cout << "Member ID: ";
    cin >> mId;
    cout << "Trainer ID: ";
    cin >> tId;
    cin.ignore();
    cout << "Start time (YYYY-MM-DD HH:MM): ";
    getline(cin, start);
    cout << "End time (YYYY-MM-DD HH:MM): ";
    getline(cin, end);
    cout << "Room: ";
    getline(cin, room);

    string q =
        "INSERT INTO PTSession (member_id, trainer_id, start_time, end_time, room) "
        "SELECT " + to_string(mId) + "," + to_string(tId) + ",'" + start + "','" + end + "','" + room + "' "
        "WHERE EXISTS (SELECT 1 FROM TrainerAvailability "
        "WHERE trainer_id=" + to_string(tId) +
        " AND '" + start + "' >= start_time "
        "AND '" + end + "' <= end_time);";

    PGresult* r = db.exec(q);
    if (!r) {
        cout << "Failed: Trainer may not be available!\n";
        return;
    }

    cout << "PT session scheduled successfully.\n";
    PQclear(r);
}
