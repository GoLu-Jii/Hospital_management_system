#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==== DATA STRUCTURES ====

// --- Patient (minimal: ID, name, gender) ---
typedef struct {
    int id;
    char name[50];
    char gender[10];
} Patient;

// --- Doctor (minimal: ID, name, gender) ---
typedef struct {
    int id;
    char name[50];
    char gender[10];
} Doctor;

// --- Appointment (links patient & doctor, date, time) ---
typedef struct {
    int id;
    int patientId;
    int doctorId;
    char date[12]; // YYYY-MM-DD
    char time[6];  // HH:MM
} Appointment;

// --- Bill (minimal: ID, patientId, doctorId, amount, date) ---
typedef struct {
    int id;
    int patientId;
    int doctorId;
    float amount;
    char date[12]; // YYYY-MM-DD
} Bill;

// ==== FILE DEFINITIONS ====
#define PATIENT_FILE      "patients_min.dat"
#define DOCTOR_FILE       "doctors_min.dat"
#define APPOINTMENT_FILE  "appointments_min.dat"
#define BILL_FILE         "bills_min.dat"

// ==== PATIENT MANAGEMENT ====

// Unique ID generator for patient
int generatePatientId() {
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) return 1;
    fseek(fp, 0, SEEK_END);
    int id = (ftell(fp) / sizeof(Patient)) + 1;
    fclose(fp);
    return id;
}

void addPatient() {
    FILE *fp = fopen(PATIENT_FILE, "ab");
    if (!fp) return;
    Patient p;
    p.id = generatePatientId();
    printf("Patient Name: "); fgets(p.name, 50, stdin); p.name[strcspn(p.name,"\n")]=0;
    printf("Gender: "); fgets(p.gender, 10, stdin); p.gender[strcspn(p.gender,"\n")]=0;
    fwrite(&p, sizeof(Patient), 1, fp);
    fclose(fp);
    printf("Patient added with ID: %d\n", p.id);
}

void listPatients() {
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) { printf("No patients.\n"); return; }
    Patient p;
    printf("ID\tName\t\tGender\n");
    while (fread(&p, sizeof(Patient), 1, fp))
        printf("%d\t%-15s\t%s\n", p.id, p.name, p.gender);
    fclose(fp);
}

void searchPatients() {
    char search[50];
    printf("Patient Name to search: "); fgets(search, 50, stdin); search[strcspn(search,"\n")]=0;
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) { printf("No patients.\n"); return; }
    Patient p; int found=0;
    while (fread(&p, sizeof(Patient), 1, fp))
        if (strstr(p.name, search)) {
            printf("ID: %d | Name: %s | Gender: %s\n", p.id, p.name, p.gender);
            found=1;
        }
    if (!found) printf("No match found.\n");
    fclose(fp);
}

Patient* findPatientById(int id) {
    static Patient p;
    FILE *fp = fopen(PATIENT_FILE, "rb");
    if (!fp) return NULL;
    while (fread(&p, sizeof(Patient), 1, fp)) {
        if (p.id == id) { fclose(fp); return &p; }
    }
    fclose(fp);
    return NULL;
}

// ==== DOCTOR MANAGEMENT ====

int generateDoctorId() {
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) return 1;
    fseek(fp, 0, SEEK_END);
    int id = (ftell(fp) / sizeof(Doctor)) + 1;
    fclose(fp);
    return id;
}

void addDoctor() {
    FILE *fp = fopen(DOCTOR_FILE, "ab");
    if (!fp) return;
    Doctor d;
    d.id = generateDoctorId();
    printf("Doctor Name: "); fgets(d.name, 50, stdin); d.name[strcspn(d.name,"\n")]=0;
    printf("Gender: "); fgets(d.gender, 10, stdin); d.gender[strcspn(d.gender,"\n")]=0;
    fwrite(&d, sizeof(Doctor), 1, fp);
    fclose(fp);
    printf("Doctor added with ID: %d\n", d.id);
}

void listDoctors() {
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) { printf("No doctors.\n"); return; }
    Doctor d;
    printf("ID\tName\t\tGender\n");
    while (fread(&d, sizeof(Doctor), 1, fp))
        printf("%d\t%-15s\t%s\n", d.id, d.name, d.gender);
    fclose(fp);
}

void searchDoctors() {
    char search[50];
    printf("Doctor Name to search: "); fgets(search, 50, stdin); search[strcspn(search,"\n")]=0;
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) { printf("No doctors.\n"); return; }
    Doctor d; int found=0;
    while (fread(&d, sizeof(Doctor), 1, fp))
        if (strstr(d.name, search)) {
            printf("ID: %d | Name: %s | Gender: %s\n", d.id, d.name, d.gender);
            found=1;
        }
    if (!found) printf("No match found.\n");
    fclose(fp);
}

Doctor* findDoctorById(int id) {
    static Doctor d;
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    if (!fp) return NULL;
    while (fread(&d, sizeof(Doctor), 1, fp)) {
        if (d.id == id) { fclose(fp); return &d; }
    }
    fclose(fp);
    return NULL;
}

// ==== APPOINTMENT MANAGEMENT ====

