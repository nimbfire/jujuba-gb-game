#include <gb/gb.h>

typedef struct CharacterController{
    UINT8 x;
    UINT8 y;
    UINT8 sprite_1;
    UINT8 sprite_2;
    UINT8 sprite_3;
    UINT8 sprite_4;
    UINT8 sprite_5;
    UINT8 sprite_6;
    UINT8 direction; // 1 up, 2 right, 3 down, 4 left
    UBYTE power_active;
    UINT8 power_timer;
    UBYTE type; // 1 bunny, 2 dog1, 3 dog2, 4cat, 5horse
    UBYTE is_active; // If its the char that the player is controlling now.
} CharacterController;