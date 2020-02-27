#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdio.h>

#include "sprites/Characters.c"
#include "CharacterController.c"
#include "ObjectController.c"

#include "maps/map1.c"
#include "maps/map2.c"
#include "maps/map3.c"
#include "maps/map4.c"
#include "maps/map5.c"
#include "maps/map6.c"
#include "maps/map7.c"
#include "maps/map8.c"
#include "maps/map9.c"
#include "maps/map10.c"
#include "maps/mapTest.c"
#include "sprites/SpritesPark.c"

// If by collecting this object the map is won.
#define OBJECT_WIN_CONDITION_FLAG    0x01U

// If you can push this object
#define OBJECT_MOVABLE_FLAG      0x02U

// If any character is in this object position, you lose the map.
#define OBJECT_DEATH_FLAG     0x04U

#define RGB_GREEN_1    RGB(28, 30, 26)
#define RGB_GREEN_2    RGB(18, 22, 14)
#define RGB_GREEN_3    RGB(7, 12, 10)
#define RGB_GREEN_4    RGB(1, 3, 4)
#define RGB_BLUE       RGB( 0,  0, 31)
#define RGB_PURPLE     RGB(21,  0, 21)
#define RGB_LIGHTFLESH RGB(30, 20, 15)

#define RGB_RED        RGB(31,  0,  0)
#define RGB_BLUE       RGB( 0,  0, 31)
#define RGB_PURPLE     RGB(21,  0, 21)
#define RGB_LIGHTFLESH RGB(30, 20, 15)

UWORD spritePalette[] = {
  RGB_GREEN_1, RGB_GREEN_2, RGB_GREEN_3, RGB_GREEN_4,
  // 0, RGB_GREEN, RGB_BLUE, RGB_PURPLE,
  // 0, RGB_BLACK, RGB_RED, RGB_WHITE
};

UBYTE running;
UINT8 timer;
UINT8 characters_available;
UBYTE input_timer;
UBYTE current_map;
// UBYTE16 player_map_position;
// UBYTE map
unsigned char map[360]; // Controlls which map we are seeing.

CharacterController bunny;  //1
CharacterController dog1;   //2
CharacterController dog2;   //3
CharacterController cat;    //4
CharacterController horse;  //5
CharacterController *player;
ObjectController key;
ObjectController item1;
ObjectController item2;
ObjectController item3;

void performantDelay(UINT8 numLoops) {
  UINT8 i;
  for (i = 0; i != numLoops; i++) {
    wait_vbl_done();
  }
}

int can_move_to_map_pos(UINT16 map_position);

int got_door(CharacterController** c);

// Check player controls.
void player_input(CharacterController** c);

// Check player is ded.
int is_ded(CharacterController** c);

// Inits the game base variables, sprites, background.
void map_init();

UINT16 _get_next_map_position(UINT16 map_pos, UINT8 direction);

int got_key(CharacterController** c);

// Handles the first map logic.
void map_loop();

void set_character_sprite(CharacterController* c);

void move_character(CharacterController* c);

int can_move(INT8 x, INT8 y, UINT8 direction);

UINT16 get_player_map_position(UINT8 x,UINT8 y) ;

void dog1_power_apply(UINT16 map_position_block, UINT8 direction, UINT16 map_position_next);

void timers() {
  // printf("%d\n", input_timer);
  if (input_timer != 0) {
    input_timer -= 1;
  }
  timer += 1;
}


void main(void)
{
  init();
  map_init();

  while(running) {

    timers();
    map_loop();
    performantDelay(2);
  
  }

  printf("The end\n");
  while (1) {

  }
}

void init() {
  current_map = 200;
  characters_available = 2;

  copy_map();
  set_bkg_data(0,120,sprites_park);
  set_bkg_tiles(0, 0, 20, 18, map);
  
  set_sprite_data(0,30,Characters);
  set_sprite_palette(0, 4, spritePalette);
  // set_sprite_palette(0, 1, spritePalette[1]);
  // set_sprite_palette(0, 2, spritePalette[2]);
  // set_sprite_palette(0, 3, spritePalette[3]);
  set_bkg_palette(0, 3, spritePalette);
  set_sprite_prop(0, 0);

  // BGP_REG = 0xF9U;
  // BGP_REG = 0xFEU;
  // BGP_REG = 0xFFU;
  BGP_REG = 0xE4U;

  DISPLAY_ON;
  
  SHOW_BKG;

  SHOW_SPRITES;

  input_timer = 0u;
  running = 1u;
  timer = 1u;
  player = &bunny;
}

