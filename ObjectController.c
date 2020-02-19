#include <gb/gb.h>

typedef struct ObjectController{
    UINT8 x;
    UINT8 y;
    UINT8 sprite_1;
    UINT8 sprite_2;
    UINT8 sprite_3;
    UINT8 movable; // 1 up, 2 right, 3 down, 4 left
} ObjectController;