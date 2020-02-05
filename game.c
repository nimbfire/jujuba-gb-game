#include <gb/gb.h>
#include <stdio.h>

#include "sprites/Characters.c"
#include "CharacterController.c"

UBYTE running;

UBYTE map; // Controlls which map we are seeing.

// Inits the game base variables, sprites, background.
void init();

// Handles the first map logic.
void map_1();

void main(void)
{
  init();

  while(running) {
    if (map == 1) {
      map_1();
    }
  }

  printf("The end\n");
  while (1) {

  }
}

void map_1() {

}

void init() {
  running = 1;
  map = 1;
  DISPLAY_ON;
  SHOW_SPRITES;
  set_sprite_data(0,3,Characters);
  set_sprite_tile(0,2);
  move_sprite(0, 40, 40);

}