// Update map 1 water
void map_water(){
  unsigned char sprite_water_1[] =
  {
    0xFF,0x00,0xDD,0x00,0xAA,0x00,0xFF,0x00,
    0xFF,0x00,0xBB,0x00,0x55,0x00,0xFF,0x00
  };

  unsigned char sprite_water_2[] =
  {
    0xFF,0x00,0xBB,0x00,0x55,0x00,0xFF,0x00,
    0xFF,0x00,0xDD,0x00,0xAA,0x00,0xFF,0x00
  };

  if (timer % 32 == 0) {
    if (timer % 64 == 0) {
      set_bkg_data(11,1,sprite_water_1);
      
    }
    else {
      set_bkg_data(11,1,sprite_water_2);
      
    }
  }
  // sprites_park[90] = 0xA3;
  // sprites_park[91] = 0xA3;
  // sprites_park[92] = 0xA3;
  // sprites_park[93] = 0xA3;
  // sprites_park[94] = 0xA3;
  // unsigned char sprite[] =
  //   {
  //     0xFF,0x00,0xDD,0x00,0xAA,0x00,0xFF,0x00,
  //     0xFF,0x00,0xBB,0x00,0x55,0x00,0xFF,0x00,
  //   };
  // // sprites_park[0] = 0xA3;
  // set_bkg_data(11,1,sprite);

  // tile 11, position 88
  // if (sprites_park[90] == 0xDD) {
  //   sprites_park[90] = 0xBB;
  // }

  // UINT16 map_position;

  // if (timer % 32 == 0) {
  //   map_position = 360;
  //   while (map_position != 0) {
  //     map_position -= 1;
  //     // printf("%d\n", map_position);
  //     switch((UINT16)map[map_position]) {
  //       case (UINT16) 2:
  //         map[map_position] = 3;
  //         break;
  //       case (UINT16) 3:
  //         map[map_position] = 2;
  //         break;

  //     }
  //   }
  //   set_bkg_tiles(0, 0, 20, 18, map);

  // }
  
}

int change_char() {

  if (characters_available == 1) {
    return 0;
  }

  player->direction = 0;

  if (characters_available == 2) {
    switch (player->type) {
      case 1:
        player = &dog1;
        break;
      case 2:
        player = &bunny;
        break;
    }
  }
  if (characters_available == 3) {
    switch (player->type) {
      case 1:
        player = &dog1;
        break;
      case 2:
        player = &dog2;
        break;
      case 3:
        player = &bunny;
        break;
    }
  }
  if (characters_available == 4) {
    switch (player->type) {
      case 1:
        player = &dog1;
        break;
      case 2:
        player = &dog2;
        break;
      case 3:
        player = &cat;
        break;
      case 4:
        player = &bunny;
        break;
    }
  }
  // This will make the player know which player char was 
  // selected and give some time so it does not keep
  // pressing and changing chars.
  rotate_player();

}

void map_loop() {
  
  if (is_ded(&player)) {
    map_init();
  }
  got_door(&player);
  // printf("%u\n", (UINT16)get_player_map_position(player->x, player->y));
  got_key(&player);
  map_water();

  // generate_bunny();
  player_input(&player);

 
  move_character(&bunny);
  move_character(&dog1);
  move_character(&dog2);
  move_character(&cat);


}

void helper_copy_map(char *base_map) {
  UINT16 i;
  for(i = 0; i < 360; i++) {
    map[i] = base_map[i];
  }

}
void copy_map() {
  switch (current_map) {
    case 1:
      helper_copy_map(&Map1);
      break;
    case 2:
      helper_copy_map(&Map2);
      break;
    case 3:
      helper_copy_map(&Map3);
      break;
    case 4:
      helper_copy_map(&Map4);
      break;
    case 5:
      helper_copy_map(&MapTest);
      break;
    case 6:
      helper_copy_map(&MapTest);
      break;
    case 7:
      helper_copy_map(&MapTest);
      break;
    case 8:
      helper_copy_map(&MapTest);
      break;
    case 9:
      helper_copy_map(&MapTest);
      break;
    case 10:
      helper_copy_map(&MapTest);
      break;
    default:
      helper_copy_map(&MapTest);
      break;

  }

}

