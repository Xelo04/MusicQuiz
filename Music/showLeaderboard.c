#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PlayerData {
    char username[50];
    int score;
};

int comparePlayers(const void* a, const void* b) {
    struct PlayerData* playerA = (struct PlayerData*)a;
    struct PlayerData* playerB = (struct PlayerData*)b;
    return playerB->score - playerA->score;
}

void showLeaderboard(sfRenderWindow* window) {
    sfFont* font = sfFont_createFromFile("fonts/arial.ttf");
    sfText* headerText = sfText_create();
    sfText* text = sfText_create();
    sfText_setFont(headerText, font);
    sfText_setFont(text, font);
    sfText_setCharacterSize(headerText, 40);
    sfText_setCharacterSize(text, 30);

    sfText_setString(headerText, "Leaderboard");
    sfFloatRect headerBounds = sfText_getGlobalBounds(headerText);
    sfText_setPosition(headerText, (sfVector2f) { 400 - headerBounds.width / 2, 10 });

    sfTexture* backgroundTexture = sfTexture_createFromFile("background2.jpeg", NULL);
    if (!backgroundTexture) {
        fprintf(stderr, "Failed to load background image.\n");
        return;
    }
    sfSprite* backgroundSprite = sfSprite_create();
    sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);

    FILE* file = fopen("leaderboard.txt", "r");
    if (file) {
        struct PlayerData* players = NULL;
        size_t playerCount = 0;
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            struct PlayerData player;
            sscanf_s(line, "%49s %d", player.username, (unsigned)_countof(player.username), &player.score);
            players = realloc(players, (playerCount + 1) * sizeof(struct PlayerData));
            players[playerCount] = player;
            playerCount++;
        }
        fclose(file);

        qsort(players, playerCount, sizeof(struct PlayerData), comparePlayers);

        int y = 60;
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        sfRenderWindow_drawText(window, headerText, NULL);
        for (size_t i = 0; i < playerCount; i++) {
            snprintf(line, sizeof(line), "%s %d", players[i].username, players[i].score);
            sfText_setString(text, line);
            sfFloatRect textBounds = sfText_getGlobalBounds(text);
            sfText_setPosition(text, (sfVector2f) { 400 - textBounds.width / 2, (float)y });
            y += 40;
            sfRenderWindow_drawText(window, text, NULL);
        }
        free(players);
        sfRenderWindow_display(window);
        sfSleep(sfSeconds(5));
    }
    else {
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        sfRenderWindow_drawText(window, headerText, NULL);
        sfText_setString(text, "No leaderboard data available.");
        sfFloatRect textBounds = sfText_getGlobalBounds(text);
        sfText_setPosition(text, (sfVector2f) { 400 - textBounds.width / 2, 100 });
        sfRenderWindow_drawText(window, text, NULL);
        sfRenderWindow_display(window);
        sfSleep(sfSeconds(3));
    }

    sfText_destroy(text);
    sfText_destroy(headerText);
    sfFont_destroy(font);
    sfSprite_destroy(backgroundSprite);
    sfTexture_destroy(backgroundTexture);
}
