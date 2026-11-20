#include "AssetCollection.h"
#include <stdio.h>
// ---
// initialisation de la collection de musique
// ---
struct WaveCollection* init_WaveCollection()
{
    struct WaveCollection* wc = NULL;
    wc = (struct WaveCollection*)RL_MALLOC(sizeof(struct WaveCollection));
    /* allocation failure */
    if(wc!=NULL)
    {
        wc->waves = NULL;
        wc->count = 0;
    }
    return wc;
}
// ---
// ajout d'une musique dans la collection de musique
// ---
struct WaveCollection* add_WaveCollection(struct WaveCollection* pwc, const char* file)
{
    if (pwc == NULL)
    {
        pwc = init_WaveCollection();
        if (pwc == NULL)
            return NULL;
    }

    Wave* new_waves = (Wave*)RL_REALLOC(pwc->waves,
                                        (pwc->count + 1) * sizeof(Wave));
    if (new_waves == NULL)
        return pwc;                          /* or handle failure as preferred */
    
    pwc->waves = new_waves;
    pwc->waves[pwc->count] = LoadWave(file);
    pwc->count++;

    return pwc;
}

struct header_wave
{
    unsigned int frameCount;
    unsigned int sampleRate;
    unsigned int sampleSize;
    unsigned int channels;
    unsigned int dataSize;
};
// ---
// Écriture d’une collection de Wave dans un fichier binaire
// ---
bool save_WaveCollection(FILE* fic, const struct WaveCollection *collection)
{
    if (fic == NULL || collection == NULL || collection->count == 0)
        return false;

    // 2. Écrire chaque Wave
    for (unsigned int i = 0; i < collection->count; i++) {
        const Wave *w = &collection->waves[i];

        struct header_wave hw = {
            (unsigned int)w->frameCount,
            (unsigned int)w->sampleRate,
            (unsigned int)w->sampleSize,
            (unsigned int)w->channels,
            (unsigned int)(w->frameCount * w->channels * (w->sampleSize / 8))
        };

        if(fwrite(&hw, sizeof(struct header_wave),1,fic) !=1)
        {
            return false;
        }

        // Données brutes
        if (hw.dataSize > 0 && w->data != NULL) {
            if (fwrite(w->data, 1, hw.dataSize, fic) != hw.dataSize) {
                return false;
            }
        }
    }

   // fclose(file);
    return true;
}

// ---
// Lecture d’une collection de Wave depuis un fichier binaire
// ---
struct WaveCollection* load_WaveCollection(FILE* fic,unsigned int count)
{
    if (fic == NULL)
        return NULL;

    // Allouer le tableau de Wave
    struct WaveCollection *collection = init_WaveCollection();
    collection->waves = (Wave *)RL_CALLOC(count, sizeof(Wave));
    if (collection->waves == NULL) {
        return NULL;
    }

    // 2. Lire chaque Wave
    for (unsigned int i = 0; i < count; i++) {
        struct header_wave hw = {0};
        //unsigned int frameCount = 0, sampleRate = 0, sampleSize = 0, channels = 0, dataSize = 0;


        if (fread(&hw, sizeof(struct header_wave), 1, fic)!=1)
        {
            // Libérer ce qui a déjà été alloué
            for (unsigned int j = 0; j < i; j++) {
                if (collection->waves[j].data) RL_FREE(collection->waves[j].data);
            }
            RL_FREE(collection->waves);
            collection->waves = NULL;
            //fclose(fic);
            return NULL;
        }

        collection->waves[i].frameCount = hw.frameCount;
        collection->waves[i].sampleRate = hw.sampleRate;
        collection->waves[i].sampleSize = hw.sampleSize;
        collection->waves[i].channels   = hw.channels;
        collection->waves[i].data       = NULL;

        if (hw.dataSize > 0) {
            collection->waves[i].data = RL_MALLOC(hw.dataSize);
            if (collection->waves[i].data == NULL ||
                fread(collection->waves[i].data, 1, hw.dataSize, fic) != hw.dataSize) {
                // Nettoyage en cas d’échec
                for (unsigned int j = 0; j <= i; j++) {
                    if (collection->waves[j].data) RL_FREE(collection->waves[j].data);
                }
                RL_FREE(collection->waves);
                collection->waves = NULL;
                //fclose(fic);
                return NULL;
            }
        }
    }

