/*
*  Genera líneas y sonido
*  apt install libsdl2-dev
*  pacman -S sdl2
* Compilar con gcc sslines.c -o sslines -lSDL2 -lm
*/

#include <SDL2/SDL.h>

#define PROGRAM     "SSLines"
#define EXECUTABLE  "sslines"
#define DESCRIPTION "SDL Demo. Generates lines and sound."
#define VERSION     "1.0b"
#define AUTHOR      "Manuel Domínguez López"
#define MAIL        "zqbzybc@tznvy.pbz"
#define URL         "https://github.com/mdomlop/sslines"
#define LICENSE     "GPLv3+"

#define M_PI 3.14159265358979323846

#define WINDOWTITLE "SSLines by mdomlop"

#define RATEFREQ 44100 /* Frecuencia de muestreo */
#define NSAMPLES 2048  /* Número de muestras */
#define CHANNELS 1     /* Número de canales de audio */

#define randomize (srand(time(NULL)))
#define random(limit) (rand() % (limit))

int width = 640;
int height = 480;

short fullscreen = 0;
short mute = 0;
short rnd = 1;
short idx = 0;
short bgidx = 0;

short timeout = 100;  /* Para borrar la pantalla */

float frequency;
int duration;
int duration_incr = 100;

struct choord {
	short x;
	short y;
} origin, destination = { 0, 0 };

struct rgb {
	short r;
	short g;
	short b;
} color = { 0, 0, 0 };

struct colornote {
	char *note;
	float freq;
	char *color;
	short r;
	short g;
	short b;
} cntab[] = {
	{ "Do4",  261.626, "Negro",    0,   0,   0 },
	{ "Re4",  293.665, "Gris",     125, 125, 125 },
	{ "Mi4",  329.628, "Blanco",   255, 255, 255 },
	{ "Fa4",  349.228, "Rojo",     255, 0,   0 },
	{ "Sol4", 391.995, "Naranja",  255, 125, 0 },
	{ "La4",  440.000, "Amarillo", 255, 255, 0 },
	{ "Si4",  493.883, "Verde",    0,   255, 0 },
	{ "Do5",  523.251, "Cian",     0,   255, 255 },
	{ "Re5",  587.330, "Azul",     0,   0,   255 },
	{ "Mi5",  659.255, "Magenta",  255, 0,   255 },
};

SDL_AudioSpec audioSpec;

void play_sound()
{
    int nsamples;

    nsamples = (audioSpec.freq * duration) / 1000;
    Uint16 *samples = (Uint16 *)malloc(sizeof(Uint16) * nsamples);

    for (int i = 0; i < nsamples; i++)
    {
        double time = (double)i / audioSpec.freq;
        double angle = 2.0 * M_PI * frequency * time;
        samples[i] = (Uint16)(32767 * sin(angle));
    }

    SDL_QueueAudio(1, samples, sizeof(Uint16) * nsamples);
    SDL_PauseAudio(0); /* Play!! */

    SDL_Delay(duration);
    SDL_PauseAudio(1); /* Stop!! */
    free(samples);
}

void rnd_color(void)
{
    short max = 255;
    color.r = random(max);
    color.g = random(max);
    color.b = random(max);
}

void set_bgcolor(SDL_Renderer *canvas)
{
    SDL_SetRenderDrawColor(canvas,
                           cntab[bgidx].r, cntab[bgidx].g, cntab[bgidx].b, 255);
    SDL_RenderClear(canvas);
}

int dist_calc(void)
{
    int dx = destination.x - origin.x;
    int dy = destination.y - origin.y;

    return (int)sqrt(dx * dx + dy * dy);
}

void cls(SDL_Renderer *canvas, short x)
{
    short n = random(x); /* En qué momento se limpia la pantalla */
    if (n == 1)
    {
        set_bgcolor(canvas);
    }
}

void drawline(SDL_Renderer *canvas)
{
    SDL_SetRenderDrawColor(canvas, color.r, color.g, color.b, 255);
    SDL_RenderDrawLine(canvas,
                       origin.x, origin.y, destination.x, destination.y);
}

void help(void)
{
    printf("\n****************************\n"
           "* SSlines demo por mdomlop *\n"
           "****************************\n\n"
           "* Presiona ESCAPE o Q para salir.\n"
           "* Presiona ESPACIO o P para pausar la demo.\n"
           "* Presiona M para desactivar o activar el sonido.\n"
           "* Presiona INTRO o F para intercambiar"
           " a pantalla completa.\n"
           "* Presiona CONTROL o C para limpiar la pantalla.\n"
           "* Presiona TABULADOR o R para intercambiar el modo aleatorio.\n"
           "* Presiona ARRIBA/ABAJO o A/S para cambiar la velocidad.\n"
           "* Presiona IZQ/DER o B/N para cambiar el color del fondo.\n"
           "También puedes usar los números del 0 al 9.\n"
           "* Presiona F1 o H"
           " para volver a ver este mensaje.\n");
}

