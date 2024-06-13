#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>
#include <stdio.h>
#include "menu.h"

int showMenu(sfRenderWindow* window) {
    int choice = -1;

    sfTexture* backgroundTexture = sfTexture_createFromFile("background2.jpeg", NULL);
    if (!backgroundTexture) {
        fprintf(stderr, "Failed to load background image.\n");
        return -1;
    }
    sfSprite* backgroundSprite = sfSprite_create();
    sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);

    sfFont* font = sfFont_createFromFile("fonts/arial.ttf");
    sfText* text = sfText_create();
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 40);

    sfText* options[3];
    const char* optionTexts[3] = { "New Game", "Leaderboard", "Exit" };

    sfVector2u windowSize = sfRenderWindow_getSize(window);
    float windowWidth = (float)windowSize.x;
    float windowHeight = (float)windowSize.y;

    for (int i = 0; i < 3; ++i) {
        options[i] = sfText_create();
        sfText_setString(options[i], optionTexts[i]);
        sfText_setFont(options[i], font);
        sfText_setCharacterSize(options[i], 40);

        sfFloatRect textBounds = sfText_getGlobalBounds(options[i]);
        float textWidth = textBounds.width;
        float textHeight = textBounds.height;

        sfText_setPosition(options[i], (sfVector2f) { (windowWidth - textWidth) / 2, (windowHeight / 2 - 100) + i * 60 });
    }

    while (sfRenderWindow_isOpen(window) && choice == -1) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            else if (event.type == sfEvtMouseButtonPressed) {
                for (int i = 0; i < 3; ++i) {
                    sfFloatRect textBounds = sfText_getGlobalBounds(options[i]);
                    if (sfFloatRect_contains(&textBounds, (float)event.mouseButton.x, (float)event.mouseButton.y)) {
                        choice = i;
                        break;
                    }
                }
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        for (int i = 0; i < 3; ++i) {
            sfRenderWindow_drawText(window, options[i], NULL);
        }
        sfRenderWindow_display(window);
    }

    for (int i = 0; i < 3; ++i) {
        sfText_destroy(options[i]);
    }
    sfText_destroy(text);
    sfFont_destroy(font);
    sfSprite_destroy(backgroundSprite);
    sfTexture_destroy(backgroundTexture);

    return choice;
}
