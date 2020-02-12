#include <gb/gb.h>
#include <stdio.h>

#include "sprites/Characters.c"
#include "CharacterController.c"

UBYTE running;

UBYTE map; // Controlls which map we are seeing.

CharacterController bunny;
CharacterController dog1;
CharacterController dog2;
CharacterController cat;


// Inits the game base variables, sprites, background.
void init();

// Handles the first map logic.
void map_1();

void set_character_sprite(CharacterController* c, UINT8 index);

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
  // generate_bunny();
}

void init() {
  running = 1;
  map = 1;
  DISPLAY_ON;
  SHOW_SPRITES;

  set_sprite_data(0,12,Characters);


  // generate bunny
  bunny.x = 40;
  bunny.y = 40;
  bunny.sprite_1 = 0;
  bunny.sprite_2 = 1;
  bunny.sprite_3 = 2;
  bunny.direction = 1;
  bunny.power_active = 0;
  bunny.type = 1;
  bunny.is_the_active = 1;

  dog1.x = 60;
  dog1.y = 60;
  dog1.sprite_1 = 3;
  dog1.sprite_2 = 4;
  dog1.sprite_3 = 5;
  dog1.direction = 1;
  dog1.power_active = 0;
  dog1.type = 2;
  dog1.is_the_active = 0;

  set_character_sprite(&bunny, 0);
  set_character_sprite(&dog1, 1);



}

void set_character_sprite(CharacterController* c, UINT8 index) {
  printf("HM\n");
  set_sprite_tile(index, c->sprite_1);
  move_sprite(index, c->x, c->y);

}
