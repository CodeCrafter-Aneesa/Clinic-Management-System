#include<iostream>     // For input/output operations
#include<fstream>      // For file handling
#include<string>       // For using string data type
#include<vector>       // For using dynamic arrays
#include<iomanip>      // For formatted output
#include<windows.h>    // For setting console text color

using namespace std;

// -------------------- Structures (Data Models) --------------------

// Function to set console text color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Doctor structure
struct Doctor {
    int id;
    string name;
    string specialization;
    string timeSlot;
    string contact;
};

// Patient structure
struct Patient {
    int id;
    string name;
    int age;
    string contact;
};

// Appointment structure
struct Appointment {
    int id;
    int doctorId;
    int patientId;
    string date;
    string slot;
};

// -------------------- Global Vectors --------------------
vector<Doctor> doctors;
vector<Patient> patients;
vector<Appointment> appointments;

// -------------------- File I/O --------------------

void loadDoctors() {
    doctors.clear();
    ifstream file("doctors.txt");
    Doctor d;
    while (file >> d.id) {
        file.ignore();
        getline(file, d.name);
        getline(file, d.specialization);
        getline(file, d.timeSlot);
        getline(file, d.contact);
        doctors.push_back(d);
    }
    file.close();
}

void saveDoctors() {
    ofstream file("doctors.txt");
    for (Doctor d : doctors) {
        file << d.id << "\n"
             << d.name << "\n"
             << d.specialization << "\n"
             << d.timeSlot << "\n"
             << d.contact << "\n";
    }
    file.close();
}

void loadPatients() {
    patients.clear();
    ifstream file("patients.txt");
    Patient p;
    while (file >> p.id) {
        file.ignore();
        getline(file, p.name);
        file >> p.age;
        file.ignore();
        getline(file, p.contact);
        patients.push_back(p);
    }
    file.close();
}

void savePatients() {
    ofstream file("patients.txt");
    for (Patient p : patients) {
        file << p.id << "\n"
             << p.name << "\n"
             << p.age << "\n"
             << p.contact << "\n";
    }
    file.close();
}

void loadAppointments() {
    appointments.clear();
    ifstream file("appointments.txt");
    Appointment a;
    while (file >> a.id >> a.doctorId >> a.patientId) {
        file.ignore();
        getline(file, a.date);
        getline(file, a.slot);
        appointments.push_back(a);
    }
    file.close();
}

void saveAppointments() {
    ofstream file("appointments.txt");
    for (Appointment a : appointments) {
        file << a.id << " " << a.doctorId << " " << a.patientId << "\n"
             << a.date << "\n"
             << a.slot << "\n";
    }
    file.close();
}

// -------------------- Utility Functions --------------------

Doctor* findDoctor(int id) {
    for (auto &d : doctors)
        if (d.id == id) return &d;
    return nullptr;
}

Patient* findPatient(int id) {
    for (auto &p : patients)
        if (p.id == id) return &p;
    return nullptr;
}

Doctor* findDoctorByName(const string& name) {
    for (auto &d : doctors)
        if (d.name == name) return &d;
    return nullptr;
}

// -------------------- Patient Book Appointment --------------------
void patientBookAppointment(int patientId);


// -------------------- Patient Menu --------------------
void patientMenu(int patientId) {
    int choice;
    do {
        setColor(11);
        cout << "\n=========== PATIENT MENU ===========\n";
        setColor(7);
        cout << "1. View Available Doctors\n";
        cout << "2. Search Doctor by Specialization\n";
        cout << "3. Book Appointment\n";
        cout << "4. Cancel Appointment\n";
        cout << "5. Update Profile\n";
        cout << "6. Check Appointment Status\n";
        cout << "7. Print Appointment Slip\n";
        cout << "0. Logout\n";
        cout << "====================================\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            for (Doctor d : doctors) {
                cout << "\nID: " << d.id
                     << "\nName: " << d.name
                     << "\nSpecialization: " << d.specialization
                     << "\nTime Slot: " << d.timeSlot
                     << "\nContact: " << d.contact
                     << "\n----------------------";
            }
        } else if (choice == 2) {
            string spec;
            cout << "\nEnter specialization: ";
            getline(cin, spec);
            for (Doctor d : doctors) {
                if (d.specialization.find(spec) != string::npos) {
                    cout << "\nID: " << d.id
                         << "\nName: " << d.name
                         << "\nSpecialization: " << d.specialization
                         << "\n----------------------";
                }
            }
        } else if (choice == 3) {
            patientBookAppointment(patientId);
        } else if (choice == 4) {
            int aid;
            cout << "\nEnter Appointment ID to cancel: "; cin >> aid;
            bool found = false;
            for (auto it = appointments.begin(); it != appointments.end(); ++it) {
                if (it->id == aid && it->patientId == patientId) {
                    appointments.erase(it);
                    saveAppointments();
                    setColor(10); cout << "\n✅ Appointment cancelled.\n"; setColor(7);
                    found = true;
                    break;
                }
            }
            if (!found) {
                setColor(4); cout << "\n⚠️ Appointment not found.\n"; setColor(7);
            }
        } else if (choice == 5) {
            for (auto &p : patients) {
                if (p.id == patientId) {
                    cout << "\nEnter new name: "; getline(cin, p.name);
                    cout << "Enter new age: "; cin >> p.age; cin.ignore();
                    cout << "Enter new contact: "; getline(cin, p.contact);
                    savePatients();
                    setColor(10); cout << "\n✅ Profile updated.\n"; setColor(7);
                    break;
                }
            }
        } else if (choice == 6) {
            bool found = false;
            for (auto &a : appointments) {
                if (a.patientId == patientId) {
                    Doctor* d = findDoctor(a.doctorId);
                    cout << "\nAppointment ID: " << a.id
                         << "\nDoctor: " << (d ? d->name : "Unknown")
                         << "\nDate: " << a.date
                         << "\nTime: " << a.slot
                         << "\n----------------------";
                    found = true;
                }
            }
            if (!found) {
                setColor(4); cout << "\n⚠️ No appointments found.\n"; setColor(7);
            }
        } else if (choice == 7) {
            for (auto &a : appointments) {
                if (a.patientId == patientId) {
                    Doctor* d = findDoctor(a.doctorId);
                    cout << "\n----- Appointment Slip -----\n";
                    cout << "Appointment ID: " << a.id << "\n";
                    cout << "Doctor: " << (d ? d->name : "Unknown") << "\n";
                    cout << "Date: " << a.date << "\n";
                    cout << "Time: " << a.slot << "\n";
                    cout << "----------------------------\n";
                }
            }
        }
    } while (choice != 0);
}

