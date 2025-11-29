#include <iostream>
using namespace std;

#include "db.hpp"
#include "member.hpp"
#include "trainer.hpp"
#include "admin.hpp"


// Sub-menu for managing member registration and profile updates
void memberManagementMenu(Database& db) {
    // X represents what whatever user chooses
    int x;
    do {
        cout << "\n--- MEMBER MANAGEMENT ---\n"
             << "1. Register New Member\n"
             << "2. Update Existing Member\n"
             << "3. Back to Member Menu\n"
             << "Choice: ";
        cin >> x;
        cin.ignore();

        switch (x) {
            case 1:
                // Calls the register member function in Member
                MemberSystem::registerMember(db);
                break;
            case 2: {
                // Update member data
                int id;
                string name, email, contact;
                
                // Member choosen via member id
                cout << "Enter member ID to update: ";
                cin >> id;
                cin.ignore();

                // Takes user input
                cout << "Enter new name (leave blank to skip): ";
                getline(cin, name);
                cout << "Enter new email (leave blank to skip): ";
                getline(cin, email);
                cout << "Enter new contact info (leave blank to skip): ";
                getline(cin, contact);

                // Leaves same if empty, else updates info
                if (!name.empty() || !email.empty() || !contact.empty()) {
                    MemberSystem::updateProfile(db, id, name, email, contact);
                } else {
                    cout << "No changes entered.\n";
                }
                break;
            }
            case 3:
                // Returns to Member menu
                cout << "Returning to Member Menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(x != 3);
}

// Sub-menu for adding trainer availability
void availabilityMenu(Database& db, int trainerId) {
    int x;
    do {
        cout << "\n--- ADD AVAILABILITY ---\n"
             << "1. Weekly recurring slot\n"
             << "2. Individual date/time slot\n"
             << "3. Back to Trainer Menu\n"
             << "Choice: ";
        cin >> x;
        cin.ignore();

        switch (x) {
            case 1:
                TrainerSystem::addWeeklyAvailability(db, trainerId);
                break;
            case 2:
                TrainerSystem::addIndividualAvailability(db, trainerId);
                break;
            case 3:
                cout << "Returning to Trainer Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

    } while (x != 3);
}


// Main function, contains the menus
int main() {
    // IMPORTANT!!!!!!
    // CHANGE to your actual PostgreSQL password
    Database db("host=localhost dbname=fitness user=postgres password=");

    while (true) {
        // Menu for choosing role
        cout << "\n=== FITNESS SYSTEM ===\n"
             << "1. Member\n"
             << "2. Trainer\n"
             << "3. Admin\n"
             << "0. Exit\n"
             << "Choice: ";
        
        // c represents user choice
        int c;
        cin >> c;
        cin.ignore();

        // If 0 is choosen, leave
        if (c == 0) break;

        // If member menu is choosen
        if (c == 1) {
            // x takes next user choice 
            int x;
            do {
                // Main member menu for choosing member functionalities
                cout << "\n--- MEMBER MENU ---\n"
                << "1. User Registration & Update\n"
                << "2. Add Health Metric Entry\n"
                << "3. Set Fitness Goal\n"
                << "4. Schedule a PT Session\n"
                << "5. Register for a Group Class\n"
                << "6. Back to Main Menu\n"
                << "Choice: ";
                cin >> x;
                cin.ignore();

                switch (x) {
                    case 1:
                        // Calls member register/update menu
                        memberManagementMenu(db);
                        break;
                    case 2:
                        // Calls member health metric function in member.cpp
                        MemberSystem::addMetric(db);
                        break;
                    case 3:
                        // Calls member set fitness goal function in member.cpp
                        MemberSystem::setGoal(db);
                        break;
                    case 4:
                        // Calls member scheduling function in member.cpp
                        MemberSystem::schedulePT(db);
                        break;
                    case 5:
                        // Calls member class join function
                        MemberSystem::registerForClass(db); 
                        break;
                    case 6:
                        // Return to main menu
                        cout << "Returning to Main Menu...\n";
                        break;
                    default:
                        // Choice invalid
                        cout << "Invalid choice!\n";
                }
            } while(x != 6); // Continue in member menu until return to main menu is called
        }
        else if (c == 2) {
            // x is new choice
            int x;
            do {
                // trainer menu
                cout << "\n--- TRAINER MENU ---\n"
                << "1. Add Availability\n"
                << "2. View Your Schedule\n"
                << "3. View Member Progress\n"
                << "4. Back to Main Menu\n"
                << "Choice: ";
                cin >> x;
                cin.ignore();

                switch (x) {
                    case 1: 
                        // takes user input
                        int trainerId;
                        cout << "Enter your Trainer ID: ";
                        cin >> trainerId;
                        cin.ignore();
                        // Calls trainer sub menu
                        availabilityMenu(db, trainerId); 
                        break;
                    case 2: 
                        // Calls view schedule function in trainer.cpp
                        TrainerSystem::viewSchedule(db); 
                        break;
                    case 3: {
                        // Calls view member progress function in trainer.cpp

                        // Needs member name, case sensitive, unique names
                        string memberName;
                        cout << "Enter Member Name (case-sensitive): ";
                        getline(cin, memberName);

                        // Calls view progress function in trainer.cpp
                        TrainerSystem::viewMemberProgress(db, memberName);
                        break;
                    }
                    case 4:
                        // Return to main menu
                        cout << "Returning to Main Menu...\n";
                        break;
                    default: 
                        // Oi, you didn't choose a valid option
                        cout << "Invalid choice.\n";
                } 
            } while (x != 4); // Repeats trainer menu
        }
        else if (c == 3) {
            // x is new user choice
            int x;
            do {
                // Now onto the admin menu
                cout << "\n--- ADMIN MENU ---\n"
                    << "1. View Room Schedule\n"
                    << "2. Book a Room for PT Session\n"
                    << "3. Add Equipment\n"
                    << "4. Log Maintenance\n"
                    << "5. Update Maintenance Log\n"
                    << "6. Create a Class\n"
                    << "7. Update Class Capacity\n"
                    << "8. Cancel a Class\n"
                    << "9. Back to Main Menu\n"
                    << "Choice: ";
                cin >> x;
                cin.ignore();

                switch (x) {
                    case 1: { 
                        // View Room Schedule
                        string room;
                        cout << "Enter room name: ";
                        getline(cin, room);

                        // Check if room exists in room table
                        string checkRoomQ = "SELECT COUNT(*) FROM Room WHERE room_name = '" + room + "';";
                        PGresult* res = db.exec(checkRoomQ);
                        int exists = atoi(PQgetvalue(res, 0, 0));
                        PQclear(res);

                        // If room exists, call view room function in admin.cpp
                        if (exists > 0) {
                            AdminSystem::viewRoom(db, room);
                        } else {
                            cout << "Error: Room '" << room << "' does not exist!\n";
                        }
                        break;
                    }
                    case 2: { 
                        // Book a Room

                        // Takes the user input for the room they want
                        string room, start, end;
                        int trainerId, memberId;
                        cout << "Room: "; getline(cin, room);

                        // Check if room exists
                        string checkRoomQ = "SELECT COUNT(*) FROM Room WHERE room_name = '" + room + "';";
                        PGresult* res = db.exec(checkRoomQ);
                        int exists = atoi(PQgetvalue(res, 0, 0));
                        PQclear(res);

                        // Um, we have no so called room...
                        if (exists == 0) {
                            cout << "Error: Room '" << room << "' does not exist!\n";
                            break; 
                        }

                        // Else, room exists, so take trainer and member id
                        cout << "Trainer ID: "; cin >> trainerId;
                        cout << "Member ID: "; cin >> memberId;
                        cin.ignore();

                        // Takes start and end of sess
                        cout << "Start (YYYY-MM-DD HH:MM): "; getline(cin, start);
                        cout << "End (YYYY-MM-DD HH:MM): "; getline(cin, end);

                        // Use these values and calls book room function in admin.cpp 
                        AdminSystem::bookRoom(db, room, trainerId, memberId, start, end);
                        break;
                    }
                    // Adds equipment to equipment table
                    case 3: {
                        string name;
                        // Gets user input for equipment
                        cout << "Equipment name: "; getline(cin, name);
                        // Calls add equipment function in admin.cpp
                        AdminSystem::addEquipment(db, name);
                        break;
                    }
                    case 4: {
                        // Maintenance function
                        int eqId;
                        string issue, assigned;
                        
                        // Equipment id, Issue problem and assignment(?) for broken equipment   
                        cout << "Equipment ID: "; cin >> eqId; cin.ignore();
                        cout << "Issue description: "; getline(cin, issue);
                        cout << "Assigned to: "; getline(cin, assigned);

                        // Take these values/info and calls logM maintenance in admin.cpp
                        AdminSystem::logMaintenance(db, eqId, issue, assigned);
                        break;
                    }
                    case 5: {
                        // Update equipment status 
                        int logId;
                        string status, notes;

                        // takes user input
                        cout << "Maintenance Log ID: "; cin >> logId; cin.ignore();
                        cout << "Update status (e.g., Pending, In Progress, Completed): "; getline(cin, status);
                        cout << "Notes: "; getline(cin, notes);

                        // Uses values and calls update maintenance in admin.cpp
                        AdminSystem::updateMaintenance(db, logId, status, notes);
                        break;
                    }
                    case 6: {
                        // Creates class
                        string cname, time;
                        int trainerId, capacity;
                        
                        // Takes user input
                        cout << "Class Name: "; getline(cin, cname);
                        cout << "Trainer ID: "; cin >> trainerId; cin.ignore();
                        cout << "Class Time (YYYY-MM-DD HH:MM): "; getline(cin, time);
                        cout << "Capacity: "; cin >> capacity; cin.ignore();

                        // uses info and calls create class in admin.cpp
                        AdminSystem::createClass(db, cname, trainerId, time, capacity);
                        break;
                    }
                    case 7: {
                        // Update class capacity
                        int classId, capacity;

                        // Takes class id and new cap info
                        cout << "Class ID: "; cin >> classId;
                        cout << "New Capacity: "; cin >> capacity; cin.ignore();

                        // Uses info to call class update in admin.cpp
                        AdminSystem::updateClassCapacity(db, classId, capacity);
                        break;
                    }
                    case 8: {
                        // Cancel a class
                        int classId;

                        // Which class to cancel
                        cout << "Class ID: "; cin >> classId; cin.ignore();

                        // takes input and calls cancel class in admin.cpp
                        AdminSystem::cancelClass(db, classId);
                        break;
                    }
                    case 9:
                        // Returns to main menu
                        cout << "Returning to Main Menu...\n";
                        break;
                    default:
                        // INVALID!!!
                        cout << "Invalid choice!\n";
                }
            } while (x != 9); // Keeps calling admin menu until chooses to return to main menu
        }
        // Invalid choice for user role
        else cout << "Invalid role choice.\n";
    }
    return 0; // habit ig
}