void map_init() {
  copy_map();
  set_bkg_tiles(0, 0, 20, 18, map);
  instanciate_chars();



  // key.x = 152;
  // key.y = 144;
  // // key.x = 8;
  // // key.y = 32;
  // key.sprite_1 = 14;


  // set_character_sprite(&key);

}

void instanciate_chars() {
  // generate bunny
  bunny.x = 8;
  bunny.y = 16;
  bunny.sprite_1 = 0;
  bunny.sprite_2 = 1;
  bunny.sprite_3 = 2;
  bunny.sprite_4 = 16;
  bunny.sprite_5 = 17;
  bunny.sprite_6 = 18;
  bunny.direction = 0;
  bunny.power_active = 0;
  bunny.type = 1;
  bunny.map_position = 400;

  dog1.x = 16;
  dog1.y = 16;
  dog1.sprite_1 = 3;
  dog1.sprite_2 = 4;
  dog1.sprite_3 = 5;
  dog1.sprite_4 = 19;
  dog1.sprite_5 = 20;
  dog1.sprite_6 = 21;
  dog1.direction = 0;
  dog1.power_active = 0;
  dog1.type = 2;
  dog1.map_position = 400;

  dog2.x = 8;
  dog2.y = 24;
  dog2.sprite_1 = 6;
  dog2.sprite_2 = 7;
  dog2.sprite_3 = 8;
  dog2.sprite_4 = 19;
  dog2.sprite_5 = 20;
  dog2.sprite_6 = 21;
  dog2.direction = 0;
  dog2.power_active = 0;
  dog2.type = 3;
  dog2.map_position = 400;

  cat.x = 16;
  cat.y = 24;
  cat.sprite_1 = 9;
  cat.sprite_2 = 10;
  cat.sprite_3 = 5;
  cat.sprite_4 = 19;
  cat.sprite_5 = 20;
  cat.sprite_6 = 21;
  cat.direction = 0;
  cat.power_active = 0;
  cat.type = 4;
  cat.map_position = 400;


  set_character_sprite(&bunny);
  set_character_sprite(&dog1);
  set_character_sprite(&dog2);
  set_character_sprite(&cat);
}


int got_key(CharacterController** c) {
  UINT16 player_map_position = get_player_map_position(player->x, player->y);

  // 46 is the key
  if ((UINT16)map[player_map_position] == 46) {
    // update the block so you can pass it
    UINT16 map_position;


    map_position = 360;
    while (map_position != 0) {
      map_position -= 1;
      // printf("%d\n", map_position);
      switch((UINT16)map[map_position]) {
        case (UINT16) 41: // The lock
          map[map_position] = 42;
          break;
        case (UINT16) 46: // The lock
          map[map_position] = 9;
          break;

      }
    }
    set_bkg_tiles(0, 0, 20, 18, map);
  }
}

int got_door(CharacterController** c) {
  UINT16 player_map_position = get_player_map_position(player->x, player->y);

  switch ((UINT16)map[player_map_position]) {
    case 101:
      current_map = 1;
      map_init();
      break;
    case 102:
      current_map = 2;
      map_init();
      break;
    case 103:
      current_map = 3;
      map_init();
      break;
    case 104:
      current_map = 4;
      map_init();
      break;
    case 105:
      current_map = 5;
      map_init();
      break;
    case 106:
      current_map = 6;
      map_init();
      break;
    case 107:
      current_map = 7;
      map_init();
      break;
    case 108:
      current_map = 8;
      map_init();
      break;
    case 109:
      current_map = 9;
      map_init();
      break;
    case 110:
      current_map =10;
      map_init();
      break;
    case 111:
      current_map =11;
      map_init();
      break;
    case 112:
      current_map =12;
      map_init();
      break;
    case 113:
      current_map =13;
      map_init();
      break;
    case 114:
      current_map =14;
      map_init();
      break;
    case 115:
      current_map =15;
      map_init();
      break;
    case 116:
      current_map =16;
      map_init();
      break;
    case 117:
      current_map =17;
      map_init();
      break;
    case 118:
      current_map =18;
      map_init();
      break;
    case 119:
      current_map =19;
      map_init();
      break;
    case 120:
      current_map =20;
      map_init();
      break;
  }

}


