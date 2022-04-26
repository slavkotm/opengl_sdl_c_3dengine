#include "../header/texture.h"

SDL_Texture* create_texture(const char* path,
                            SDL_Surface *surface,
                            SDL_Texture *texture,
                            SDL_Renderer *renderer) 
{
    surface = IMG_Load(path);

    texture = SDL_CreateTextureFromSurface(renderer, 
                                           surface);
    SDL_FreeSurface(surface);

    return texture;
}

SDL_Texture *create_color_texture(const char *path,
                                  SDL_Surface *surface,
                                  SDL_Texture *texture,
                                  SDL_Renderer *renderer,
                                  int red,
                                  int green,
                                  int blue)
{
    surface = IMG_Load(path);

    texture = SDL_CreateTextureFromSurface(renderer, 
                                           surface);

    SDL_SetTextureColorMod(texture, 
                           red, 
                           green, 
                           blue);

    SDL_FreeSurface(surface);

    return texture;
}

void create_text_texture(const char *path,
                                 int font_size,
                                 SDL_Surface *surface,
                                 SDL_Renderer *renderer,
                                 TTF_Font *font,
                                 int red,
                                 int green,
                                 int blue,
                                 int alpha,
                                 const char *text,
                                 SDL_Texture **texture,
                                 SDL_Rect *texture_rectangle)
{
    SDL_Color color;
    font = TTF_OpenFont(path, font_size);
    color.r = red;
    color.g = green;
    color.b = blue;
    color.a = alpha;
    surface = TTF_RenderText_Solid(font, text, color);
    (*texture) = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture((*texture), NULL, NULL, &texture_rectangle->w, &texture_rectangle->h);
    texture_rectangle->x = 100;
    texture_rectangle->y = 100;
    SDL_FreeSurface(surface);
    if((*texture) == NULL)
        printf("AADARFQRQRGERGQEGQERGQERGQERFFSDQEFFEWBFERWGFREWFERWGFREWFE\n");
}
