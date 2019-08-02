#include "gfx.h"

#include <SDL2/SDL_image.h>

const int dither[] = { 0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5 };
const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ      " "0123456789.,!?'\"-+=/\\%()<>:;     ";

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

SDL_Color palette[256];
SDL_Color* pixels;

int focus = 0;

int getcolor1(int d) {
 int r, g, b;
 
 if (d < 0) {
  return 0xff;
 }
 
 r = d / 100 % 10;
 g = d / 10 % 10;
 b = d / 1 % 10;
 
 return r * 36 + g * 6 + b;
}

int getcolor(int a, int b, int c, int d) {
 return (getcolor1(d) << 24) + (getcolor1(c) << 16) + (getcolor1(b) << 8) + getcolor1(a);
}

void loadspritesheet(spritesheet_t* sheet, const char* path) {
 SDL_Surface* source;
 SDL_Surface* surface;
 int* colors;
 int i;
 
 source = IMG_Load(path);
 
 if (!source) {
  LOGREPORT("could not load '%s' - %s.", path, IMG_GetError());
  return;
 }
 
 surface = SDL_ConvertSurfaceFormat(source, SDL_PIXELFORMAT_ARGB8888, 0);
 
 if (!surface) {
  LOGREPORT("could not convert surface for '%s' - %s.", path, SDL_GetError());
  return;
 }
 
 SDL_FreeSurface(source);
 
 sheet->width = surface->w;
 sheet->height = surface->h;
 
 sheet->pixels = malloc(sizeof(int) * sheet->width * sheet->height);
 
 colors = (int*) surface->pixels;
 
 for (i = 0; i < sheet->width * sheet->height; i++) {
  sheet->pixels[i] = (colors[i] & 0xff) / 64;
 }
 
 SDL_FreeSurface(surface);
}

void createscreen(screen_t* screen, int width, int height, const char* icons) {
 screen->x = screen->y = 0;
 screen->w = width;
 screen->h = height;
 
 screen->pixels = malloc(sizeof(color_t) * width * height);
 
 loadspritesheet(&screen->sheet, icons);
 
 return;
}

void clearscreen(screen_t* screen, int color) {
 for (int i = 0; i < screen->w * screen->h; i++) {
  screen->pixels[i] = color;
 }
}

void offsetscreen(screen_t* screen, int x, int y) {
 screen->x = x;
 screen->y = y;
}

void rendersprite(int xp, int yp, int tile, int color, int bits, screen_t* screen) {
 int mx, my;
 int x, y, to, tx, ty, xs, ys;
 int target;
 
 xp -= screen->x;
 yp -= screen->y;
 
 mx = bits & BIT_MIRROR_X;
 my = bits & BIT_MIRROR_Y;
 
 tx = tile % 32;
 ty = tile / 32;
 to = tx * 8 + ty * 8 * screen->sheet.width;
 
 for (y = 0; y < TILESCALE; y++) {
  if (y + yp < 0 || y + yp >= screen->h) {
   continue;
  }
  
  ys = y;
  
  if (my) {
   ys = 7 - y;
  }
  
  for (x = 0; x < TILESCALE; x++) {
   if (x + xp < 0 || x + xp >= screen->w) {
	continue;
   }
   
   xs = x;
   
   if (mx) {
	xs = 7 - x;
   }
   
   target = (color >> (screen->sheet.pixels[xs + ys * screen->sheet.width + to] * 8)) & 255;
   
   if (target < 255) {
	screen->pixels[(x + xp) + (y + yp) * screen->w] = target;
   }
  }
 }
}

void renderlight(int x, int y, int r, screen_t* screen) {
 int x0, x1, y0, y1;
 int yd, xd, yy, xx;
 int len, br;
 
 x -= screen->x;
 y -= screen->y;
 
 x0 = x - r;
 x1 = x + r;
 y0 = y - r;
 y1 = y + r;
 
 if (x0 < 0) {
  x0 = 0;
 }
 if (y0 < 0) {
  y0 = 0;
 }
 if (x1 > screen->w) {
  x1 = screen->w;
 }
 if (y1 > screen->h) {
  y1 = screen->h;
 }
 
 for (yy = y0; yy < y1; yy++) {
  yd = yy - y;
  yd = yd * yd;
  
  for (xx = x0; xx < x1; xx++) {
   xd = xx - x;
   len = xd * xd + yd;
   
   if (len <= r * r) {
	br = 255 - len * 255 / (r * r);
	
	if (screen->pixels[xx + yy * screen->w] < br) {
	 screen->pixels[xx + yy * screen->w] = br;
	}
   }
  }
 }
}