int is_ded(CharacterController** c) {

  UINT8 _x;
  UINT8 _y;
  UINT16 map_position;

  if ((*c)->power_active && (*c)->type == 1) {
    return 0;
  }

      // printf("%u %u\n", (unsigned) player->x, ((unsigned) player->x) / 8) -1;

  _x = (((unsigned) (*c)->x) / 8) -1;
  _y = (((unsigned) (*c)->y) / 8) -2;


  // Map position
  map_position = 0;
  map_position += (unsigned) _x;
  map_position += ((unsigned) _y) * 20;

  switch((UINT16)map[map_position]) {
    case (UINT16)11: // Wata
    // case (UINT16)12: // abism

      return 1;
  }
  return 0;
}

// Return 1 if from the map_pos on the direction there are more ice
// or something that would make it slide
int ice_should_slide(UINT16 map_pos, UINT8 direction) {
  UINT16 next_map_pos;
  UINT8 i;
  i = 20;
  if ((UINT16)map[map_pos] == 13 || (UINT16)map[map_pos] == 14) {
    next_map_pos =_get_next_map_position(map_pos, direction); // handles colision
    if(map_pos == next_map_pos) {
      return 0;
    }
    return 1;
  }
  
  
  return 0;
}

void dog1_power() {
  UINT16 map_position_block;
  UINT16 map_position_next;

  set_sprite_tile(player->type, player->sprite_3);

  map_position_block = get_player_map_position(player->x, player->y);
  
  map_position_block = _get_next_map_position(map_position_block,player->direction );
  map_position_next = _get_next_map_position(map_position_block, player->direction);

  // printf("%u %u\n", map_position_block, map_position_next);
  switch((UINT16)map[map_position_block]) {
    case (UINT16)45:// Brunio's block
      // Update the position on the map to be the block after the block movment 47
      
      // Is there something there, like a player, that should make it notmove?
      if (can_move_to_map_pos(map_position_next) == 0) {
        break;
      }

      // move it!

      if ((UINT16)map[map_position_next] < (UINT16)15 || 
        (UINT16)map[map_position_next] == 47 || 
        (UINT16)map[map_position_next] == 48 ||
        (UINT16)map[map_position_next] == 42) {
        map[map_position_block] = 47;
        set_bkg_tiles(0, 0, 20, 18, map);
        dog1_power_apply((UINT16)map_position_block, (UINT8)player->direction, map_position_next);
      }
      if ((UINT16)map[map_position_next] < (UINT16)13) {
        // Ice
        // ice_should_slide
      }
      // switch ((UINT16)map[map_position_next]) {
        
      // }

      break;
  }
  // printf("4\n");
  // UINT8 x_map = (x * 20);
  // printf("%d %d\n", x, x_map );
  // return 1;
  set_sprite_tile(player->type, player->sprite_1);
}

UINT16 _get_next_map_position(UINT16 map_pos, UINT8 direction) {
    switch(direction) {
    case 1: //up
      if (map_pos > 20) {
        map_pos -= 20;
      }
      break;
    case 2: //right  
      if (((map_pos + 1) % 20) != 0) {
        map_pos += 1;
      }
      break;
    case 3: //down  
      if (map_pos < 340) {
        map_pos += 20;
      }
      break;
    case 4: //left
      if (((map_pos) % 20) != 0) {
        map_pos -= 1;
      }
      // map_pos -= 1;
      break;
  }
  return map_pos;
}

void dog1_power_apply(UINT16 map_position_block, UINT8 direction, UINT16 map_position_next) {
  UINT8 x;
  UINT8 y;
  UINT8 i;

  // Creates the sprite block
  y = ((map_position_block / 20 )*8) + 16;
  x = ((map_position_block % 20 )*8) + 8;
  set_sprite_tile(5, 15);
  
  // move the sprite
  i = 4;
  move_sprite(5, x, y);   
  
  while(i != 0) {
    switch (direction) {
      case 1:
        y -= 2;
        break;
      case 2:
        x += 2;
        break;
      case 3:
        y += 2;
        break;
      case 4:
        x -= 2;
        break;
    }
    performantDelay(2);
    move_sprite(5, x, y);  
    i -=1;
  }
  switch (map[map_position_next]) {
    case 11: // Water
      map[map_position_next] = 48;
      break;
    case 12: // abism, does nothing;
      break;
    default:
      map[map_position_next] = 45;
      break;
  }
  set_bkg_tiles(0, 0, 20, 18, map); 
  move_sprite(5, 0, 0);  
  


}

