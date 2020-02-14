#include <gb/gb.h>
#include <stdio.h>

#include "sprites/Characters.c"
#include "CharacterController.c"

UBYTE running;
UBYTE timer;
UBYTE troca_timer;
// UBYTE map
UBYTE map; // Controlls which map we are seeing.

CharacterController bunny;  //1
CharacterController dog1;   //2
CharacterController dog2;   //3
CharacterController cat;    //4
CharacterController horse;  //5
CharacterController *player;


// Check player controls.
void player_input(CharacterController** c);

// Inits the game base variables, sprites, background.
void init();

// Handles the first map logic.
void map_1();

void set_character_sprite(CharacterController* c);

void move_character(CharacterController* c);

void timers() {
  if (troca_timer != 0) {
    troca_timer -= 1;
  }
}

void main(void)
{
  init();

  while(running) {
    timers();
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
  player_input(&player);

  if (troca_timer != 0) {
    if (troca_timer == 10) {
      printf("TROCA\n");
      switch (player->type) {
        case 1:
          player = &dog1;
          break;
        case 2:
          player = &bunny;
          break;
      }

    }
  }
  move_character(&bunny);
  move_character(&dog1);


}

void init() {
  troca_timer = 0;
  running = 1;
  map = 1;
  player = &bunny;
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

  player->x=10;

  set_character_sprite(&bunny);
  set_character_sprite(&dog1);



}

void player_input(CharacterController** c) {
  printf("%d\n", (*c)->x);

  if ((*c)->power_active == 0 && troca_timer == 0) {

    switch (joypad()) {
      case J_A:
        (*c)->power_timer = 10;
        (*c)->power_active = 1;
        break;

      case J_B:
        printf("troca\n");
        troca_timer = 10;
        
        break;

      case J_LEFT:
        (*c)->x -= 2; 
        (*c)->direction = 4; 
        set_sprite_prop((*c)->type, S_FLIPX);
        break;
      case J_RIGHT:
        (*c)->x += 2; 
        (*c)->direction = 2; 
        set_sprite_prop((*c)->type, 0);
        break;
      case J_UP:
        (*c)->y -= 2; 
        (*c)->direction = 1; 
        break;
      case J_DOWN:
        (*c)->y += 2; 
        (*c)->direction = 3; 
        break;
    }
  }

  // move_sprite((*c)->type, (*c)->x, (*c)->y);

  // c->x = 10;
  // move_sprite(*c->type, *c->x, *c->y);
}

void move_character(CharacterController* c) {
  if (c->type == 1) {// bunny
    if (c->power_active) {
      set_sprite_tile(c->type, c->sprite_3);

      switch(c->direction) {
        case 4: //LEFT
          c->x -= 2; 
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 2: // RIGHT
          c->x += 2; 
          set_sprite_prop(c->type, 0);
          break;
        case 1: // UP
          c->y -= 2; 
          break;
        case 3: // DOWN
          c->y += 2; 
          break;
      }
      c->power_timer -= 1;
      if (c->power_timer == 0) {
        c->power_active = 0;
        set_sprite_tile(c->type, c->sprite_1);
      }
    }
  }
  move_sprite(c->type, c->x, c->y);
}

void set_character_sprite(CharacterController* c) {
  set_sprite_tile(c->type, c->sprite_1);
  move_sprite(c->type, c->x, c->y);
}
// 