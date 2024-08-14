#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h> // Include time.h for time
#define pack_color(r, g, b) (unsigned)((r << 16) | (g << 8) | b)

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
   printf("Enqueued %c\n", item);
   if (isQueueFull(q))
   {
      printf("Queue is full!\n");
      return;
   }
   q->data[q->rear] = item;
   q->rear = (q->rear + 1) % QUEUE_SIZE;
   q->size++;
}

// Dequeue an element
char dequeue(Queue *q)
{
   if (isQueueEmpty(q))
   {
      printf("Queue is empty!\n");
      return '\0';
   }
   char item = q->data[q->front];
   q->front = (q->front + 1) % QUEUE_SIZE;
   q->size--;
   return item;
}

Queue queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

bool read_keys = true;

// Thread function to read from the keyboard and add to the queue
void *readFromKeyboard(void *arg)
{
   char ch;
   printf("Start reading from keyboard\n");
   while (1)
   {
      printf("Waiting for keyboard\n");
      ch = getchar();

      // Caracteres validos
      if (read_keys && (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd'))
      {
         enqueue(&queue, ch);
      }
   }
   return NULL;
}

// bool read_input_enabled = false;
// pthread_mutex_t input_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t input_cond = PTHREAD_COND_INITIALIZER;

// void *timer(void *arg)
// {
//    while (1)
//    {
//       printf("Start timer\n");
//       pthread_mutex_lock(&input_mutex);

//       // Wait for the main thread to signal activation
//       pthread_cond_wait(&input_cond, &input_mutex);

//       read_input_enabled = true;
//       pthread_mutex_unlock(&input_mutex);
//       pthread_cond_signal(&input_cond);
//       pthread_mutex_lock(&input_mutex);

//       pthread_mutex_unlock(&input_mutex);

//       usleep(200000); // Duration for which input is allowed (e.g., 0.2 seconds)

//       pthread_mutex_lock(&input_mutex);
//       read_input_enabled = false;
//       pthread_mutex_unlock(&input_mutex);
//    }
//    return NULL;
// }

// Trocar pela tela especificada na biblioteca USPi.
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

SDL_Renderer *renderer;

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

// Desenha pixel
void fb_pixel(int x, int y, unsigned color)
{
   SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
   SDL_RenderDrawPoint(renderer, x, y);
}

void draw_line(int x, int y, int length, unsigned color, int thickness, bool vertical)
{
   for (int i = 0; i < thickness; i++)
      for (int j = 0; j < length; j++)
         fb_pixel(x + (vertical ? i : j), y + (vertical ? j : i), color);
}

void draw_rect(int x, int y, int width, int height, int thickness, unsigned color)
{
   draw_line(x, y, height + 2 * thickness, color, thickness, true);                      // left
   draw_line(x + width + thickness, y, height + 2 * thickness, color, thickness, true);  // right
   draw_line(x, y, width + 2 * thickness, color, thickness, false);                      // top
   draw_line(x, y + height + thickness, width + 2 * thickness, color, thickness, false); // bottom
}

void draw_frame()
{
   unsigned white = pack_color(255, 255, 255);
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

   unsigned black = pack_color(0, 0, 0);

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

bool check_collision(Block block)
{
   printf("Checking collision\n");
   int(*rotation)[4] = get_rotation(block);
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         if (rotation[i][j] && (grid[block.y + 1 + i][block.x + j] || block.y + 1 + i >= 20))
            return true;
   return false;
}

bool check_lateral_collision(int blockX, int blockY, int i, int j, char ch) {
   return (ch == 'a') ? (blockX + j - 1 < 0 || grid[blockY + i][blockX + j - 1]) : 
                        (blockX + 1 + j >= 10 || grid[blockY + i][blockX + j + 1]);
}

// The tile is not in the grid yet, so it does not collide with itself.
bool check_horizontal_collision(Block block, char ch)
{
   // printf("Checking collision for char %c\n", ch);
   int(*rotation)[4] = get_rotation(block);
   
   for (int i = 0; i < 4; i++) {      
      for (int j = 0; j < 4; j++)
         if (rotation[i][j] && check_lateral_collision(block.x, block.y, i, j, ch)) {
            // printf("Collision left\n");
            return true;
         }
   }      
   
   return false;
}

Block create_tile()
{
   Block current_block;
   current_block.type = rand() % 7; // Random tile
   current_block.rotation = 0;
   current_block.x = 3; // Starting X position (middle of the frame)
   current_block.y = 0; // Starting Y position (top of the frame)

   return current_block;
}

void setup()
{
   draw_frame();
   // draw_grid(); // debugging
}

// salva no grid quando tile colide
void save_to_grid(Block block)
{
   BlockType type = block_types[block.type];
   int(*rotation)[4] = get_rotation(block);
   unsigned color = type.color;

   printf("Saving to grid\n");
   int first_full = -1, cnt_full = 0;

   // verifica qual é a primeira linha completa e quantas completaram
   // não verifica quando ele completa linhas de forma descontínua (linhas 16, 17 e 19 por exemplo)
   for (int i = 0; i < 4 && block.y + i < 20; i++)
   {
      bool addedToLine = false;
      for (int j = 0; j < 4; j++)
      {
         if (block.x + j < 10 && rotation[i][j])
         {
            grid[block.y + i][block.x + j] = rotation[i][j] ? color : 0;
            addedToLine = true;
         }
      }
      if (addedToLine)
      {

         bool full_line = true;
         for (int j = 0; j < 10; j++)
         {
            if (!grid[block.y + i][j])
               full_line = false;
         }

         if (full_line)
         {
            if (first_full == -1)
               first_full = block.y + i;
            cnt_full++;
         }
      }
   }

   printf("Block saved to grid\n");
   printf("%d lines cleared\n", cnt_full);
   printf("First line cleared is: %d\n", first_full);

   // aparentemente toda a lógica abaixo n ta correta ou o SDL que está bugando

   // apaga linhas completas
   for (int i = 0; i < cnt_full; i++)
   {
      for (int j = 0; j < 10; j++)
      {
         draw_grid_square(first_full + i, j, 0);
      }
   }

   SDL_RenderPresent(renderer); // remover para rpi
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
      SDL_RenderPresent(renderer); // remover para rpi
      usleep(500000);
   }
}

// quando o jogador perde
void end_game()
{
   printf("DONE\n");
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

bool run()
{
   Block current_block = create_tile();
   SDL_RenderPresent(renderer); // remover para rpi

   if (check_collision(current_block)) // colidiu antes de spawnar
      return false;                    // finaliza o jogo

   put_block(current_block); // spawna nova tile

   // uma iteração por movimento vertical da nova tile
   while (1)
   {
      read_keys = true; // habilita leitura do teclado

      usleep(speed);

      read_keys = false; // desabilita leitura do teclado

      // Le teclas e move tile
      while (!isQueueEmpty(&queue))
      {
         char ch = dequeue(&queue);
         if (ch == 'w') {
            printf("rotate: TO DO\n");
         } else {
            if (!check_horizontal_collision(current_block, ch)) {
               move_block(&current_block, ch);
            } else {
               printf("cant move due to collision\n");
            }        
         }      
      }

      usleep(speed);

      // quando a tile colide no final do frame ou com outra tile
      if (check_collision(current_block))
      {
         // salva no grid e verifica/opera linhas completas
         save_to_grid(current_block);
         return true; // proxima tile
      }

      // quando não colide, move a tile 1 para baixo

      remove_block(current_block);
      current_block.y++;
      put_block(current_block);

      SDL_RenderPresent(renderer); // remover para rpi
   }
}

void *game(void *arg)
{
   srand(time(NULL)); // configura seed do rand

   // configurações da tela SDL somente para desenvolvimento
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
      fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

   SDL_Window *window = SDL_CreateWindow("Low Level Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
   if (window == NULL)
   {
      fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
      SDL_Quit();
   }

   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   if (renderer == NULL)
   {
      fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
      SDL_DestroyWindow(window);
      SDL_Quit();
   }

   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear the screen with black
   SDL_RenderClear(renderer);

   // ===== Main code =====

   setup(); // antes de adicionar as tiles
   usleep(2 * speed);

   // cada iteração spawna uma nova tile
   // sai do loop quando o jogador perde
   while (run())
      ;

   end_game();

   // =====================


   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();

   return NULL;
}

int main(void)
{
   pthread_t keyboardThread, gameThread;

   initQueue(&queue);

   pthread_create(&gameThread, NULL, game, NULL);
   pthread_create(&keyboardThread, NULL, readFromKeyboard, NULL);

   pthread_join(gameThread, NULL);
   pthread_join(keyboardThread, NULL);

   return 0;
}