UINT16 get_player_map_position(UINT8 x,UINT8 y) {
  UINT8 _x;
  UINT8 _y;
  UINT16 map_position;

  // Translate the xy to the map tile they are currently on.
  // Resolution: 160x144 pixels (AKA 20x18 8x8-pixel tiles)
  
  _x = (((unsigned) x) / 8) -1;
  _y = (((unsigned) y) / 8) -2;

  // Map position
  map_position = 0;
  map_position += (unsigned) _x;
  map_position += ((unsigned) _y) * 20;

  return map_position;
}

void player_input(CharacterController** c) {
  // printf("%d %d %d\n", input_timer, (*c)->power_timer, (*c)->power_active);

  if ((*c)->power_timer == 0 && input_timer == 0) {

    switch (joypad()) {
      case J_A:
        if ((*c)->type == 1) {
          (*c)->power_timer = 16;
          (*c)->power_active = 1;
        }
        if ((*c)->type == 2) {
          dog1_power();
        }
        
        // input_timer needs to be updated inside the player stuff
        break;

      case J_B:
        // printf("troca\n");
        change_char();
        break;

      case J_LEFT:
          input_timer = 4;
          (*c)->direction = 4; 
        break;
      case J_RIGHT:
          input_timer = 4;
          (*c)->direction = 2; 
        break;
      case J_UP:
          input_timer = 4;
          (*c)->direction = 1; 
        break;
      case J_DOWN:
          (*c)->direction = 3; 
          input_timer = 4;
        break;


    }
  }

  // move_sprite((*c)->type, (*c)->x, (*c)->y);

  // c->x = 10;
  // move_sprite(*c->type, *c->x, *c->y);
}

int can_move_to_map_pos(UINT16 map_position) {
  // printf("%u\n", map_position);
  if ((UINT16)map[map_position] > (UINT16)20 &&
    (UINT16)map[map_position] < (UINT16)41) {
    // printf("2\n");
    return 0;
  }

  // Or, test individual blocks
  switch((UINT16)map[map_position]) {
    case (UINT16)41:
    case (UINT16)45:// Brunio's block
    // case (UINT16)2:
    // case (UINT16)3:
    // case (UINT16)4:
    // case (UINT16)7:
    // case (UINT16)9:
    // case (UINT16)4:
    // case (UINT16)10:
    // printf("PlayerNotMove\n");
    // printf("3\n");
      return 0;
      break;
  }

  if ((UINT16)map_position == bunny.map_position 
    || (UINT16)map_position == dog1.map_position 
    || (UINT16)map_position == dog2.map_position 
    || (UINT16)map_position == cat.map_position ) {
    // printf("3\n");
    return 0;
  }
  // printf("4\n");
  return 1;
}

int can_move(INT8 x, INT8 y, UINT8 direction) {
  UINT8 _x;
  UINT8 _y;
  UINT16 map_position;
      // printf("%u %u\n", (unsigned) player->x, ((unsigned) player->x) / 8) -1;
  // return 1;

  // Translate the xy to the map tile they are currently on.
  // Resolution: 160x144 pixels (AKA 20x18 8x8-pixel tiles)
  
  // Updatex x y to test if we can move there
  switch(direction) {
    case 4: //LEFT
      x -= 8; 
      break;
    case 2: // RIGHT
      x += 8; 
      break;
    case 1: // UP
      y -= 8; 
      break;
    case 3: // DOWN
      y += 8; 
      break;
    case 0:
      return 0;
  }

  _x = (((unsigned) x) / 8) -1;
  _y = (((unsigned) y) / 8) -2;


  //printf("x:%u _ %u %u\n", (unsigned) x, (unsigned) _x, (unsigned) direction);
  // x logic
  // 8 -> 0 (because it starts at 8)
  // Stops the player of leaving the map.
  if (_x < 0 || (unsigned) _x > 19 || _y < 0 || (unsigned) _y > 17) {
    // printf("1\n");
    return 0;
  }

  // If we are here, they are not on a border of the map, lets test other collisions.

  // Map position
  map_position = 0;
  map_position += (unsigned) _x;
  map_position += ((unsigned) _y) * 20;

  if (can_move_to_map_pos(map_position)) {
    return 1;
  }

  return 0;
}