// -------------------- Patient Book Appointment --------------------
void patientBookAppointment(int patientId) {
    Appointment a;
    cout << "\nEnter Appointment ID: "; cin >> a.id;
    cin.ignore();
    cout << "Enter Doctor Name: ";
    string docName; getline(cin, docName);

    Doctor* d = findDoctorByName(docName);
    if (!d) {
        setColor(4); cout << "\n⚠️ Doctor not found.\n"; setColor(7);
        return;
    }
    a.doctorId = d->id;
    a.patientId = patientId;
    cout << "Choose Time Slot (e.g., " << d->timeSlot << "): "; getline(cin, a.slot);
    cout << "Enter Date (YYYY-MM-DD): "; getline(cin, a.date);

    for (auto& ap : appointments) {
        if (ap.doctorId == a.doctorId && ap.date == a.date && ap.slot == a.slot) {
            setColor(4); cout << "\n⚠️ Time slot already booked.\n"; setColor(7);
            return;
        }
    }

    appointments.push_back(a);
    saveAppointments();

    setColor(10);
    cout << "\n✅ Appointment booked successfully!\n";
    cout << "Doctor: " << d->name << "\n";
    cout << "Date  : " << a.date << "\n";
    cout << "Time  : " << a.slot << "\n";
    setColor(7);
}

