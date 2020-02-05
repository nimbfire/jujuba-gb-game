#include <gb/gb.h>

typedef struct CharacterController{
    UINT8 x;
    UINT8 y;
    UINT8 sprite_1;
    UINT8 sprite_2;
    UINT8 sprite_3;
    INT8 direction; // 1 up, 2 right, 3 down, 4 left
    UBYTE power_active;
    UBYTE type; // 1 bunny, 2 dog1, 3 dog2, 4cat
} CharacterController;