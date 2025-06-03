#include "student.h"
#include <string.h>

// Hàm thêm sở thích
int student_add_hobby(Student* s, const char* hobby) {
    if (s->hobbies_count >= MAX_HOBBIES) return 0;
    for (int i = 0; i < s->hobbies_count; i++)
        if (strcmp(s->hobbies[i], hobby) == 0) return 1;
    strcpy(s->hobbies[s->hobbies_count++], hobby);
    return 1;
}

// Hàm thêm thói quen
int student_add_habit(Student* s, const char* habit) {
    if (s->habits_count >= MAX_HABITS) return 0;
    for (int i = 0; i < s->habits_count; i++)
        if (strcmp(s->habits[i], habit) == 0) return 1;
    strcpy(s->habits[s->habits_count++], habit);
    return 1;
}

// Hàm thêm bạn bè
int student_add_friend(Student* s, const char* friend_id) {
    if (s->friends_count >= MAX_FRIENDS) return 0;
    if (strcmp(s->id, friend_id) == 0) return 0; // không tự kết bạn với chính mình
    for (int i = 0; i < s->friends_count; i++)
        if (strcmp(s->friends[i], friend_id) == 0) return 1;
    strcpy(s->friends[s->friends_count++], friend_id);
    return 1;
}

// Hàm xóa bạn bè
int student_remove_friend(Student* s, const char* friend_id) {
    for (int i = 0; i < s->friends_count; i++) {
        if (strcmp(s->friends[i], friend_id) == 0) {
            for (int j = i; j < s->friends_count - 1; j++)
                strcpy(s->friends[j], s->friends[j + 1]);
            s->friends_count--;
            return 1;
        }
    }
    return 0;
}

// Kiểm tra có phải bạn bè
int student_is_friend(const Student* s, const char* friend_id) {
    for (int i = 0; i < s->friends_count; i++) {
        if (strcmp(s->friends[i], friend_id) == 0) return 1;
    }
    return 0;
}
