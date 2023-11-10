#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <complex.h>
#include <math.h>

#include <raylib.h>

#include <dlfcn.h>

#include "plug.h"

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

const char *libplug_file_name = "./bin/libplug.dylib";
void *libplug = NULL;

// This technique is called X Macro
// ## == concatenate
#ifdef HOTRELOAD
#define PLUG(name, ...) name##_t *name = NULL;
#else
#define PLUG(name, ...) name##_t name;
#endif
LIST_OF_PLUGS
#undef PLUG

#ifdef HOTRELOAD
bool reload_libplug(void)
{
    if (libplug != NULL) dlclose(libplug);

    // Load and link a DLL to create a handle
    libplug = dlopen(libplug_file_name, RTLD_NOW);
    if (libplug == NULL) {
        fprintf(stderr, "ERROR: could not load %s: %s", libplug_file_name, dlerror());
        return false;
    }

    // dlsym() returns the address of the code or data location specified by the null-terminated
    // character string symbol.  Which libraries and bundles are searched depends on the handle parameter.
    #define PLUG(name, ...) \
    name = dlsym(libplug, #name); \
    if (name == NULL) { \
        fprintf(stderr, "ERROR: could not find $s symbol in %s: %s", \
                #name, libplug_file_name, dlerror()); \
        return false; \
    }
    LIST_OF_PLUGS
    #undef PLUG

    return true;
}

#else
#define reload_libplug() true
#endif

int main(void)
{
    if (!reload_libplug()) return 1;

    size_t factor = 60;
    InitWindow(factor*16, factor*9, "WaveVisializer");
    SetTargetFPS(60);
    InitAudioDevice();

    plug_init();
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            void *state = plug_pre_reload();
            if (!reload_libplug()) return 1;
            plug_post_reload(state);
        }
        plug_update();
    }

    return 0;
}
