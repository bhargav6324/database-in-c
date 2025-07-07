#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

struct Student {
    int id;
    char name[MAX_NAME_LEN];
    float cgpa;
};

void insert_record(int id, char *name, float cgpa) {
    FILE *fp = fopen("students.db", "ab");  // append binary
    if (fp == NULL) {
        perror("File opening failed");
        return;
    }

    struct Student s;
    s.id = id;
    strncpy(s.name, name, MAX_NAME_LEN);
    s.name[MAX_NAME_LEN - 1] = '\0';  // ensure null-termination
    s.cgpa = cgpa;

    fwrite(&s, sizeof(struct Student), 1, fp);

    fclose(fp);
    printf("✅ Record inserted successfully!\n");
}

void view_all_records() {
    FILE *fp = fopen("students.db", "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    struct Student s;
    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        printf("ID: %d | Name: %s | CGPA: %.2f\n", s.id, s.name, s.cgpa);
    }

    fclose(fp);
}


int main() {
    char command[100];
    printf("MiniDB Started. Type commands:\n");

   while (1) {
    printf(">> ");
    fgets(command, sizeof(command), stdin);

    if (strncmp(command, "EXIT", 4) == 0) {
        printf("👋 Exiting MiniDB.\n");
        break;
    }
    else if (strncmp(command, "INSERT", 6) == 0) {
        int id;
        char name[MAX_NAME_LEN];
        float cgpa;

        int parsed = sscanf(command, "INSERT %d %s %f", &id, name, &cgpa);
        if (parsed == 3) {
            insert_record(id, name, cgpa);
        } else {
            printf("❌ Invalid syntax! Use: INSERT <id> <name> <cgpa>\n");
        }
    }
    else if (strncmp(command, "VIEW", 4) == 0) {
        view_all_records();
    }
    else {
        printf("❌ Unknown command. Use: INSERT, VIEW or EXIT\n");
    }
}


    return 0;
}
