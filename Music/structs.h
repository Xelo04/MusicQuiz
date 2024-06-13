#ifndef STRUCTS_H
#define STRUCTS_H

struct Player {
    char username[50];
    int score;
    int lives;
};

struct Question {
    char question[256];
    char correctAnswer[256];
    char option2[256];
    char option3[256];
    char option4[256];
};

#endif