    //collection->waves = waves;
    collection->count = count;
    return collection;
}

// ---
// Libération de la mémoire d’une collection
// ---
void unload_WaveCollection(struct WaveCollection *collection)
{
    if (collection == NULL) return;

    if (collection->waves != NULL) {
        for (unsigned int i = 0; i < collection->count; i++) {
            if (collection->waves[i].data != NULL) {
                RL_FREE(collection->waves[i].data);
            }
        }
        RL_FREE(collection->waves);
        RL_FREE(collection);
        collection = NULL;
    }
}

struct ImageCollection* init_ImageCollection()
{
    struct ImageCollection* ic = (struct ImageCollection*)RL_MALLOC(sizeof(struct ImageCollection))
    if(ic != NULL)
    {
        ic->images = NULL;
        ic->count = 0;
    }
    return ic;
}

struct ImageCollection* add_ImageCollection(struct ImageCollection* pic, const char* file)
{
    if(pic != NULL)
    {
        pic = RL_REALLOC(pic,sizeof(struct ImageCollection)*pic->count);
        if(pic == NULL) {}
        pic->images[pic->count] = LoadImage(file);
        pic->count++;
        return pic;
    }
    return NULL;
}


void unload_ImageCollection(struct ImageCollection* pic)
{
    if(pic != NULL)
    {
        for(unsigned int i=pic->count;i>0;i--)
        {
            UnloadImage(pic->images[i]);
        }
        if(pic->images != NULL) RL_FREE(pic->images);
        RL_FREE(pic);
        pic = NULL;
    }
    return;
}


struct header_asset
{
    unsigned int nb_wav;
};

struct AssetCollection *init_AssetCollection()
{
    struct AssetCollection* ac = (struct AssetCollection*)RL_MALLOC(sizeof(struct AssetCollection));
    if(ac!=NULL)
    {
        ac->swaves = init_WaveCollection();
    }
    return ac;
}

struct AssetCollection *add_AssetCollection(struct AssetCollection *ac, const char *path)
{
    const char* ext = GetFileExtension(path);
    if(ac==NULL)
    {
        ac = init_AssetCollection();
    }
 
    if(TextIsEqual(ext,".wav") || TextIsEqual(ext,".mp3"))
    {
        ac->swaves = add_WaveCollection(ac->swaves,path);
        printf("\n[AssetCollection] : load music %s",path);
        return ac;
    }
    
    return NULL;
}

Wave* getWave_AssetCollection(struct AssetCollection* ac, unsigned int id)
{
    Wave* w = (Wave*)RL_MALLOC(sizeof(Wave));
    if(ac != NULL)
    {
        if(ac->swaves->count > id)
        *w = ac->swaves->waves[id];
    }
    return w;
}

bool save_AssetCollection(const char* path,struct AssetCollection* ac)
{
    FILE* fic = fopen(path,"wb");
    if(fic != NULL && ac != NULL)
    {
        struct header_asset ha = {
            ac->swaves->count
        };

        if(fwrite(&ha,sizeof(struct header_asset),1,fic) != 1)
        {
            fclose(fic);
            return false;
        }
        save_WaveCollection(fic,ac->swaves);
        fclose(fic);
        return true;
    }
    return false;

}

struct AssetCollection* load_AssetCollection(const char* path)
{
    if(IsPathFile(path))
    {
        FILE* fic = fopen(path,"rb");
        if(fic != NULL)
        {
            struct AssetCollection* ac = init_AssetCollection();
            struct header_asset ha = {0};

            if(fread(&ha,sizeof(struct header_asset),1,fic) != 1)
            {
                fclose(fic);
                return NULL;
            }

            ac->swaves = load_WaveCollection(fic,ha.nb_wav);
            fclose(fic);
            return ac;

        }
    }
    return NULL;
}

void unload_AssetCollection(struct AssetCollection* ac)
{
    if(ac != NULL)
    {
        unload_WaveCollection(ac->swaves);
        RL_FREE(ac);
        ac = NULL;
    }
    return;
}