#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raylib.h"

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

typedef struct {
    float left;
    float right;
} Frame;

Frame global_frames[4800*2] = {0};
size_t global_frames_count = 0;

// Param1: Buffer holding the data
// Param2: The data we send into the buffer
void callback(void *bufferData, unsigned int frames)
{
    size_t capacity = ARRAY_LEN(global_frames);
    // First scenario: the frames can fit into the remaining memory
    if (frames <= capacity - global_frames_count) {
        memcpy(global_frames + global_frames_count, bufferData, sizeof(Frame)*frames);
        global_frames_count += frames;
        return;
    }
    // Second scenario: the frames can't fit into the remaining memory,
    // so we need to move the memory after the frames' end address to the start address of the buffer
    // and then copy the new frames to where the frames' end address located in the buffer after the move
    else if (frames <= capacity) {
        memmove(global_frames, global_frames + sizeof(Frame)*frames, sizeof(Frame)*(capacity - frames));
        memcpy(global_frames + (capacity - frames), bufferData, sizeof(Frame)*frames);
    }
    // Third scenario: the frames are larger then the bufferData
    // Only cpy the buffer-sized of the frames into the memory
    else {
        memcpy(global_frames, bufferData, sizeof(Frame)*capacity);
        global_frames_count = capacity;
    }

    if (frames > ARRAY_LEN(global_frames)) frames = ARRAY_LEN(global_frames);

    memcpy(global_frames, bufferData, sizeof(Frame)*frames);
    global_frames_count = frames;
}

int main(void)
{
    InitWindow(1000, 800, "Musializer");
    SetTargetFPS(60);

    InitAudioDevice();
    Music music = LoadMusicStream("DeepSpaceB.mp3");
    // assert(music.stream.sampleSize == 32);
    // assert(music.stream.channels == 2);
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
        float cell_width = (float)w/global_frames_count; 
        for (size_t i = 0; i < global_frames_count; ++i) {
            float t = global_frames[i].left;
            if (t > 0) {
                DrawRectangle(i*cell_width, h/2 - h/2*t, 1, h/2*t, RED);
            } else {
                DrawRectangle(i*cell_width, h/2, 1, h/2*t, RED);
            }
        }
        EndDrawing();
    }
    return 0;
}
