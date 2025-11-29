#include "member.hpp"
#include <iostream>
using namespace std;


// Register a new member function 
void MemberSystem::registerMember(Database& db) {
    string name, email, dob, gender, contact;

    // Takes user input for info
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

    // Insert new member into Member table
    string q =
        "INSERT INTO Member (name, email, date_of_birth, gender, contact) VALUES ('" +
        name + "','" + email + "','" + dob + "','" + gender + "','" + contact + "');";
    // Result of ^^ and check if invalid
    PGresult* r = db.exec(q);
    if (!r) return;

    // Lets user know member registered, clears r
    cout << "Member registered successfully.\n";
    PQclear(r);
}

// Function to add health metric 
void MemberSystem::addMetric(Database& db) {
    int id, hr;
    double w, h;

    // Takes user input
    cout << "Enter member ID: ";
    cin >> id;
    cout << "Weight: ";
    cin >> w;
    cout << "Height: ";
    cin >> h;
    cout << "Heart rate: ";
    cin >> hr;
    cin.ignore();

    // Insert metric into Health Metric table
    string q =
        "INSERT INTO HealthMetric (member_id, weight, height, heart_rate) VALUES (" +
        to_string(id) + "," + to_string(w) + "," + to_string(h) + "," + to_string(hr) + ");";

    // Result of ^^^ and check invalid
    PGresult* r = db.exec(q);
    if (!r) return;

    // Lets uer know all good and clears r
    cout << "Metric logged successfully.\n";
    PQclear(r);
}

// Setting Fitness Goal Fucntion 
void MemberSystem::setGoal(Database& db) {
    int id;
    double tw, tf;

    // Takes user input 
    cout << "Enter member ID: ";
    cin >> id;
    cout << "Target weight: ";
    cin >> tw;
    cout << "Target body fat: ";
    cin >> tf;
    cin.ignore();

    // Insert goal into fitness goal table
    string q =
        "INSERT INTO FitnessGoal (member_id, target_weight, target_body_fat, start_date, end_date) "
        "VALUES (" + to_string(id) + "," + to_string(tw) + "," + to_string(tf) +
        ", CURRENT_DATE, CURRENT_DATE + INTERVAL '30 day');";
    // Result of ^^^ and check invalid
    PGresult* r = db.exec(q);
    if (!r) return;

    // Lets user know its all good and clears r
    cout << "Goal set successfully.\n";
    PQclear(r);
}

void MemberSystem::schedulePT(Database& db) {
    int mId, tId;
    string start, end, room;

    // Takes user input 
    cout << "Member ID: ";
    cin >> mId;
    cout << "Trainer ID: ";
    cin >> tId;
    cin.ignore();
    // Must be 24 hr format
    cout << "Start time (YYYY-MM-DD HH:MM): ";
    getline(cin, start);
    cout << "End time (YYYY-MM-DD HH:MM): ";
    getline(cin, end);
    cout << "Room: ";
    getline(cin, room);

    // Insert PT session IF trainer is available
    string q =
        "INSERT INTO PTSession (member_id, trainer_id, start_time, end_time, room) "
        "SELECT " + to_string(mId) + "," + to_string(tId) + ",'" + start + "','" + end + "','" + room + "' "
        "WHERE EXISTS (SELECT 1 FROM TrainerAvailability "
        "WHERE trainer_id=" + to_string(tId) +
        " AND '" + start + "' >= start_time "
        "AND '" + end + "' <= end_time);";

    PGresult* r = db.exec(q);
    if (!r) return;

    // Check number of rows actually inserted
    int affected = atoi(PQcmdTuples(r));
    PQclear(r);

    if (affected == 0) {
        cout << "Failed: Trainer may not be available at that time.\n";
        return;
    }

    // Lets user know all good
    cout << "PT session scheduled successfully.\n";
}

// Update member profile function 
void MemberSystem::updateProfile(Database& db, int memberId, const string& newName, const string& newEmail, const string& newContact) {
    // To avoid repeating/typing the same thing
    // Update Member Table
    string q = "UPDATE Member SET ";
    bool first = true;

    // If name is empty, no update made
    // Otherwise, update name
    if (!newName.empty()) {
        q += "name='" + newName + "'";
        first = false;
    }
    // If no email typed, no update made
    // Otherwise, new email updated
    if (!newEmail.empty()) {
        if (!first) q += ", ";
        q += "email='" + newEmail + "'";
        first = false;
    }
    // If no new contact made, no update made
    // Else, update new contact info
    if (!newContact.empty()) {
        if (!first) q += ", ";
        q += "contact='" + newContact + "'";
    }

    // Updates Member table where ^^^ changes were made
    q += " WHERE member_id=" + to_string(memberId) + ";";

    // Result of ^^^ and check invalid
    PGresult* r = db.exec(q);
    if (r) PQclear(r);

    // Let user know update successful
    cout << "Member profile updated successfully.\n";
}

void MemberSystem::registerForClass(Database& db) {
    int memberId, classId;

    cout << "Enter Member ID: ";
    cin >> memberId;
    cout << "Enter Class ID: ";
    cin >> classId;
    cin.ignore();

    // Check if member is already registered
    string checkExisting =
        "SELECT COUNT(*) FROM MemberClass "
        "WHERE member_id = " + to_string(memberId) +
        " AND class_id = " + to_string(classId) + ";";
    // Result of ^^^ and checks if yes
    PGresult* res = db.exec(checkExisting);
    int already = atoi(PQgetvalue(res, 0, 0));
    // Clears result of ^^^ (res)
    PQclear(res);

    // If they are registered, let them know
    if (already > 0) {
        cout << "You are already registered for this class.\n";
        return;
    }

    // Check if class cap is full
    string checkCapacity =
        "SELECT "
        "(SELECT COUNT(*) FROM MemberClass WHERE class_id = " + to_string(classId) + ") AS registered, "
        "(SELECT capacity FROM GroupClass WHERE class_id = " + to_string(classId) + ") AS cap;";
    // Result of ^^^
    res = db.exec(checkCapacity);

    // Check if class exists and also if full
    int registered = atoi(PQgetvalue(res, 0, 0));
    int capacity   = atoi(PQgetvalue(res, 0, 1));
    // Clears res
    PQclear(res);

    // If class doesn't exist, let user know
    if (capacity == 0) {
        cout << "Error: Class does not exist.\n";
        return;
    }

    // If cap is full, let user know
    if (registered >= capacity) {
        cout << "Class is full. Registration denied.\n";
        return;
    }

    // Insert member into class
    string insertQ =
        "INSERT INTO MemberClass (member_id, class_id) "
        "VALUES (" + to_string(memberId) + ", " + to_string(classId) + ");";
    // Result of ^^^
    PGresult* r = db.exec(insertQ);

    // Error occurs?
    if (!r) {
        cout << "Unexpected SQL error. Registration failed.\n";
        return;
    }

    // Clears r and let user know registration was a success!
    PQclear(r);
    cout << "Successfully registered for the class!\n";
}
