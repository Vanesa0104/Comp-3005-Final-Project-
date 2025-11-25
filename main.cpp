#include <iostream>
using namespace std;

#include "db.hpp"
#include "member.hpp"
#include "trainer.hpp"
#include "admin.hpp"

int main() {
    // CHANGE to your actual PostgreSQL password
    Database db("host=localhost dbname=fitness user=postgres password=Vanesa@2004");

    while (true) {
        cout << "\n=== FITNESS SYSTEM ===\n"
             << "1. Member\n"
             << "2. Trainer\n"
             << "3. Admin\n"
             << "0. Exit\n"
             << "Choice: " << flush;
        
        int c;
        cin >> c;
        cin.ignore();

        if (c == 0) break;

        if (c == 1) {
            cout << "\n--- MEMBER MENU ---\n"
                 << "1. User Registeration\n"
                 << "2. Add Health Metric Entry\n"
                 << "3. Set Fitness Goal\n"
                 << "4. Schedule a PT Session\n"
                 << "Choice: ";
            int x;
            cin >> x;
            cin.ignore();

            switch (x) {
                case 1: MemberSystem::registerMember(db); break;
                case 2: MemberSystem::addMetric(db); break;
                case 3: MemberSystem::setGoal(db); break;
                case 4: MemberSystem::schedulePT(db); break;
                default: cout << "Invalid choice.\n";
            }
        }

        else if (c == 2) {
            cout << "\n--- TRAINER MENU ---\n"
                 << "1. Add Availability\n"
                 << "2. View Your Schedule\n"
                 << "3. View Member Progress\n"
                 << "Choice: ";
            int x;
            cin >> x;
            cin.ignore();

            switch (x) {
                case 1: TrainerSystem::addAvailability(db); break;
                case 2: TrainerSystem::viewSchedule(db); break;
                case 3: {
                    int memberId;
                    cout << "Enter Member ID: ";
                    cin >> memberId;
                    cin.ignore();
                    TrainerSystem::viewMemberProgress(db, memberId);
                    break;
                }
                default: cout << "Invalid choice.\n";
            }
        }

        else if (c == 3) {
            cout << "\n--- ADMIN MENU ---\n"
                 << "1. View Room Schedule\n"
                 << "2. Book a Room for PT Session\n"
                 << "3. Add Equipment\n"
                 << "4. Log Maintenance\n"
                 << "5. Create a Class\n"
                 << "6. Update Class Capacity\n"
                 << "7. Cancel a Class\n"
                 << "Choice: ";
            int x;
            cin >> x;
            cin.ignore();

            switch (x) {
                case 1: {
                    string room;
                    cout << "Enter room name: ";
                    getline(cin, room);
                    AdminSystem::viewRoom(db, room);
                    break;
                }
                case 2: {
                    string room, start, end;
                    int trainerId, memberId;
                    cout << "Room: "; getline(cin, room);
                    cout << "Trainer ID: "; cin >> trainerId;
                    cout << "Member ID: "; cin >> memberId;
                    cin.ignore();
                    cout << "Start (YYYY-MM-DD HH:MM): "; getline(cin, start);
                    cout << "End (YYYY-MM-DD HH:MM): "; getline(cin, end);
                    AdminSystem::bookRoom(db, room, trainerId, memberId, start, end);
                    break;
                }
                case 3: {
                    string name;
                    cout << "Equipment name: "; getline(cin, name);
                    AdminSystem::addEquipment(db, name);
                    break;
                }
                case 4: {
                    int eqId; string issue, assigned;
                    cout << "Equipment ID: "; cin >> eqId; cin.ignore();
                    cout << "Issue description: "; getline(cin, issue);
                    cout << "Assigned to: "; getline(cin, assigned);
                    AdminSystem::logMaintenance(db, eqId, issue, assigned);
                    break;
                }
                case 5: {
                    string cname, time; int trainerId, capacity;
                    cout << "Class Name: "; getline(cin, cname);
                    cout << "Trainer ID: "; cin >> trainerId; cin.ignore();
                    cout << "Class Time (YYYY-MM-DD HH:MM): "; getline(cin, time);
                    cout << "Capacity: "; cin >> capacity; cin.ignore();
                    AdminSystem::createClass(db, cname, trainerId, time, capacity);
                    break;
                }
                case 6: {
                    int classId, capacity;
                    cout << "Class ID: "; cin >> classId;
                    cout << "New Capacity: "; cin >> capacity; cin.ignore();
                    AdminSystem::updateClassCapacity(db, classId, capacity);
                    break;
                }
                case 7: {
                    int classId;
                    cout << "Class ID: "; cin >> classId; cin.ignore();
                    AdminSystem::cancelClass(db, classId);
                    break;
                }
                default: cout << "Invalid choice.\n";
            }
        }
        else cout << "Invalid role choice.\n";
    }

    return 0;
}


