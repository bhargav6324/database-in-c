#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

struct Student {
    int id;
    char name[MAX_NAME_LEN];
    float cgpa;
    int valid;  // 1 = active, 0 = deleted
};

// INSERT a new student record
void insert_record(int id, char *name, float cgpa) {
    FILE *fp = fopen("students.db", "ab");
    if (fp == NULL) {
        perror("File open failed");
        return;
    }

    struct Student s;
    s.id = id;
    strncpy(s.name, name, MAX_NAME_LEN);
    s.name[MAX_NAME_LEN - 1] = '\0';
    s.cgpa = cgpa;
    s.valid = 1;

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("✅ Record inserted successfully!\n");
}

// VIEW all valid records
void view_all_records() {
    FILE *fp = fopen("students.db", "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    struct Student s;
    printf("\n--- Student Records ---\n");
    int any = 0;

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.valid) {
            printf("ID: %d | Name: %s | CGPA: %.2f\n", s.id, s.name, s.cgpa);
            any = 1;
        }
    }

    fclose(fp);
    if (!any) {
        printf("⚠️  No valid records to display.\n");
    }
}

// SELECT record by ID if valid
void select_where_id(int search_id) {
    FILE *fp = fopen("students.db", "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    struct Student s;
    int found = 0;

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.valid && s.id == search_id) {
            printf("✅ Record Found:\n");
            printf("ID: %d | Name: %s | CGPA: %.2f\n", s.id, s.name, s.cgpa);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("❌ Record with ID %d not found.\n", search_id);
    }
}

// DELETE record by ID (soft delete)
void delete_where_id(int target_id) {
    FILE *fp = fopen("students.db", "r+b");
    if (fp == NULL) {
        printf("❌ Could not open file.\n");
        return;
    }

    struct Student s;
    int found = 0;

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.valid && s.id == target_id) {
            s.valid = 0;

            // Move back to overwrite
            fseek(fp, -sizeof(struct Student), SEEK_CUR);
            fwrite(&s, sizeof(struct Student), 1, fp);

            printf("🗑️  Record with ID %d marked as deleted.\n", target_id);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("❌ Record with ID %d not found.\n", target_id);
    }
}

int main() {
    char command[100];
    printf("📦 MiniDB Started. Type commands:\n");
    printf("   - INSERT <id> <name> <cgpa>\n");
    printf("   - SELECT WHERE id = <value>\n");
    printf("   - DELETE WHERE id = <value>\n");
    printf("   - VIEW\n");
    printf("   - EXIT\n");

    while (1) {
        printf(">> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;  // Remove newline

        int id;
        char name[MAX_NAME_LEN];
        float cgpa;

        if (sscanf(command, "INSERT %d %s %f", &id, name, &cgpa) == 3) {
            insert_record(id, name, cgpa);
        }
        else if (strcmp(command, "VIEW") == 0) {
            view_all_records();
        }
        else if (sscanf(command, "SELECT WHERE id = %d", &id) == 1) {
            select_where_id(id);
        }
        else if (sscanf(command, "DELETE WHERE id = %d", &id) == 1) {
            delete_where_id(id);
        }
        else if (strcmp(command, "EXIT") == 0) {
            printf("👋 Exiting MiniDB.\n");
            break;
        }
        else {
            printf("❌ Unknown command. Use: INSERT, VIEW, SELECT, DELETE, or EXIT\n");
        }
    }

    return 0;
}
