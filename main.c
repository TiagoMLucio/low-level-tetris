#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>
#include "queue.h"
#include "game_screen.h"
#include "tile.h"
#include "uptime.h"

#include "custom_types.h"
#include <stdint.h>

#define malloc uspi_malloc
#include <stdlib.h>
#undef malloc

static const char FromGame[] = "tetris";

const unsigned LINES_PER_LEVEL = 4;
const unsigned CYCLES_PER_MICRO_SECOND = 250;

const unsigned SCORE_PER_CLEAR[4] = {100, 300, 500, 800};

unsigned grid[GAME_GRID_HEIGHT][GAME_GRID_WIDTH] = {0};
unsigned delay = 150000; // micro seconds
unsigned game_level = 1;
unsigned closed_lines = 0;
unsigned top = 0;
unsigned score = 0;

Queue queue;

boolean validTetrisChar(char c)
{
   return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

static void KeyPressedHandler(const char *pString)
{
   char c = *pString++;
   if (validTetrisChar(c))
      enqueue(&queue, c);
}

boolean check_collision(Tile tile)
{
   int(*rotation)[ROTATION_GRID_SIZE] = get_rotation(tile);
   for (int i = 0; i < ROTATION_GRID_SIZE; i++)
      for (int j = 0; j < ROTATION_GRID_SIZE; j++)
         if (rotation[i][j] && (grid[tile.y + 1 + i][tile.x + j] || tile.y + 1 + i >= GAME_GRID_HEIGHT))
            return TRUE;
   return FALSE;
}

boolean check_lateral_collision(int tileX, int tileY, int i, int j, char ch)
{
   return (ch == 'a') ? (tileX + j - 1 < 0 || grid[tileY + i][tileX + j - 1]) : (tileX + 1 + j >= GAME_GRID_WIDTH || grid[tileY + i][tileX + j + 1]);
}

boolean check_horizontal_collision(Tile tile, char ch)
{
   int(*rotation)[ROTATION_GRID_SIZE] = get_rotation(tile);

   for (int i = 0; i < ROTATION_GRID_SIZE; i++)
      for (int j = 0; j < ROTATION_GRID_SIZE; j++)
         if (rotation[i][j] && check_lateral_collision(tile.x, tile.y, i, j, ch))
            return TRUE;

   return FALSE;
}

boolean check_rotational_collision(int tileX, int tileY, int i, int j)
{
   return (tileX + j < 0 || tileX + j >= GAME_GRID_WIDTH || grid[tileY + i][tileX + j] || tileY + 1 + i >= GAME_GRID_HEIGHT);
}

boolean check_rotation_collision(Tile tile)
{
   int(*next_rotation)[ROTATION_GRID_SIZE] = get_next_rotation(tile);

   for (int i = 0; i < ROTATION_GRID_SIZE; i++)
      for (int j = 0; j < ROTATION_GRID_SIZE; j++)
         if (next_rotation[i][j] && check_rotational_collision(tile.x, tile.y, i, j))
            return TRUE;

   return FALSE;
}

void update_score(unsigned cleared)
{
   score += SCORE_PER_CLEAR[cleared - 1] * game_level;
   screen_update_score(score);
}

void setup()
{
   setup_game_frame();
   setup_type_frame();
   setup_lines_frame();
   setup_score_frame();
   setup_next_frame();
   setup_level_frame();
   setup_statistics_frame();
   // draw_grid(); // debugging
}

// salva no grid quando tile colide
void save_to_grid(Tile tile)
{
   TileType type = TILE_TYPES[tile.type];
   int(*rotation)[ROTATION_GRID_SIZE] = get_rotation(tile);
   unsigned color = type.color;

   int full_lines[4];
   int cnt_full = 0;

   // verifica quais linhas completaram
   for (int i = 0; i < ROTATION_GRID_SIZE && tile.y + i < GAME_GRID_HEIGHT; i++)
   {
      boolean added_to_line = 0;
      for (int j = 0; j < ROTATION_GRID_SIZE; j++)
      {
         if (tile.x + j < GAME_GRID_WIDTH && rotation[i][j])
         {
            grid[tile.y + i][tile.x + j] = rotation[i][j] ? color : 0;
            added_to_line = 1;
         }
      }
      if (added_to_line)
      {

         boolean full_line = TRUE;
         for (int j = 0; j < GAME_GRID_WIDTH; j++)
            if (!grid[tile.y + i][j])
               full_line = FALSE;

         if (full_line)
            full_lines[cnt_full++] = tile.y + i;
      }
   }

   if (cnt_full > 0)
   {
      LogWrite(FromGame, LOG_DEBUG, "Linhas completas: %d", cnt_full);
      closed_lines += cnt_full;
      update_score(cnt_full);
      screen_update_lines(closed_lines);
   }

   // apaga linhas completas
   for (int i = 0; i < cnt_full; i++)
      delete_line(full_lines[i]);

   // desce as linhas não completas uma a uma
   for (int cnt = 0; cnt < cnt_full; cnt++)
   {
      TimerusDelay(TimerGet(), delay);
      for (int i = full_lines[cnt] - 1; i >= cnt; i--)
      {
         for (int j = 0; j < GAME_GRID_WIDTH; j++)
         {
            grid[i + 1][j] = grid[i][j];
            draw_grid_block(i, j, 0);
            if (grid[i + 1][j])
               draw_grid_block(i + 1, j, grid[i + 1][j]);
         }
      }
   }
}

// quando o jogador perde
void end_game()
{
   LogWrite(FromGame, LOG_DEBUG, "Game ended!");
}

// realiza operação de teclado no tile atual
// d: direita
// a: esqueda
// w: rotação
// s: acelera a descida
void move_tile(Tile *tile, char dir)
{
   switch (dir)
   {
   case 'd':
   case 'a':
      if (check_horizontal_collision(*tile, dir))
         return;

      remove_tile(*tile);
      tile->x += dir == 'd' ? 1 : -1;
      put_tile(*tile);
      break;
   case 'w':
      if (check_rotation_collision(*tile))
         return;

      remove_tile(*tile);
      tile->rotation = (tile->rotation + 1) % 4;
      put_tile(*tile);
      break;
   case 's':
      tile->y++;
      break;
   default:
      LogWrite(FromGame, LOG_ERROR, "Key not found in move_tile()!");
      break;
   }
}

boolean run()
{
   Tile tile = create_tile();

   put_tile(tile); // spawna nova tile

   if (check_collision(tile)) // colidiu antes de spawnar
      return FALSE;           // finaliza o jogo

   // uma iteração por movimento vertical da nova tile
   while (1)
   {
      unsigned start_ticks = TimerGetTicks(TimerGet());

      // Le teclas e move tile
      while (TimerGetTicks(TimerGet()) - start_ticks <= 20 - game_level)
         if (!isQueueEmpty(&queue))
         {
            char c = dequeue(&queue);
            if (c == 's')
               break;
            move_tile(&tile, c);
         }

      // quando a tile colide no final do frame ou com outra tile
      if (check_collision(tile))
      {
         // salva no grid e verifica/opera linhas completas
         save_to_grid(tile);

         if (closed_lines >= game_level * LINES_PER_LEVEL)
         {
            delay = (19 * delay) / 20;
            closed_lines = 0;
            game_level++;
            LogWrite(FromGame, LOG_DEBUG, "Level %d", game_level);
         }

         return TRUE; // proxima tile
      }

      // quando não colide, move a tile 1 para baixo

      remove_tile(tile);
      tile.y++;
      put_tile(tile);
   }
}

void start_game()
{
   unsigned seed = get_current_time();

   LogWrite(FromGame, LOG_NOTICE, "Seed: %u", seed);
   LogWrite(FromGame, LOG_DEBUG, "Width: %upx, Height: %upx", ScreenDeviceGetWidth(USPiEnvGetScreen()), ScreenDeviceGetHeight(USPiEnvGetScreen()));

   srand(seed);

   setup(); // antes de adicionar as tiles
   TimerusDelay(TimerGet(), 2 * delay);

   // cada iteração spawna uma nova tile
   // sai do loop quando o jogador perde
   while (run())
      ;

   end_game();
}

int main(void)
{
   if (!USPiEnvInitialize())
      return EXIT_HALT;

   if (!USPiInitialize())
   {
      LogWrite(FromGame, LOG_ERROR, "Cannot initialize USPi");

      USPiEnvClose();

      return EXIT_HALT;
   }

   if (!USPiKeyboardAvailable())
   {
      LogWrite(FromGame, LOG_ERROR, "Keyboard not found");

      USPiEnvClose();

      return EXIT_HALT;
   }

   initQueue(&queue);

   USPiKeyboardRegisterKeyPressedHandler(KeyPressedHandler);

   start_game();

   return EXIT_HALT;
}
