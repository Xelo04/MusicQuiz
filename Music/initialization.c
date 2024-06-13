#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameLogic.h"
#include "structs.h"

#define MAX_PLAYERS 10

int getSavedPlayerCount() {
    FILE* file = fopen("leaderboard.txt", "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    fclose(file);
    return count;
}

void removeOldestPlayer() {
    FILE* file = fopen("leaderboard.txt", "r");
    FILE* tempFile = fopen("temp_leaderboard.txt", "w");
    if (!file || !tempFile) {
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (count != 0) {
            fputs(line, tempFile);
        }
        count++;
    }

    fclose(file);
    fclose(tempFile);

    remove("leaderboard.txt");
    rename("temp_leaderboard.txt", "leaderboard.txt");
}

void newGame(sfRenderWindow* window) {
    struct Player player;
    FILE* file = fopen("saved_game.txt", "w");
    if (file) {
        player.lives = 3;
        player.score = 0;

        sfFont* font = sfFont_createFromFile("fonts/arial.ttf");
        sfText* text = sfText_create();
        sfText* inputText = sfText_create();
        sfText_setFont(text, font);
        sfText_setCharacterSize(text, 40);

        sfText_setString(text, "Enter your username:");
        sfFloatRect textBounds = sfText_getGlobalBounds(text);
        sfText_setPosition(text, (sfVector2f) { 400 - textBounds.width / 2, 150 });

        char username[50] = "";
        size_t length = 0;

        sfText_setFont(inputText, font);
        sfText_setCharacterSize(inputText, 30);
        sfText_setPosition(inputText, (sfVector2f) { 400, 250 });

        sfTexture* backgroundTexture = sfTexture_createFromFile("background.jpeg", NULL);
        if (!backgroundTexture) {
            fprintf(stderr, "Failed to load background image.\n");
            return;
        }
        sfSprite* backgroundSprite = sfSprite_create();
        sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);

        while (sfRenderWindow_isOpen(window)) {
            sfEvent event;
            while (sfRenderWindow_pollEvent(window, &event)) {
                if (event.type == sfEvtClosed) {
                    sfRenderWindow_close(window);
                }
                else if (event.type == sfEvtTextEntered) {
                    if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        if (length < sizeof(username) - 1) {
                            username[length++] = (char)event.text.unicode;
                            username[length] = '\0';
                        }
                    }
                    else if (event.text.unicode == '\b' && length > 0) {
                        username[--length] = '\0';
                    }
                    sfText_setString(inputText, username);
                    sfFloatRect inputBounds = sfText_getGlobalBounds(inputText);
                    sfText_setPosition(inputText, (sfVector2f) { 400 - inputBounds.width / 2, 250 });
                }
                else if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter && length > 0) {
                    if (getSavedPlayerCount() >= MAX_PLAYERS) {
                        removeOldestPlayer();
                    }
                    strcpy_s(player.username, sizeof(player.username), username);
                    fprintf(file, "%s\n%d %d", player.username, player.score, player.lives);
                    fclose(file);
                    startGame(window, &player);
                    sfText_destroy(text);
                    sfText_destroy(inputText);
                    sfFont_destroy(font);
                    sfSprite_destroy(backgroundSprite);
                    sfTexture_destroy(backgroundTexture);
                    return;
                }
            }

            sfRenderWindow_clear(window, sfBlack);
            sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
            sfRenderWindow_drawText(window, text, NULL);
            sfRenderWindow_drawText(window, inputText, NULL);
            sfRenderWindow_display(window);
        }

        sfText_destroy(text);
        sfText_destroy(inputText);
        sfFont_destroy(font);
        sfSprite_destroy(backgroundSprite);
        sfTexture_destroy(backgroundTexture);
    }
}
