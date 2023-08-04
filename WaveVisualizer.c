#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "D:\msys2\mingw64\include\raylib.h"

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

uint32_t global_frames[4800] = {0};
size_t global_frames_count = 0;

// Param1: Buffer holding the data
// Param2: The data we send into the buffer
void callback(void *bufferData, unsigned int frames)
{
    size_t capacity = ARRAY_LEN(global_frames);
    // First scenario: the frames can fit into the remaining memory
    if (frames <= capacity - global_frames_count)
    {
        memcpy(global_frames + global_frames_count, bufferData, sizeof(uint32_t)*frames);
        global_frames_count += frames;
        return;
    }
    // Second scenario: the frames can't fit into the remaining memory,
    // so we need to move the memory after the frames' end address to the start address of the buffer
    // and then copy the new frames to where the frames' end address located in the buffer after the move
    else if (frames <= capacity)
    {
        memmove(global_frames, global_frames + sizeof(uint32_t)*frames, sizeof(uint32_t)*(capacity - frames));
        memcpy(global_frames + (capacity - frames), bufferData, sizeof(uint32_t)*frames);
    } else
    // Third scenario: the frames are larger then the bufferData
    // Only cpy the buffer-sized of the frames into the memory
    {
        memcpy(global_frames, bufferData, sizeof(uint32_t)*capacity);
        global_frames_count = capacity;
    }

    if (frames > ARRAY_LEN(global_frames)) frames = ARRAY_LEN(global_frames);

    memcpy(global_frames, bufferData, sizeof(uint32_t)*frames);
    global_frames_count = frames;
}

int main(void)
{
    InitWindow(800, 600, "Musializer");
    SetTargetFPS(60);

    InitAudioDevice();
    Music music = LoadMusicStream("DeepSpaceB.mp3");
    assert(music.stream.sampleSize == 16);
    assert(music.stream.channels == 2);
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
        float cell_width = (float)GetRenderWidth()/global_frames_count; 
        for (size_t i = 0; i < global_frames_count; ++i) {
            int16_t sample = *(int16_t*)&global_frames[i];
            float t = (float)sample/INT16_MAX;
            if (sample < 0) {
                DrawRectangle(i*cell_width, h/2 - h/2*t, cell_width, h/2*t, RED);
            } else {
                float t = (float)sample/INT16_MIN;
                DrawRectangle(i*cell_width, h/2, cell_width, h/2*t, RED);
            }
        }
        printf("\n");
        EndDrawing();
    }
    return 0;
}