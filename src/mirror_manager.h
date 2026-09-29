#ifndef MIRROR_MANAGER_H
#define MIRROR_MANAGER_H

#include <glib.h>

typedef struct {
    const char *name;
    const char *url;
    const char *region;
    const char *location;
    int tier;
} MirrorInfo;

int neko_mirrors_count(void);
MirrorInfo *neko_mirrors_list(void);

#endif
