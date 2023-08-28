#ifndef PLUG_H_
#define PLUG_H_

#include <raylib.h>
#include <complex.h>

// #define N 256

typedef struct {
    Music music;
} Plug;

// _t is the convention
typedef void (*plug_hello_t)(void);
typedef void (*plug_init_t)(Plug *plug, const char *file_path);
typedef void (*plug_pre_reload_t)(Plug *plug);
typedef void (*plug_post_reload_t)(Plug *plug);
typedef void (*plug_update_t)(Plug *plug) ;

#endif // !PLUG_H_