void renderframe(const char* title, int x0, int y0, int x1, int y1, screen_t* screen) {
 for (int y = y0; y <= y1; y++) {
  for (int x = x0; x <= x1; x++) {
   if (x == x0 && y == y0) {
	rendersprite(x * 8, y * 8, 0 + 13 * 32, getcolor(-1, 1, 5, 445), 0, screen);
   }
   else if (x == x1 && y == y0) {
	rendersprite(x * 8, y * 8, 0 + 13 * 32, getcolor(-1, 1, 5, 445), 1, screen);
   }
   else if (x == x0 && y == y1) {
	rendersprite(x * 8, y * 8, 0 + 13 * 32, getcolor(-1, 1, 5, 445), 2, screen);
   }
   else if (x == x1 && y == y1) {
	rendersprite(x * 8, y * 8, 0 + 13 * 32, getcolor(-1, 1, 5, 445), 3, screen);
   }
   else if (y == y0) {
	rendersprite(x * 8, y * 8, 1 + 13 * 32, getcolor(-1, 1, 5, 445), 0, screen);
   }
   else if (y == y1) {
	rendersprite(x * 8, y * 8, 1 + 13 * 32, getcolor(-1, 1, 5, 445), 2, screen);
   }
   else if (x == x0) {
	rendersprite(x * 8, y * 8, 2 + 13 * 32, getcolor(-1, 1, 5, 445), 0, screen);
   }
   else if (x == x1) {
	rendersprite(x * 8, y * 8, 2 + 13 * 32, getcolor(-1, 1, 5, 445), 1, screen);
   }
   else {
	rendersprite(x * 8, y * 8, 2 + 13 * 32, getcolor(5, 5, 5, 5), 1, screen);
   }
  }
 }
 
 renderfont(title, x0 * 8 + 8, y0 * 8, getcolor(5, 5, 5, 550), screen);
}

void renderfont(const char* message, int x, int y, int color, screen_t* screen) {
 int i, ix;
 
 for (i = 0; i < strlen(message); i++) {
  ix = (int) (strchr(chars, toupper(message[i])) - chars);
  
  if (ix > -1) {
   rendersprite(x + i * 8, y, ix + 30 * 32, color, 0, screen);
  }
 }
}

void overlayscreens(screen_t* screen, screen_t* other, int xa, int ya) {
 int i, x, y;
 
 i = 0;
 
 for (y = 0; y < screen->h; y++) {
  for (x = 0; x < screen->w; x++) {
   if (other->pixels[i] / 10 <= dither[((x + xa) & 3) + ((y + ya) & 3) * 4]) {
	screen->pixels[i] = 0;
   }
   
   i++;
  }
 }
}

void createview() {
 window = SDL_CreateWindow(GAMETITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RENDERWIDTH * SCALE, RENDERHEIGHT * SCALE, SDL_WINDOW_SHOWN);
 
 SDLNULLPRONE(window, "'SDL2' window creation failed");
 
 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
 
 SDLNULLPRONE(renderer, "'SDL2' renderer creation failed");
 
 texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDERWIDTH, RENDERHEIGHT);
 
 SDLNULLPRONE(texture, "'SDL2' texture creation failed");
 
 pixels = malloc(sizeof(SDL_Color) * RENDERWIDTH * RENDERHEIGHT);
 
 NULLPRONE(pixels, "failed allocating memory for pixels");
}

void closeview() {
 free(pixels);
 
 SDL_DestroyTexture(texture);
 SDL_DestroyRenderer(renderer);
 SDL_DestroyWindow(window);
}

void fillpalette() {
 int pp, r, rr, g, gg, b, bb, mid, r1, g1, b1;
 
 pp = 0;
 
 for (r = 0; r < 6; r++) {
  rr = (r * 255 / 5);
  
  for (g = 0; g < 6; g++) {
   gg = (g * 255 / 5);
   
   for (b = 0; b < 6; b++) {
	bb = (b * 255 / 5);
	
	mid = (rr * 30 + gg * 59 + bb * 11) / 100;
	
	r1 = ((rr + mid * 1) / 2) * 230 / 255 + 10;
	g1 = ((gg + mid * 1) / 2) * 230 / 255 + 10;
	b1 = ((bb + mid * 1) / 2) * 230 / 255 + 10;
	
	palette[pp].r = b1;
	palette[pp].g = g1;
	palette[pp].b = r1;
	palette[pp].a = 0;
	
	pp++;
   }
  }
 }
}

int hasfocus() {
 return focus;
}

void updatetexture(screen_t* screen) {
 SDL_Color* t_pixels;
 int cc, pitch, x, y;
 
 for (y = 0; y < screen->h; y++) {
  for (x = 0; x < screen->w; x++) {
   cc = screen->pixels[x + y * screen->w];
   
   if (cc < 255) {
	pixels[x + y * RENDERWIDTH] = palette[cc];
   }
  }
 }
 
 if (SDL_LockTexture(texture, NULL, (void**) &t_pixels, &pitch)) {
  LOGREPORT("SDL_LockTexture failed - %s.", SDL_GetError());
  exit(EXIT_FAILURE);
 }
 
 memcpy(t_pixels, pixels, sizeof(SDL_Color) * RENDERWIDTH * RENDERHEIGHT);
 
 SDL_UnlockTexture(texture);
 
 SDL_RenderClear(renderer);
 SDL_RenderCopy(renderer, texture, NULL, NULL);
 SDL_RenderPresent(renderer);
}
