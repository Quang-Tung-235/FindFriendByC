// cli.c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cli.h"
#include "graph.h"
#include "datatables.h"
#include "recommender.h"

#define MAX_INPUT_LEN 256

// Hàm kiểm tra khoảng trắng đầu chuỗi
static int has_leading_whitespace(const char* s) {
    if (!s || s[0] == '\0') return 0;
    return isspace((unsigned char)s[0]);
}

// Hàm tách chuỗi đơn giản dựa trên dấu phẩy
static void simple_split(char* input, char delimiter, char tokens[][MAX_NAME_LEN], int* count, int max_tokens) {
    *count = 0;
    char delim[2] = {delimiter, '\0'};
    char* token = strtok(input, delim);
    while (token && *count < max_tokens) {
        strncpy(tokens[(*count)++], token, MAX_NAME_LEN - 1);
        tokens[*count - 1][MAX_NAME_LEN - 1] = '\0';
        token = strtok(NULL, delim);
    }
}

static void add_student_flow(Graph* graph, HobbyTable* hobby_table, HabitTable* habit_table) {
    if (graph->student_count >= MAX_STUDENTS) {
        printf("He thong da day, khong the them sinh vien moi.\n");
        return;
    }

    char buffer[MAX_INPUT_LEN];
    Student s = {0};
    int valid_input = 0;

    printf("Nhap ID sinh vien: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    buffer[strcspn(buffer, "\r\n")] = 0;
    strncpy(s.id, buffer, MAX_ID_LEN - 1);

    if (graph_find_student_index(graph, s.id) != -1) {
        printf("LOI: ID da ton tai trong he thong.\n");
        return;
    }

    printf("Nhap ten sinh vien: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    buffer[strcspn(buffer, "\r\n")] = 0;
    strncpy(s.name, buffer, MAX_NAME_LEN - 1);

    do {
        printf("Nhap cac so thich (cach nhau dau phay ','): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (has_leading_whitespace(buffer)) {
            printf("LOI: Du lieu nhap khong duoc bat dau bang khoang trang, vui long nhap lai!\n");
            valid_input = 0;
        } else {
            valid_input = 1;
        }
    } while (!valid_input);

    char hobby_tokens[MAX_HOBBIES][MAX_NAME_LEN];
    int hobby_count = 0;
    simple_split(buffer, ',', hobby_tokens, &hobby_count, MAX_HOBBIES);

    for (int i = 0; i < hobby_count && s.hobbies_count < MAX_HOBBIES; i++) {
        int idx = hobbytable_add(hobby_table, hobby_tokens[i]);
        if (idx != -1) {
            student_add_hobby(&s, hobby_table->list[idx]);
        } else {
            printf("CANH BAO: Khong the them so thich '%s'.\n", hobby_tokens[i]);
        }
    }

    do {
        printf("Nhap cac thoi quen (cach nhau dau phay ','): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (has_leading_whitespace(buffer)) {
            printf("LOI: Du lieu nhap khong duoc bat dau bang khoang trang, vui long nhap lai!\n");
            valid_input = 0;
        } else {
            valid_input = 1;
        }
    } while (!valid_input);

    char habit_tokens[MAX_HABITS][MAX_NAME_LEN];
    int habit_count = 0;
    simple_split(buffer, ',', habit_tokens, &habit_count, MAX_HABITS);

    for (int i = 0; i < habit_count && s.habits_count < MAX_HABITS; i++) {
        int idx = habittable_add(habit_table, habit_tokens[i]);
        if (idx != -1) {
            student_add_habit(&s, habit_table->list[idx]);
        } else {
            printf("CANH BAO: Khong the them thoi quen '%s'.\n", habit_tokens[i]);
        }
    }

    if (graph_add_student(graph, &s)) {
        printf("Da them sinh vien '%s' (ID: %s) thanh cong!\n", s.name, s.id);
    } else {
        printf("LOI: Khong the them sinh vien.\n");
    }
}

static void add_friendship_flow(Graph* graph) {
    char id1[MAX_ID_LEN], id2[MAX_ID_LEN];
    printf("Nhap ID sinh vien thu nhat: ");
    if (!fgets(id1, sizeof(id1), stdin)) return;
    id1[strcspn(id1, "\r\n")] = 0;

    printf("Nhap ID sinh vien thu hai: ");
    if (!fgets(id2, sizeof(id2), stdin)) return;
    id2[strcspn(id2, "\r\n")] = 0;

    if (strcmp(id1, id2) == 0) {
        printf("LOI: Hai ID giong nhau, khong the ket ban voi chinh minh.\n");
        return;
    }

    if (graph_add_friendship(graph, id1, id2)) {
        printf("Da tao quan he ban be giua %s va %s thanh cong!\n", id1, id2);
    } else {
        printf("LOI: Khong the tao quan he ban be. Kiem tra lai ID hoac da la ban be.\n");
    }
}

static void suggest_friends_flow(Graph* graph) {
    char user_id[MAX_ID_LEN];
    printf("Nhap ID sinh vien can goi y ban be: ");
    if (!fgets(user_id, sizeof(user_id), stdin)) return;
    user_id[strcspn(user_id, "\r\n")] = 0;

    recommend_friends(graph, user_id, 5, 1.0, 0.5);
}

static void list_students_flow(Graph* graph) {
    if (graph->student_count == 0) {
        printf("Chua co sinh vien nao trong he thong.\n");
        return;
    }

    printf("ID         | Ten                 | So thich                   | Thoi quen                  | Ban be\n");
    printf("---------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < graph->student_count; i++) {
        Student* s = &graph->students[i];

        printf("%-10s | %-20s | ", s->id, s->name);

        for (int j = 0; j < s->hobbies_count; j++) {
            printf("%s", s->hobbies[j]);
            if (j < s->hobbies_count - 1) printf(", ");
        }
        if (s->hobbies_count == 0) printf("(khong co)");
        printf(" | ");

        for (int j = 0; j < s->habits_count; j++) {
            printf("%s", s->habits[j]);
            if (j < s->habits_count - 1) printf(", ");
        }
        if (s->habits_count == 0) printf("(khong co)");
        printf(" | ");

        for (int j = 0; j < s->friends_count; j++) {
            printf("%s", s->friends[j]);
            if (j < s->friends_count - 1) printf(", ");
        }
        if (s->friends_count == 0) printf("(chua co)");
        printf("\n");
    }
}

static void remove_friendship_flow(Graph* graph) {
    char id1[MAX_ID_LEN], id2[MAX_ID_LEN];
    printf("Nhap ID sinh vien thu nhat: ");
    if (!fgets(id1, sizeof(id1), stdin)) return;
    id1[strcspn(id1, "\r\n")] = 0;

    printf("Nhap ID sinh vien thu hai: ");
    if (!fgets(id2, sizeof(id2), stdin)) return;
    id2[strcspn(id2, "\r\n")] = 0;

    Student* s1 = graph_get_student(graph, id1);
    Student* s2 = graph_get_student(graph, id2);

    if (!s1 || !s2) {
        printf("LOI: Khong tim thay mot hoac ca hai sinh vien!\n");
        return;
    }

    printf("Ban co chac chan muon xoa quan he ban be giua:\n");
    printf("1. %s (ID: %s)\n", s1->name, id1);
    printf("2. %s (ID: %s)\n", s2->name, id2);
    printf("Nhap 'y' de xac nhan, bat ky phim nao de huy: ");

    char confirm[10];
    if (!fgets(confirm, sizeof(confirm), stdin)) return;

    if (tolower(confirm[0]) == 'y') {
        if (graph_remove_friendship(graph, id1, id2)) {
            printf("Da xoa quan he ban be thanh cong!\n");
        } else {
            printf("LOI: Hai sinh vien nay khong phai la ban be!\n");
        }
    } else {
        printf("Da huy thao tac xoa quan he ban be.\n");
    }
}

void cli_run(Graph* graph, HobbyTable* hobby_table, HabitTable* habit_table) {
    int choice = -1;
    char input[MAX_INPUT_LEN];

    while (1) {
        printf("\n===== HE THONG GOI Y BAN BE =====\n");
        printf("1. Them sinh vien moi\n");
        printf("2. Ket ban giua hai sinh vien\n");
        printf("3. Goi y ban be cho mot sinh vien\n");
        printf("4. Liet ke danh sach sinh vien\n");
        printf("5. Xoa quan he ban be\n");
        printf("0. Thoat chuong trinh\n");
        printf("Lua chon cua ban: ");

        if (!fgets(input, sizeof(input), stdin)) break;
        choice = atoi(input);

        switch (choice) {
            case 1:
                add_student_flow(graph, hobby_table, habit_table);
                break;
            case 2:
                add_friendship_flow(graph);
                break;
            case 3:
                suggest_friends_flow(graph);
                break;
            case 4:
                list_students_flow(graph);
                break;
            case 5:
                remove_friendship_flow(graph);
                break;
            case 0:
                printf("Cam on ban da su dung he thong. Tam biet!\n");
                return;
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
        }
    }
}
