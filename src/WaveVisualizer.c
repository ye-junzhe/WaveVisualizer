#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fft.c"
#include "plug.h"
#include <raylib.h>

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

typedef struct {
    float left;
    float right;
} Frame;

#define N 256
float in[N];
float complex out[N];
float max_amp;

float amp(float complex z) {
    float a = fabsf(crealf(z));
    float b = fabsf(cimagf(z));
    if (a < b) return b;
    return a;
}

// Param1: Buffer holding the data
// Param2: The data we send into the buffer
void callback(void *bufferData, unsigned int frames)
{
    if (frames < N) return;

    Frame *fs = bufferData;
    for (size_t i = 0; i < frames; ++i) {
        in[i] = fs[i].left;
    }

    fft(in, 1, out, N);

    max_amp = 0.0f;
    for (size_t i = 0; i < frames; ++i) {
        float a = amp(out[i]);
        if (max_amp < a) max_amp = a;
    }
}

int main(void)
{
    plug_hello();

    // return 0;

    InitWindow(800, 600, "Musializer");
    SetTargetFPS(60);

    InitAudioDevice();
    Music music = LoadMusicStream("DeepSpaceB.mp3");
    printf("Framecount: %u\n", music.frameCount);
    printf("SampleRate: %u\n", music.stream.sampleRate);
    printf("SampleSize: %u\n", music.stream.sampleSize);
    printf("Channels  : %u\n", music.stream.channels);

    PlayMusicStream(music);
    SetMusicVolume(music, 0.5f);
    AttachAudioStreamProcessor(music.stream, callback);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

        if (IsKeyPressed(KEY_SPACE)) {
            if (IsMusicStreamPlaying(music)) {
                PauseMusicStream(music);
            } else {
                ResumeMusicStream(music);
            }
        }

        int w = GetRenderWidth();
        int h = GetRenderHeight();

    BeginDrawing();
    ClearBackground(CLITERAL(Color) {0x18, 0x18, 0x18, 0xFF});
        float cell_width = (float)w/N; 
        for (size_t i = 0; i < N; ++i) {
            float t = amp(out[i]);//max_amp;
            DrawRectangle(i*cell_width, h/2- h/2*t, cell_width, h/2*t, YELLOW);
        }
        EndDrawing();
    }
    return 0;
}
