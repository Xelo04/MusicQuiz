#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gameLogic.h"
#include "structs.h"
#include "questions.h"

void shuffle(char* array[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void drawText(sfRenderWindow* window, sfText* text, const char* string, float x, float y, sfFont* font, unsigned int size) {
    sfText_setString(text, string);
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, size);
    sfText_setPosition(text, (sfVector2f) { x, y });
    sfRenderWindow_drawText(window, text, NULL);
}

void askQuestion(sfRenderWindow* window, struct Player* player, struct Question* question) {
    char* options[4] = { question->correctAnswer, question->option2, question->option3, question->option4 };
    shuffle(options, 4);

    sfFont* font = sfFont_createFromFile("fonts/arial.ttf");
    sfText* questionText = sfText_create();
    sfText* answerTexts[4];
    sfText* playerNameText = sfText_create();
    sfText* livesText = sfText_create();
    sfText* scoreText = sfText_create();

    sfText_setString(questionText, question->question);
    sfText_setFont(questionText, font);
    sfText_setCharacterSize(questionText, 40);
    sfFloatRect textBounds = sfText_getGlobalBounds(questionText);
    float questionX = 500 - textBounds.width / 2;
    drawText(window, questionText, question->question, questionX, 120, font, 30);

    for (int i = 0; i < 4; ++i) {
        answerTexts[i] = sfText_create();
        sfText_setString(answerTexts[i], options[i]);
        sfText_setFont(answerTexts[i], font);
        sfText_setCharacterSize(answerTexts[i], 30);
        sfFloatRect answerBounds = sfText_getGlobalBounds(answerTexts[i]);
        float answerX = 400 - answerBounds.width / 2;
        drawText(window, answerTexts[i], options[i], answerX, 180 + (float)i * 50.0f, font, 30);
    }

    char livesStr[20];
    char scoreStr[20];

    sfTexture* backgroundTexture = sfTexture_createFromFile("background.jpeg", NULL);
    if (!backgroundTexture) {
        fprintf(stderr, "Failed to load background image.\n");
        return;
    }
    sfSprite* backgroundSprite = sfSprite_create();
    sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);

    while (sfRenderWindow_isOpen(window) && player->lives > 0) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            else if (event.type == sfEvtMouseButtonPressed) {
                for (int i = 0; i < 4; ++i) {
                    sfFloatRect textBounds = sfText_getGlobalBounds(answerTexts[i]);
                    if (sfFloatRect_contains(&textBounds, (float)event.mouseButton.x, (float)event.mouseButton.y)) {
                        if (strcmp(options[i], question->correctAnswer) == 0) {
                            player->score += 10;
                            printf("Correct answer\n");
                        }
                        else {
                            printf("Incorrect answer. The correct answer is: %s\n", question->correctAnswer);
                            player->lives -= 1;
                        }
                        sfSprite_destroy(backgroundSprite);
                        sfTexture_destroy(backgroundTexture);
                        return;
                    }
                }
            }
        }

        snprintf(livesStr, sizeof(livesStr), "Lives: %d", player->lives);
        snprintf(scoreStr, sizeof(scoreStr), "Score: %d", player->score);

        drawText(window, playerNameText, player->username, 10, 10, font, 20);
        drawText(window, livesText, livesStr, 10, 40, font, 20);

        sfFloatRect scoreBounds = sfText_getGlobalBounds(scoreText);
        float scoreX = 800 - scoreBounds.width - 10;
        drawText(window, scoreText, scoreStr, scoreX, 10, font, 20);

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        sfRenderWindow_drawText(window, questionText, NULL);
        for (int i = 0; i < 4; ++i) {
            sfRenderWindow_drawText(window, answerTexts[i], NULL);
        }
        sfRenderWindow_drawText(window, playerNameText, NULL);
        sfRenderWindow_drawText(window, livesText, NULL);
        sfRenderWindow_drawText(window, scoreText, NULL);
        sfRenderWindow_display(window);
    }

    sfText_destroy(questionText);
    for (int i = 0; i < 4; ++i) {
        sfText_destroy(answerTexts[i]);
    }
    sfText_destroy(playerNameText);
    sfText_destroy(livesText);
    sfText_destroy(scoreText);
    sfFont_destroy(font);
    sfSprite_destroy(backgroundSprite);
    sfTexture_destroy(backgroundTexture);
}

void savePlayerResults(struct Player* player) {
    FILE* file = fopen("leaderboard.txt", "a");
    if (file) {
        fprintf(file, "%s %d\n", player->username, player->score);
        fclose(file);
    }
}

void startGame(sfRenderWindow* window, struct Player* player) {
    struct Question questions[100];
    int questionCount = loadQuestions(questions);

    srand((unsigned int)time(NULL));

    while (player->lives > 0) {
        int questionIndex = rand() % questionCount;
        askQuestion(window, player, &questions[questionIndex]);
    }

    printf("Game over, your score is %d\n", player->score);
    savePlayerResults(player);
}