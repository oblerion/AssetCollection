#include "raylib.h"
#include "AssetCollection.h"
int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------
   // float fraq = GetMonitorWidth(0)/GetMonitorHeight(0);
    const int screenWidth = 1000; //GetMonitorWidth(0);
    const int screenHeight = 600;//GetMonitorHeight(0);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //Font f = LoadFont(".ttf");
    // struct WaveCollection* wc = NULL;
    // wc = add_WaveCollection(wc,"asset/boop.mp3");
    // wc = add_WaveCollection(wc,"asset/wafb_fill_acoustic.mp3");
    // printf("\n save : %d ",save_WavesToBinary("test.asset",wc));
    // printf("\n count: %d", wc->count);

    struct AssetCollection* ac = init_AssetCollection();
    //ac = add_AssetCollection(ac,"asset/boop.mp3");
    //save_AssetCollection("test.asset",ac);
    ac = load_AssetCollection("test.asset");
    Wave* w = getWave_AssetCollection(ac,0);

    printf("valid %d",IsWaveValid(*w));
    printf("\n\n count : %u",ac->swaves->count);
    printf("\n\n %u %u %u",w->frameCount,w->sampleRate,w->sampleSize);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //if(IsKeyDown(KEY_S))
       
        BeginDrawing();
        ClearBackground(BLUE);

        EndDrawing();
    }
    unload_AssetCollection(ac);
   // UnloadModel(model);
    CloseWindow();        // Close window and OpenGL
    return 0;
}