// -------------------- Login Function --------------------
bool login(string& role) {
    string username, password;
    cout << "\nLOGIN\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "admin123") {
        role = "admin";
        return true;
    } else if (username == "recept" && password == "recept123") {
        role = "receptionist";
        return true;
    } else if (username == "patient" && password == "patient123") {
        role = "patient";
        return true;
    } else {
        setColor(4);
        cout << "\nInvalid credentials!\n";
        setColor(7);
        return false;
    }
}// -------------------- Admin Menu --------------------
void adminMenu() {
    int choice;
    do {
        setColor(11);
        cout << "\n=========== ADMIN MENU ===========\n";
        setColor(7);
        cout << "1. Add Doctor\n2. View Doctors\n3. Search Doctor\n4. Update Doctor\n5. Delete Doctor\n6. View Appointments by Date\n7. Doctor-wise Appointments\n8. Patient Appointment History\n0. Logout\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Doctor d;
            cout << "\nEnter Doctor ID: "; cin >> d.id; cin.ignore();
            cout << "Enter Name: "; getline(cin, d.name);
            cout << "Enter Specialization: "; getline(cin, d.specialization);
            cout << "Enter Time Slot: "; getline(cin, d.timeSlot);
            cout << "Enter Contact: "; getline(cin, d.contact);

            if (findDoctor(d.id)) {
                setColor(4); cout << "\n⚠️ Doctor ID already exists.\n"; setColor(7);
            } else {
                doctors.push_back(d);
                saveDoctors();
                setColor(10); cout << "\n✅ Doctor added successfully!\n"; setColor(7);
            }
        } else if (choice == 2) {
            for (auto d : doctors) {
                cout << "\nID: " << d.id
                     << "\nName: " << d.name
                     << "\nSpecialization: " << d.specialization
                     << "\nTime Slot: " << d.timeSlot
                     << "\nContact: " << d.contact
                     << "\n--------------------------";
            }
        } else if (choice == 3) {
            string query;
            cout << "\nEnter name or specialization to search: ";
            getline(cin, query);
            bool found = false;
            for (auto d : doctors) {
                if (d.name.find(query) != string::npos || d.specialization.find(query) != string::npos) {
                    cout << "\nID: " << d.id
                         << "\nName: " << d.name
                         << "\nSpecialization: " << d.specialization
                         << "\n--------------------------";
                    found = true;
                }
            }
            if (!found) {
                setColor(4); cout << "\n⚠️ No matching doctor found.\n"; setColor(7);
            }
        } else if (choice == 4) {
            int id;
            cout << "\nEnter Doctor ID to update: "; cin >> id; cin.ignore();
            Doctor* d = findDoctor(id);
            if (d) {
                cout << "Enter new Name: "; getline(cin, d->name);
                cout << "Enter new Specialization: "; getline(cin, d->specialization);
                cout << "Enter new Time Slot: "; getline(cin, d->timeSlot);
                cout << "Enter new Contact: "; getline(cin, d->contact);
                saveDoctors();
                setColor(10); cout << "\n✅ Doctor updated.\n"; setColor(7);
            } else {
                setColor(4); cout << "\n⚠️ Doctor not found.\n"; setColor(7);
            }
        } else if (choice == 5) {
            int id;
            cout << "\nEnter Doctor ID to delete: "; cin >> id; cin.ignore();
            bool removed = false;
            for (size_t i = 0; i < doctors.size(); ++i) {
                if (doctors[i].id == id) {
                    doctors.erase(doctors.begin() + i);
                    saveDoctors();
                    setColor(10); cout << "\n✅ Doctor deleted.\n"; setColor(7);
                    removed = true;
                    break;
                }
            }
            if (!removed) {
                setColor(4); cout << "\n⚠️ Doctor not found.\n"; setColor(7);
            }
        } else if (choice == 6) {
            string date;
            cout << "\nEnter date (YYYY-MM-DD): "; getline(cin, date);
            for (auto a : appointments) {
                if (a.date == date) {
                    Doctor* d = findDoctor(a.doctorId);
                    Patient* p = findPatient(a.patientId);
                    cout << "\nAppointment ID: " << a.id
                         << "\nDoctor: " << (d ? d->name : "Unknown")
                         << "\nPatient: " << (p ? p->name : "Unknown")
                         << "\nSlot: " << a.slot << "\n--------------------------";
                }
            }
        } else if (choice == 7) {
            int did;
            cout << "\nEnter Doctor ID: "; cin >> did; cin.ignore();
            for (auto a : appointments) {
                if (a.doctorId == did) {
                    Patient* p = findPatient(a.patientId);
                    cout << "\nAppointment ID: " << a.id
                         << "\nPatient: " << (p ? p->name : "Unknown")
                         << "\nDate: " << a.date
                         << "\nSlot: " << a.slot
                         << "\n--------------------------";
                }
            }
        } else if (choice == 8) {
            int pid;
            cout << "\nEnter Patient ID: "; cin >> pid; cin.ignore();
            for (auto a : appointments) {
                if (a.patientId == pid) {
                    Doctor* d = findDoctor(a.doctorId);
                    cout << "\nAppointment ID: " << a.id
                         << "\nDoctor: " << (d ? d->name : "Unknown")
                         << "\nDate: " << a.date
                         << "\nSlot: " << a.slot
                         << "\n--------------------------";
                }
            }
        }
    } while (choice != 0);
}


// -------------------- Main Function --------------------
int main() {
    loadDoctors();
    loadPatients();
    loadAppointments();

    string role;
    int mainMenuOption;

    do {
        setColor(11);
        cout << "\n==== CLINIC MANAGEMENT SYSTEM ====\n";
        setColor(7);
        cout << "1. Login\n2. Register as Patient\n0. Exit\nChoice: ";
        cin >> mainMenuOption;

        if (mainMenuOption == 2) {
            Patient p;
            cout << "\nRegister New Patient";
            cout << "\nEnter Patient ID: "; cin >> p.id;
            cin.ignore();
            cout << "Enter Name: "; getline(cin, p.name);
            cout << "Enter Age: "; cin >> p.age;
            cin.ignore();
            cout << "Enter Contact: "; getline(cin, p.contact);

            if (findPatient(p.id)) {
                setColor(4); cout << "\n⚠️ Patient ID already exists.\n"; setColor(7);
            } else {
                patients.push_back(p);
                savePatients();
                setColor(10); cout << "\n✅ Registration successful! You can now log in using patient ID: " << p.id << "\n"; setColor(7);
            }
            continue;
        }

        if (mainMenuOption == 1 && login(role)) {
            if (role == "admin") {
                adminMenu();  // <== This triggers the admin menu
            } else if (role == "receptionist") {
                setColor(10); cout << "\n(Receptionist Menu Placeholder)\n"; setColor(7);
            } else if (role == "patient") {
                int pid;
                cout << "Enter your Patient ID: ";
                cin >> pid;
                if (findPatient(pid)) {
                    patientMenu(pid);
                } else {
                    setColor(4); cout << "⚠️ Patient not found.\n"; setColor(7);
                }
            }
        }
    } while (mainMenuOption != 0);

    saveDoctors();
    savePatients();
    saveAppointments();
    return 0;
}