int main(int argc, char *argv[])
{
    /* Iniciar vídeo */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    /* La ventana */
    SDL_Window *window = SDL_CreateWindow(WINDOWTITLE,
		    SDL_WINDOWPOS_UNDEFINED,
		    SDL_WINDOWPOS_UNDEFINED,
		    width, height,
		    SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        fprintf(stderr, "Error al crear la ventana: %s\n", SDL_GetError());
        return 1;
    }

    /* Crea un área de dibujo en la ventana (pantalla) */
    SDL_Renderer *canvas = SDL_CreateRenderer(window, -1,
                                              SDL_RENDERER_ACCELERATED);
    if (canvas == NULL)
    {
        fprintf(stderr, "Error al crear el lienzo: %s\n", SDL_GetError());
        return 1;
    }

    /* Iniciar audio */
    SDL_Init(SDL_INIT_AUDIO);

    audioSpec.freq = RATEFREQ;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = CHANNELS;
    audioSpec.samples = NSAMPLES;
    audioSpec.callback = NULL;

    SDL_OpenAudio(&audioSpec, NULL);

    int quit = 0;
    SDL_Event e;

    help();

    while (!quit)
    {
        int distance;

        origin.x = destination.x;
        origin.y = destination.y;
        destination.x = random(width);
        destination.y = random(height);

        distance = dist_calc();

        if (rnd)
        {
            frequency = (random(63) + 37) * 10; /* 37 ~ 100 */
	    duration = (random(9) + 1) * 10 + duration_incr;    /* 1 ~ 10 */
            rnd_color();
        }
        else
        {
            idx = random(10); /* Para conocer la nota tocada. */
            frequency = cntab[idx].freq;
            duration = distance + duration_incr; /* = longitud de la línea. */
            color.r = cntab[idx].r;
            color.g = cntab[idx].g;
            color.b = cntab[idx].b;
        }

        drawline(canvas);

        if (mute) SDL_Delay(duration);
        else play_sound();

        cls(canvas, timeout);      /* Borra la pantalla aleatoriamente */

        SDL_RenderPresent(canvas); /* Actualiza la pantalla */

        while (SDL_PollEvent(&e) != 0)
        {
            set_bgcolor(canvas);
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_SPACE:
                case SDLK_p:
                    puts("Programa pausado."
                         " Pulsa cualquier tecla"
                         " para continuar.");
                    int key_pressed = 0;
                    while (!key_pressed)
                    {
                        while (SDL_PollEvent(&e) != 0)
                        {
                            if (e.type == SDL_KEYDOWN)
                            {
                                key_pressed = 1;
                                puts("Continuando la ejecución");
                            }
                        }
                    }
                    break;
                case SDLK_ESCAPE:
                case SDLK_q:
                    quit = 1;
                    break;
		case SDLK_RCTRL:
		case SDLK_LCTRL:
		case SDLK_c:
		    cls(canvas, 1);
		    break;
                case SDLK_RETURN:
                case SDLK_f:
                    if (fullscreen)
                    {
                        SDL_SetWindowFullscreen(window, 0);
                        fullscreen = 0;
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                        fullscreen = 1;
                    }
                    break;
                case SDLK_m:
                    if (mute)
                    {
                        puts("Sonido encendido.");
                        mute = 0;
                    }
                    else
                    {
                        puts("Sonido apagado.");
                        mute = 1;
                    }
                    break;
                case SDLK_TAB:
                case SDLK_r:
                    if (rnd)
                    {
                        rnd = 0;
                        puts("Modo aleatorio apagado.");
                    }
                    else
                    {
                        rnd = 1;
                        puts("Modo aleatorio encendido.");
                    }
                    break;
                case SDLK_0:
                    bgidx = 0;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_1:
                    bgidx = 1;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_2:
                    bgidx = 2;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_3:
                    bgidx = 3;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_4:
                    bgidx = 4;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_5:
                    bgidx = 5;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_6:
                    bgidx = 6;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_7:
                    bgidx = 7;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_8:
                    bgidx = 8;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_9:
                    bgidx = 9;
		    printf("%d: %s\n", bgidx, cntab[bgidx].color);
                    break;
                case SDLK_F1:
                case SDLK_h:
                    help();
                    break;
                case SDLK_UP:
                case SDLK_a:  /* add */
                    if (duration_incr < 1000)
                        duration_incr += 1;
                    duration_incr += 1;
                    printf("Duración del sonido: %d (incr: %d)\n",
                           duration, duration_incr);
                    break;
                case SDLK_DOWN:
                case SDLK_s:  /* substract */
                    if (duration_incr > 0)
                        duration_incr -= 1;
                    printf("Duración del sonido: %d (incr: %d)\n",
                           duration, duration_incr);
                    break;
                case SDLK_RIGHT:
                case SDLK_n:  /* next color */
                    if (bgidx < 9)
                        bgidx++;
                    else
                        bgidx = 0;
                    break;
                case SDLK_LEFT:
                case SDLK_b:  /* previous (back) color */
                    if (bgidx > 0)
                        bgidx--;
                    else
                        bgidx = 9;
                    break;
                default:
                    break;
                }
            }
        }
    }

    /* Aquí se acaba todo */
    SDL_CloseAudio();
    SDL_DestroyRenderer(canvas);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("\nGRACIAS POR JUGAR.\n");
    return 0;
}
