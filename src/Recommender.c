#include "recommender.h"
#include <stdio.h>
#include <string.h>

#define MAX_LEN 50

double jaccard_similarity(const char arr1[][MAX_LEN], int count1, const char arr2[][MAX_LEN], int count2) {
    int intersection = 0;
    for (int i = 0; i < count1; i++) {
        for (int j = 0; j < count2; j++) {
            if (strcmp(arr1[i], arr2[j]) == 0) {
                intersection++;
            }
        }
    }
    int union_count = count1 + count2 - intersection;
    if (union_count == 0) return 0.0;
    return (double)intersection / union_count;
}

void recommend_friends(Graph* g, const char* user_id, int k, double weight_mutual, double weight_jaccard) {
    Student* user = graph_get_student(g, user_id);
    if (!user) {
        printf("Sinh vien khong ton tai.\n");
        return;
    }

    Candidate candidates[MAX_POTENTIAL_CANDIDATES];
    int candidate_count = 0;

    for (int i = 0; i < g->student_count; i++) {
        Student* candidate = &g->students[i];
        if (strcmp(user->id, candidate->id) == 0 || student_is_friend(user, candidate->id)) {
            continue;
        }

        double mutual_friends_score = 0;
        for (int j = 0; j < user->friends_count; j++) {
            if (student_is_friend(candidate, user->friends[j])) {
                mutual_friends_score++;
            }
        }

        double jaccard_hobbies = jaccard_similarity((const char (*)[MAX_LEN])user->hobbies, user->hobbies_count,
                                                    (const char (*)[MAX_LEN])candidate->hobbies, candidate->hobbies_count);
        double jaccard_habits = jaccard_similarity((const char (*)[MAX_LEN])user->habits, user->habits_count,
                                                   (const char (*)[MAX_LEN])candidate->habits, candidate->habits_count);

        double total_jaccard_score = (jaccard_hobbies + jaccard_habits) / 2.0;
        double final_score = (weight_mutual * mutual_friends_score) + (weight_jaccard * total_jaccard_score);

        if (final_score > 0 && candidate_count < MAX_POTENTIAL_CANDIDATES) {
            strcpy(candidates[candidate_count].id, candidate->id);
            candidates[candidate_count].score = final_score;
            candidate_count++;
        }
    }

    // Sắp xếp giảm dần theo điểm
    for (int i = 0; i < candidate_count - 1; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            if (candidates[i].score < candidates[j].score) {
                Candidate temp = candidates[i];
                candidates[i] = candidates[j];
                candidates[j] = temp;
            }
        }
    }

    // In kết quả có tên bạn bè
    printf("Goi y ban be cho %s (ID: %s):\n", user->name, user->id);
    for (int i = 0; i < k && i < candidate_count; i++) {
        Student* sug = graph_get_student(g, candidates[i].id);
        if (sug) {
            printf("%d. %s (ID: %s) - Diem: %.2f\n", i + 1, sug->name, sug->id, candidates[i].score);
        } else {
            // Dự phòng, nếu không tìm thấy
            printf("%d. %s (ID: %s) - Diem: %.2f\n", i + 1, candidates[i].id, candidates[i].id, candidates[i].score);
        }
    }
}
