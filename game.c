#include <gb/gb.h>
#include <stdio.h>

#include "sprites/Characters.c"

UBYTE running;
void init();

void main(void)
{
  init();
  // printf("Game first test\n");
  while(running) {

  }
}


void init() {
  running = 1;
  DISPLAY_ON;
  SHOW_SPRITES;
  set_sprite_data(0,3,Characters);
  set_sprite_tile(0,2);
  move_sprite(0, 40, 40);

}