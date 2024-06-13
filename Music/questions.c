#include "questions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int loadQuestions(struct Question* questions) {
    FILE* file = fopen("questions.txt", "r");
    if (!file) {
        printf("Error opening questions file.\n");
        return 0;
    }

    int count = 0;
    while (fscanf_s(file, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n",
        questions[count].question, (unsigned int)sizeof(questions[count].question),
        questions[count].correctAnswer, (unsigned int)sizeof(questions[count].correctAnswer),
        questions[count].option2, (unsigned int)sizeof(questions[count].option2),
        questions[count].option3, (unsigned int)sizeof(questions[count].option3),
        questions[count].option4, (unsigned int)sizeof(questions[count].option4)) == 5) {
        count++;
    }

    fclose(file);
    return count;
}
