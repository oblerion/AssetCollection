#pragma once
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// wav mp3 asset collection
struct WaveCollection 
{
    Wave *waves;
    unsigned int count;
};
struct WaveCollection* init_WaveCollection();
struct WaveCollection* add_WaveCollection(struct WaveCollection* pwc, const char* file);
bool save_WaveCollection(FILE* fic, const struct WaveCollection *collection);
struct WaveCollection* load_WaveCollection(FILE* fic,unsigned int count);
void unload_WaveCollection(struct WaveCollection *collection);

struct ImageCollection
{
    Image *images;
    unsigned int count;
};
struct ImageCollection* init_ImageCollection();
struct ImageCollection* add_ImageCollection(struct ImageCollection* pic, const char* file);
bool save_ImageCollection(FILE* fic, const struct ImageCollection* pic);
struct ImageCollection* load_ImageCollection(FILE* fic,unsigned int count);
void unload_ImageCollection(struct ImageCollection* pic);

struct AssetCollection 
{
    struct WaveCollection* swaves;
};
struct AssetCollection* init_AssetCollection();
struct AssetCollection* add_AssetCollection(struct AssetCollection* ac,const char* path);
Wave* getWave_AssetCollection(struct AssetCollection* ac, unsigned int id);
bool save_AssetCollection(const char* path,struct AssetCollection* ac);
struct AssetCollection* load_AssetCollection(const char* path);
void unload_AssetCollection(struct AssetCollection* ac);

// Structure pour regrouper plusieurs Wave


