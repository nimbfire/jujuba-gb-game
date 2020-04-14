// Handles maps logic

// old maps
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
#include "maps/mapTestSmallGrid.c"


// -------------------------------------------------------
// -------------------------------------------------------
// Maps!!!!
// -------------------------------------------------------
// -------------------------------------------------------

#include "tilemaps/MapDev.c"
#define MAP_DEV 0

#include "tilemaps/MapTeachJumpOverIce.c"
#define MAP_TEACH_JUMP_OVER_ICE 15

#include "tilemaps/MapForestKeyEasy.c"
#define MAP_FOREST_KEY_EASY 1

#include "tilemaps/MapBunnyWaterHard.c"
#define MAP_BUNNY_WATER_HARD 2



void fperformantDelay(UINT8 numLoops) {
  UINT8 i;
  for (i = 0; i != numLoops; i++) {
    wait_vbl_done();
  }
}

// Handles the logic behind to which new map the player should
// jump to after getting inside a door
// If a map has a door, this is the switch that decides to where
// the player should jump to
int got_door_switch(UINT8 x, UINT8 y, UINT8 current_map) {

  switch(current_map) {
    case MAP_DEV: 
      if (x == 13 && y == 2) {
        return MAP_FOREST_KEY_EASY;
      }
      if (x == 13 && y == 4) {
        return MAP_TEACH_JUMP_OVER_ICE;
      }
      if (x == 13 && y == 6) {
        return MAP_BUNNY_WATER_HARD;
      }
      // printf("MAP DEV BUT NOT POSITIONING\n");
      // fperformantDelay(500);


    default:
      // printf("DEFAULT FUUUCK\n");
      // fperformantDelay(500);
      
      return MAP_DEV;
  }
}


void helper_copy_map(char *base_map) {
  UINT16 i;
  for(i = 0; i < 360; i++) {
    map[i] = base_map[i];
  }
}

void helper_copy_map_smaller(char *base_map, UINT8 width, UINT8 height) {
  UINT8 gap_width = (20 - width) / 2;
  UINT8 gap_height = (20 - height) / 2;
  UINT16 i;
  UINT16 base_map_i;
  UINT8 col; // small col counter
  UINT8 line; // small col counter
  UINT16 k;// The line
  i = 0;
  line = 0;
  base_map_i = 0;
  width = gap_width + width;

  // if the widht of the map is 10, it will have 5 on the left 
  //   and 5 on the right
  // also, if the height is 10, it will have 5 lines before
  // 0                                        19
  //   _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
  // |                                         | 19
  // |                                         | 39
  // |                                         | 59
  // |                                         | 79
  // |                                         | 99
  // |           X O O O O O O O O O O         |
  // |
  // |
  // |
  // |
  // |
  // |
  // |
  // |
  // |
  // |
  // |
  // |
  // |

  // So, the X will be gap_height * 20 - 1 (so, 99 in this case)
  // and the X will be the gap_widht on the first run, and double the gap on the next ones

  // Put the first blank lines:
  // We don't use the i beind decremented because of bugs on the gameboy
  // and the gbdk library
  i = 0;
  k = (gap_height * 20);
  while(i != k) {
    map[i] = 30;
    i += 1;
  }

  while (line < height) {
    col = 0;
    while (col < gap_width) {
      map[i] = 30;
      i += 1;
      col += 1;
    }

    while (col < width) {
      map[i] = base_map[base_map_i];
      i += 1;
      col += 1;
      base_map_i += 1;
    }
    while (col < 20) {
      map[i] = 30;
      i += 1;
      col += 1;
    }
    line += 1;
  }

  // Now we fill the last blocks!
  while(i != 360) {
    map[i] = 30;
    i += 1;
  }
  




  // set_bkg_tiles(0, 0, 20, 18, map);




  // performantDelay(500);
  // performantDelay(500);
  // performantDelay(500);
  // performantDelay(500);


}

void copy_map(UINT8 current_map) {
  switch (current_map) {
    case MAP_FOREST_KEY_EASY:
      helper_copy_map(&MapForestKeyEasy);
      break;
    case MAP_BUNNY_WATER_HARD:
      helper_copy_map(&MapBunnyWaterHard);
      break;
    case 3:
      helper_copy_map(&Map3);
      break;
    case 4:
      helper_copy_map(&Map4);
      break;
    case 5:
      helper_copy_map(&Map5);
      break;
    case 6:
      helper_copy_map(&Map6);
      break;
    case 7:
      helper_copy_map(&MapDev);
      break;
    case 8:
      helper_copy_map(&MapDev);
      break;
    case 9:
      helper_copy_map(&MapDev);
      break;
    case 10:
      //   helper_copy_map(&MapDev);
      // case 11:
      helper_copy_map_smaller(mapTestSmallGrid, mapTestSmallGridWidth, mapTestSmallGridHeight);
      
      break;
    case MAP_TEACH_JUMP_OVER_ICE:
      //   helper_copy_map(&MapDev);
      // case 11:
      helper_copy_map_smaller(MapTeachJumpOverIce, MapTeachJumpOverIceWidth, MapTeachJumpOverIceHeight);
      // helper_copy_map(&MapDev);
      // set_bkg_tiles(0, 0, 20, 18, map);

      // printf("Mapa QUINZE");

      // performantDelay(500);

      break;
    default:
      helper_copy_map(&MapDev);
      break;

  }

}


  // switch ((UINT16)map[player_map_position]) {
  //   case 101:
  //     current_map = 1;
  //     map_init();
  //     break;
  //   case 102:
  //     current_map = 2;
  //     map_init();
  //     break;
  //   case 103:
  //     current_map = 3;
  //     map_init();
  //     break;
  //   case 104:
  //     current_map = 4;
  //     map_init();
  //     break;
  //   case 105:
  //     current_map = 5;
  //     map_init();
  //     break;
  //   case 106:
  //     current_map = 6;
  //     map_init();
  //     break;
  //   case 107:
  //     current_map = 7;
  //     map_init();
  //     break;
  //   case 108:
  //     current_map = 8;
  //     map_init();
  //     break;
  //   case 109:
  //     current_map = 9;
  //     map_init();
  //     break;
  //   case 110:
  //     current_map =10;
  //     map_init();
  //     break;
  //   case 111:
  //     current_map =11;
  //     map_init();
  //     break;
  //   case 112:
  //     current_map =12;
  //     map_init();
  //     break;
  //   case 113:
  //     current_map =13;
  //     map_init();
  //     break;
  //   case 114:
  //     current_map =14;
  //     map_init();
  //     break;
  //   case 115:
  //     current_map =15;
  //     map_init();
  //     break;
  //   case 116:
  //     current_map =16;
  //     map_init();
  //     break;
  //   case 117:
  //     current_map =17;
  //     map_init();
  //     break;
  //   case 118:
  //     current_map =18;
  //     map_init();
  //     break;
  //   case 119:
  //     current_map =19;
  //     map_init();
  //     break;
  //   case 120:
  //     current_map =20;
  //     map_init();
  //     break;
  // }
