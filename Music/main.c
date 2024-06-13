#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "initialization.h"
#include "gameLogic.h"
#include "menu.h"
#include "showLeaderboard.h"
#include "structs.h"

int main() {
    srand((unsigned int)time(NULL));

    sfRenderWindow* window;
    sfVideoMode videoMode = { 800, 600, 32 };
    window = sfRenderWindow_create(videoMode, "Quiz Game", sfResize | sfClose, NULL);

    int choice;
    int game = 1;

    while (sfRenderWindow_isOpen(window)) {
        choice = showMenu(window);
        if (choice == 0) {
            newGame(window);
        }
        else if (choice == 1) {
            showLeaderboard(window);
        }
        else if (choice == 2) {
            game = 0;
            sfRenderWindow_close(window);
        }
    }

    sfRenderWindow_destroy(window);
    return 0;
}