void rotate_player() {
  set_sprite_tile(player->type, player->sprite_6);
  set_sprite_prop(player->type, S_FLIPX);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_3);
  set_sprite_prop(player->type, S_FLIPX | S_FLIPY);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_6);
  set_sprite_prop(player->type, S_FLIPY);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_3);
  set_sprite_prop(player->type, 0);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_1);
  set_sprite_prop(player->type, 0);
}

void move_character(CharacterController* c) {
  UINT8 movement;
  // We only want this to run for the current active player.
  if (player->type != c->type){
    return;
  }

  if (c->power_active != 0) {

    // Bunny power  -----------------------------------------
    if (c->type == 1) {// bunny
      // First time running here
      movement = 2;

      // Test once each time we are on a tile
      if (c->power_timer == 4 ||
          c->power_timer == 8 ||
          c->power_timer == 12 || 
          c->power_timer == 16) 
      {
        if (!can_move(c->x, c->y, c->direction)) {
          c->power_timer = 1; // 1 will make it end this loop.
          // printf('cantmove\n');
          movement = 0;
        }
      }
      
      // Do the movment 
      switch(c->direction) {
        case 4: //LEFT
          c->x -= movement;
          set_sprite_tile(c->type, c->sprite_3);
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 2: // RIGHT
          set_sprite_tile(c->type, c->sprite_3);
          set_sprite_prop(c->type, 0);
          c->x += movement; 
          break;
        case 1: // UP
          set_sprite_tile(c->type, c->sprite_6);
          set_sprite_prop(c->type, S_FLIPX);
          c->y -= movement; 
          break;
        case 3: // DOWN
          set_sprite_tile(c->type, c->sprite_6);
          set_sprite_prop(c->type, S_FLIPY);
          c->y += movement; 
          break;
        case 0:
          rotate_player();
          // The player didn't chose a direction, so we just... do a loop!
      }
      
      c->power_timer -= 1;
      if (c->power_timer == 0) {
        c->power_active = 0;
        switch(c->direction) {
          case 4: //LEFT
            // Add the feet movement
            set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, S_FLIPX);
            break;
          case 2: // RIGHT
            // Add the feet movement
            set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, 0);
            break;
          case 1: // UP
            set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, S_FLIPX);
            break;
          case 3: // DOWN
            set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, S_FLIPY);
            break;
        }
      }
      
    }

    // Dog power -----------------------------------------
    if (c->type == 2) {
      set_sprite_tile(c->type, c->sprite_3);

      if (c->power_active == 1) {
        // printf("Power active 1\n");
        c->power_active = 2;
        c->power_timer = 16;
      }
      c->power_timer -= 1;

      if (c->power_timer == 0) {
        c->power_active = 0;
        set_sprite_tile(c->type, c->sprite_1);
      }
    }
    
  }
  else {
    // The first time the player moves, the input timer is 4
    // then we test if the player can move, if not, we cancel
    // all other checks by setting the input timer as incatctive
    if (input_timer == 4) {
      if (!can_move(c->x, c->y, c->direction)) {
        input_timer = 0;
        // But, we still need to update the sprites accordingly, since
        // the last direction is used by the powers.
      }
    }

    // Handle the sprites. We have this separated from the movment
    // because the movment can not happen in the end.
    switch(c->direction) {
        case 4: //LEFT
          // Add the feet movement
          set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_2);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_1);
          }
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 2: // RIGHT
          // Add the feet movement
          set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_2);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_1);
          }
          // Unflip just to be sure
          set_sprite_prop(c->type, 0);
          break;
        case 1: // UP
          set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
          // Add the feet movement
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_5);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_4);
          }
        
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 3: // DOWN
          set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
          // Add the feet movement
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_5);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_4);
          }
          set_sprite_prop(c->type, S_FLIPY);
          break;
      }

    // Normal movement
    if (input_timer != 0) {
      
      switch(c->direction) {
        case 4: //LEFT
          c->x -= 2;
          break;
        case 2: // RIGHT
          c->x += 2; 
          break;
        case 1: // UP
          c->y -= 2; 
          break;
        case 3: // DOWN
          c->y += 2; 
          break;
      }
    }
    
  }

  move_sprite(c->type, c->x, c->y);
  c->map_position = get_player_map_position(c->x, c->y);
}

void set_character_sprite(CharacterController* c) {
  if (c->type <= characters_available ) {
    set_sprite_tile(c->type, c->sprite_1);
    move_sprite(c->type, c->x, c->y);  
    c->map_position = get_player_map_position(c->x, c->y);
  }
}
// 