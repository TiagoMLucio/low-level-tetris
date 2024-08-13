#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "bcm.h"

/*
 * Configuração desejada
 */
#define DEPTH 16
// #define DEPTH  24
#define WIDTH 800
#define HEIGHT 600

/**
 * Endereço base do framebuffer
 */
uint8_t *fb = NULL;

/**
 * Tamanho total do framebuffer (bytes)
 */
uint32_t fbsize;
uint16_t width = WIDTH, height = HEIGHT;
uint16_t depth = DEPTH;
uint32_t pitch;

/**
 * Lê uma mensagem da GPU.
 */
uint32_t
gpu_read(int canal)
{
   canal = canal & 0x0f;
   for (;;)
   {
      uint32_t v;
      while (bit_is_set(GPUMAIL_REG(status), 30))
         ;
      v = GPUMAIL_REG(read);
      if ((v & 0x0f) == canal)
         return v >> 4;
   }
}

/**
 * Envia uma mensagem à GPU.
 */
void gpu_write(int canal, uint32_t v)
{
   canal = canal & 0x0f;
   while (bit_is_set(GPUMAIL_REG(status), 31))
      ;
   GPUMAIL_REG(write) = (v << 4) | canal;
}

/**
 * Realiza uma troca de mensagens com a GPU
 */
uint32_t *gpu_msg(int canal, uint32_t *msg)
{
   uint32_t v = (uint32_t)msg;
   canal = canal & 0x0f;

   // aguarda enquanto mbox está "full"
   while (bit_is_set(GPUMAIL_REG(status), 31))
      ;
   GPUMAIL_REG(write) = (v & (~0x0f)) | canal;

   for (;;)
   {
      // aguarda enquanto mbox está "empty"
      while (bit_is_set(GPUMAIL_REG(status), 30))
         ;
      v = GPUMAIL_REG(read);
      if ((v & 0x0f) == canal)
         return (uint32_t *)(v & (~0x0f));
   }
}

/**
 * Buffer de mensagens.
 * Deve ser alinhado em 16 bytes.
 */
uint32_t __attribute__((aligned(16))) msg[32];

/**
 * Altera um pixel na tela
 */
void fb_pixel(unsigned x, unsigned y, unsigned c)
{
   if (fb == NULL)
      return;
   uint32_t pos = y * pitch;
#if DEPTH == 8
   pos += x;
   fb[pos] = c;
#elif DEPTH == 16
   pos += (x << 1);
   fb[pos] = c;
   fb[pos + 1] = c >> 8;
#elif DEPTH == 24
   pos += ((x << 1) + x);
   fb[pos] = c;
   fb[pos + 1] = c >> 8;
   fb[pos + 2] = c >> 16;
#endif
}

/**
 * Inicia a GPU.
 * Retorna true se bem sucedido.
 */
bool init_fb(void)
{
   /*
    * Monta mensagem.
    */
   memset(msg, 0, sizeof(msg));
   msg[0] = 4 * 26;  // tamanho total
   msg[1] = 0;       // request / response
   msg[2] = 0x40001; // alocar buffer
   msg[3] = 8;
   msg[4] = 8;
   msg[5] = 16;
   msg[6] = 0;
   msg[7] = 0x48003; // set physical size
   msg[8] = 8;
   msg[9] = 8;
   msg[10] = width;
   msg[11] = height;
   msg[12] = 0x48004; // set virtual size
   msg[13] = 8;
   msg[14] = 8;
   msg[15] = width;
   msg[16] = height;
   msg[17] = 0x48005; // set depth
   msg[18] = 4;
   msg[19] = 4;
   msg[20] = depth;
   msg[21] = 0x48008; // get pitch
   msg[22] = 4;
   msg[23] = 4;
   msg[24] = 0;
   msg[25] = 0;

   /*
    * Envia mensagem
    */
   gpu_msg(8, msg);
   if (msg[1] != 0x80000000)
      return false;

   /*
    * Processa resposta.
    */
   fb = (uint8_t *)(msg[5] & 0x3fffffff);
   fbsize = msg[6];
   width = msg[10];
   height = msg[11];
   depth = msg[20];
   pitch = msg[24];

   /*
    * Limpa o framebuffer
    */
   memset(fb, 0, fbsize);
   return true;
}

/**
 * Monta uma representação de 16 bits para a cor desejada.
 */
uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b)
{
   if (depth == 16)
   {
      r = r >> (8 - 5);
      g = (g >> (8 - 6)) & 0b111111;
      b = (b >> (8 - 5)) & 0b11111;
      uint16_t res = r;
      res = (res << 5) | g;
      return (res << 6) | b;
   }
   return (r << 16) | (g << 8) | b;
}
