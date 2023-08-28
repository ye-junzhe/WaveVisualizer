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

char *shift_args(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = (**argv);
    (*argv) += 1;
    (*argc) -= 1;
    return result;
}


const char *libplug_file_name = "./bin/libplug.dylib";
void *libplug = NULL;

// This technique is called X Macro
// ## == concatenate
#ifdef HOTRELOAD
#define PLUG(name) name##_t *name = NULL;
#else
#define PLUG(name) name##_t name;
#endif
LIST_OF_PLUGS
#undef PLUG

Plug plug = {0};

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
    #define PLUG(name) \
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

int main(int argc, char **argv)
{
    if (!reload_libplug()) return 1;

    const char *program = shift_args(&argc, &argv);

    // TODO: supply input files via drag&drop
    if (argc == 0) {
        fprintf(stderr, "Usage: %s <input>\n", program);
        fprintf(stderr, "ERROR: no input file is provided\n");
        return 1;
    }
    const char *file_path = shift_args(&argc, &argv);

    InitWindow(800, 600, "Musializer");
    SetTargetFPS(60);
    InitAudioDevice();

    plug_init(&plug, file_path);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            plug_pre_reload(&plug);
            if (!reload_libplug()) return 1;
            plug_post_reload(&plug);
        }
        plug_update(&plug);
    }

    return 0;
}