int generateAppointmentId() {
    FILE *fp = fopen(APPOINTMENT_FILE, "rb");
    if (!fp) return 1;
    fseek(fp, 0, SEEK_END);
    int id = (ftell(fp) / sizeof(Appointment)) + 1;
    fclose(fp);
    return id;
}

void scheduleAppointment() {
    FILE *fp = fopen(APPOINTMENT_FILE, "ab");
    if (!fp) return;
    Appointment a;
    a.id = generateAppointmentId();
    printf("Patient ID: "); scanf("%d", &a.patientId); getchar();
    if (!findPatientById(a.patientId)) { printf("Patient not found.\n"); fclose(fp); return; }
    printf("Doctor ID: "); scanf("%d", &a.doctorId); getchar();
    if (!findDoctorById(a.doctorId)) { printf("Doctor not found.\n"); fclose(fp); return; }
    printf("Date (YYYY-MM-DD): "); fgets(a.date, 12, stdin); a.date[strcspn(a.date,"\n")]=0;
    printf("Time (HH:MM): "); fgets(a.time, 6, stdin); a.time[strcspn(a.time,"\n")]=0;
    fwrite(&a, sizeof(Appointment), 1, fp);
    fclose(fp);
    printf("Appointment scheduled with ID: %d\n", a.id);
}

void listAppointments() {
    FILE *fp = fopen(APPOINTMENT_FILE, "rb");
    if (!fp) { printf("No appointments.\n"); return; }
    Appointment a;
    printf("ID\tPatient\t\tDoctor\t\tDate\t\tTime\n");
    while (fread(&a, sizeof(Appointment), 1, fp)) {
        Patient* p = findPatientById(a.patientId);
        Doctor* d = findDoctorById(a.doctorId);
        printf("%d\t%-10s\t%-10s\t%s\t%s\n", a.id,
            p ? p->name : "Unknown",
            d ? d->name : "Unknown",
            a.date, a.time);
    }
    fclose(fp);
}

// ==== BILLING MANAGEMENT ====

int generateBillId() {
    FILE *fp = fopen(BILL_FILE, "rb");
    if (!fp) return 1;
    fseek(fp, 0, SEEK_END);
    int id = (ftell(fp) / sizeof(Bill)) + 1;
    fclose(fp);
    return id;
}

void addBill() {
    FILE *fp = fopen(BILL_FILE, "ab");
    if (!fp) return;
    Bill b;
    b.id = generateBillId();
    printf("Patient ID: "); scanf("%d", &b.patientId); getchar();
    if (!findPatientById(b.patientId)) { printf("Patient not found.\n"); fclose(fp); return; }
    printf("Doctor ID: "); scanf("%d", &b.doctorId); getchar();
    if (!findDoctorById(b.doctorId)) { printf("Doctor not found.\n"); fclose(fp); return; }
    printf("Amount: "); scanf("%f", &b.amount); getchar();
    printf("Date (YYYY-MM-DD): "); fgets(b.date, 12, stdin); b.date[strcspn(b.date,"\n")]=0;
    fwrite(&b, sizeof(Bill), 1, fp);
    fclose(fp);
    printf("Bill generated with ID: %d\n", b.id);
}

void listBills() {
    FILE *fp = fopen(BILL_FILE, "rb");
    if (!fp) { printf("No bills.\n"); return; }
    Bill b;
    printf("ID\tPatient\t\tDoctor\t\tAmount\tDate\n");
    while (fread(&b, sizeof(Bill), 1, fp)) {
        Patient* p = findPatientById(b.patientId);
        Doctor* d = findDoctorById(b.doctorId);
        printf("%d\t%-10s\t%-10s\t%.2f\t%s\n", b.id,
            p ? p->name : "Unknown",
            d ? d->name : "Unknown",
            b.amount, b.date);
    }
    fclose(fp);
}

// ==== MAIN MENU ====

void mainMenu() {
    int choice;
    while (1) {
        printf("\n===== Hospital Management System (Minimal) =====\n");
        printf("PATIENT MANAGEMENT:\n");
        printf("1. Register Patient\n");
        printf("2. List Patients\n");
        printf("3. Search Patients\n");
        printf("\nDOCTOR MANAGEMENT:\n");
        printf("4. Add Doctor\n");
        printf("5. List Doctors\n");
        printf("6. Search Doctors\n");
        printf("\nAPPOINTMENTS:\n");
        printf("7. Schedule Appointment\n");
        printf("8. List Appointments\n");
        printf("\nBILLING:\n");
        printf("9. Generate Bill\n");
        printf("10. List Bills\n");
        printf("\n0. Exit\n");
        printf("===============================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice); getchar();

        switch (choice) {
            case 1: addPatient(); break;
            case 2: listPatients(); break;
            case 3: searchPatients(); break;
            case 4: addDoctor(); break;
            case 5: listDoctors(); break;
            case 6: searchDoctors(); break;
            case 7: scheduleAppointment(); break;
            case 8: listAppointments(); break;
            case 9: addBill(); break;
            case 10: listBills(); break;
            case 0: printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice, try again.\n");
        }
    }
}

// ==== MAIN FUNCTION ====

int main() {
    printf("Welcome to Hospital Management System (Minimal)\n");
    mainMenu();
    return 0;
}