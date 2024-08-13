//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>

static const char FromSample[] = "sample";

#define WIDTH 800
#define HEIGHT 600

const int BLOCK_SIZE = 16;

// dimensões do frame
const int frame_width = 10 * BLOCK_SIZE;
const int frame_height = 20 * BLOCK_SIZE;
const int frame_thickness = 8;

// (x, y) do canto superior esquerdo externo ao frame
const int frame_pos_x = WIDTH / 2 - frame_width / 2 - frame_thickness;
const int frame_pos_y = HEIGHT / 2 - frame_height / 2 - frame_thickness;

// (x, y) do canto superior esquerdo interno ao frame
const int grid_pos_x = frame_pos_x + frame_thickness;
const int grid_pos_y = frame_pos_y + frame_thickness;

unsigned grid[20][10] = {0};
unsigned speed = 100000;

typedef struct
{
   int rotations[4][4][4];
   unsigned color;
} BlockType;

typedef struct
{
   int type;
   int rotation;
   int x, y;
} Block;

#define pack_color(r, g, b) 1 // (unsigned)((r << 16) | (g << 8) | b)

// Blocks e as suas rotações
BlockType block_types[] = {
    // I block
    {{{{1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0}},
      {{1, 1, 1, 1},
       {0, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0}}},
     pack_color(0, 255, 255)},
    // O block
    {{{{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}}},
     pack_color(255, 255, 0)},
    // T block
    {{{{0, 1, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}}},
     pack_color(128, 0, 128)},
    // L block
    {{{{0, 0, 1, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}}},
     pack_color(255, 165, 0)},
    // J block
    {{{{1, 0, 0, 0},
       {1, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {1, 0, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {0, 1, 0, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0}}},
     pack_color(0, 0, 255)},
    // S block
    {{{{0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 1, 0},
       {1, 1, 0, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 0, 0, 0}}},
     pack_color(0, 255, 0)},
    // Z block
    {{{{1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}},
      {{1, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 0, 0},
       {0, 0, 0, 0}},
      {{0, 1, 0, 0},
       {1, 1, 0, 0},
       {1, 0, 0, 0},
       {0, 0, 0, 0}}},
     pack_color(255, 0, 0)}};

#define QUEUE_SIZE 10

typedef struct
{
   char data[QUEUE_SIZE];
   int front;
   int rear;
   int size;
} Queue;

// Initialize the queue
void initQueue(Queue *q)
{
   q->front = 0;
   q->rear = 0;
   q->size = 0;
}

// Check if the queue is empty
int isQueueEmpty(Queue *q)
{
   return q->size == 0;
}

// Check if the queue is full
int isQueueFull(Queue *q)
{
   return q->size == QUEUE_SIZE;
}

// Enqueue an element
void enqueue(Queue *q, char item)
{
   if (isQueueFull(q))
      return;

   q->data[q->rear] = item;
   q->rear = (q->rear + 1) % QUEUE_SIZE;
   q->size++;
}

// Dequeue an element
char dequeue(Queue *q)
{
   if (isQueueEmpty(q))
      return '\0';
   char item = q->data[q->front];
   q->front = (q->front + 1) % QUEUE_SIZE;
   q->size--;
   return item;
}

void fb_pixel(int x, int y, unsigned color)
{
   ScreenDeviceSetPixel(USPiEnvGetScreen(), x, y, color);
}

void draw_line(int x, int y, int length, unsigned color, int thickness, u8 vertical)
{
   for (int i = 0; i < thickness; i++)
      for (int j = 0; j < length; j++)
         fb_pixel(x + (vertical ? i : j), y + (vertical ? j : i), color);
}

void draw_rect(int x, int y, int width, int height, int thickness, unsigned color)
{
   draw_line(x, y, height + 2 * thickness, color, thickness, 1);                     // left
   draw_line(x + width + thickness, y, height + 2 * thickness, color, thickness, 1); // right
   draw_line(x, y, width + 2 * thickness, color, thickness, 0);                      // top
   draw_line(x, y + height + thickness, width + 2 * thickness, color, thickness, 0); // bottom
}

void draw_frame()
{
   unsigned white = 1;
   draw_rect(frame_pos_x, frame_pos_y, frame_width, frame_height, frame_thickness, white);
}

// usado para debugging
void draw_grid()
{
   unsigned white = pack_color(255, 255, 255);
   for (int i = 0; i < 20; i++)
   {
      for (int j = 0; j < 10; j++)
      {
         fb_pixel(grid_pos_x + j * BLOCK_SIZE, grid_pos_y + i * BLOCK_SIZE, white);
         fb_pixel(grid_pos_x + j * BLOCK_SIZE + BLOCK_SIZE - 1, grid_pos_y + i * BLOCK_SIZE, white);
         fb_pixel(grid_pos_x + j * BLOCK_SIZE, grid_pos_y + i * BLOCK_SIZE + BLOCK_SIZE - 1, white);
         fb_pixel(grid_pos_x + j * BLOCK_SIZE + BLOCK_SIZE - 1, grid_pos_y + i * BLOCK_SIZE + BLOCK_SIZE - 1, white);
      }
   }
}

// quadrado do tamanho BLOCK_SIZE
void draw_square(int pos_x, int pos_y, unsigned color)
{
   for (int i = 0; i < BLOCK_SIZE; i++)
      for (int j = 0; j < BLOCK_SIZE; j++)
         fb_pixel(pos_x + i, pos_y + j, color);
}

// quadrado na posição (i, j) do frame
// começa no canto superior esquerdo
void draw_grid_square(int i, int j, unsigned color)
{
   draw_square(grid_pos_x + i * BLOCK_SIZE, grid_pos_y + j * BLOCK_SIZE, color);
}

// desenha a tile completa
void draw_block(Block block, unsigned color)
{
   BlockType type = block_types[block.type];
   int(*rotation)[4] = type.rotations[block.rotation];

   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         if (rotation[i][j])
            draw_grid_square(block.x + j, block.y + i, color);
}

void put_block(Block block)
{
   draw_block(block, block_types[block.type].color);
}

void remove_block(Block block)
{
   draw_block(block, 0);
}

int (*get_rotation(Block block))[4]
{
   return block_types[block.type].rotations[block.rotation];
}

u8 check_collision(Block block)
{
   int(*rotation)[4] = get_rotation(block);
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         if ((rotation[i][j] && grid[block.y + 1 + i][block.x + j]) || (rotation[i][j] && block.y + 1 + i >= 20))
            return 1;
   return 0;
}

Block create_tile()
{
   static unsigned type = 0;
   Block current_block;
   current_block.type = 1; // Random tile
   current_block.rotation = type % 7;
   current_block.x = 3; // Starting X position (middle of the frame)
   current_block.y = 0; // Starting Y position (top of the frame)

   return current_block;
}

void setup()
{
   draw_frame();
   // draw_grid(); // debugging
}

int cnt;

void usleep(unsigned delay)
{
   for (int i = 0; i < 200 * delay; i++)
   {
      cnt++;
   }
}

// salva no grid quando tile colide
void save_to_grid(Block block)
{
   BlockType type = block_types[block.type];
   int(*rotation)[4] = get_rotation(block);
   unsigned color = type.color;

   int first_full = -1, cnt_full = 0;

   // verifica qual é a primeira linha completa e quantas completaram
   // não verifica quando ele completa linhas de forma descontínua (linhas 16, 17 e 19 por exemplo)
   for (int i = 0; i < 4 && block.y + i < 20; i++)
   {
      u8 addedToLine = 0;
      for (int j = 0; j < 4; j++)
      {
         if (block.x + j < 10 && rotation[i][j])
         {
            grid[block.y + i][block.x + j] = rotation[i][j] ? color : 0;
            addedToLine = 1;
         }
      }
      if (addedToLine)
      {

         u8 full_line = 1;
         for (int j = 0; j < 10; j++)
         {
            if (!grid[block.y + i][j])
               full_line = 0;
         }

         if (full_line)
         {
            if (first_full == -1)
               first_full = block.y + i;
            cnt_full++;
         }
      }
   }

   // aparentemente toda a lógica abaixo n ta correta ou o SDL que está bugando

   // apaga linhas completas
   for (int i = 0; i < cnt_full; i++)
   {
      for (int j = 0; j < 10; j++)
      {
         draw_grid_square(first_full + i, j, 0);
      }
   }

   usleep(500000);

   // desce as linhas não completas uma a uma
   for (int cnt = 0; cnt < cnt_full; cnt++)
   {
      for (int i = first_full + cnt - 1; i >= cnt; i--)
      {
         for (int j = 0; j < 10; j++)
         {
            grid[i + 1][j] = grid[i][j];
            draw_grid_square(i, j, 0);
            if (grid[i + 1][j])
               draw_grid_square(i + 1, j, grid[i + 1][j]);
         }
      }
      usleep(500000);
   }
}

// quando o jogador perde
void end_game()
{
}

// realiza operação de teclado no tile atual
// d: direita
// a: esqueda
// w: rotação
// s: acelera a descida
void move_block(Block *block, char dir)
{
   if (dir == 'd' || dir == 'a')
   {
      remove_block(*block);
      block->x += dir == 'd' ? 1 : -1;
      put_block(*block);
   }
}

Queue queue;
u8 read_keys = 1;

static void KeyPressedHandler(const char *pString)
{
   ScreenDeviceWrite(USPiEnvGetScreen(), pString, strlen(pString));
   static unsigned posx = 1;
   for (int i = 0; i < 20; i++)
   {
      for (int j = 0; j < 20; j++)
      {
         ScreenDeviceSetPixel(USPiEnvGetScreen(), posx + i, 200 + j, HIGH_COLOR);
      }
   }

   posx += 20;

   char ch = *pString++;

   // Caracteres validos
   if (read_keys && (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd'))
      enqueue(&queue, ch);
}

u8 run()
{
   Block current_block = create_tile();

   if (check_collision(current_block)) // colidiu antes de spawnar
      return 0;                        // finaliza o jogo

   put_block(current_block); // spawna nova tile

   // uma iteração por movimento vertical da nova tile
   while (1)
   {
      read_keys = 1; // habilita leitura do teclado

      usleep(speed);

      read_keys = 0; // desabilita leitura do teclado

      // Le teclas e move tile
      while (!isQueueEmpty(&queue))
      {
         char ch = dequeue(&queue);
         move_block(&current_block, ch);
      }

      usleep(speed);

      // quando a tile colide no final do frame ou com outra tile
      if (check_collision(current_block))
      {
         // salva no grid e verifica/opera linhas completas
         save_to_grid(current_block);
         return 1; // proxima tile
      }

      // quando não colide, move a tile 1 para baixo

      remove_block(current_block);
      current_block.y++;
      put_block(current_block);
   }
}

void start_game()
{
   // srand(time(0)); // configura seed do rand

   setup(); // antes de adicionar as tiles
   usleep(2 * speed);

   // cada iteração spawna uma nova tile
   // sai do loop quando o jogador perde
   while (run())
      ;

   end_game();
}

int main(void)
{
   if (!USPiEnvInitialize())
   {
      return EXIT_HALT;
   }

   if (!USPiInitialize())
   {
      LogWrite(FromSample, LOG_ERROR, "Cannot initialize USPi");

      USPiEnvClose();

      return EXIT_HALT;
   }

   if (!USPiKeyboardAvailable())
   {
      LogWrite(FromSample, LOG_ERROR, "Keyboard not found");

      USPiEnvClose();

      return EXIT_HALT;
   }

   initQueue(&queue);

   USPiKeyboardRegisterKeyPressedHandler(KeyPressedHandler);

   LogWrite(FromSample, LOG_NOTICE, "Just type something!");

   // just wait and turn the rotor
   // for (unsigned nCount = 0; 1; nCount++)
   // {

   // 	setup();

   // 	// USPiKeyboardUpdateLEDs();
   // 	// ScreenDeviceRotor(USPiEnvGetScreen(), 0, nCount);
   // }

   start_game();

   LogWrite(FromSample, LOG_ERROR, "Game ended!");

   for (unsigned nCount = 0; 1; nCount++)
   {
      USPiKeyboardUpdateLEDs();

      ScreenDeviceRotor(USPiEnvGetScreen(), 0, nCount);
   }

   return EXIT_HALT;
}
