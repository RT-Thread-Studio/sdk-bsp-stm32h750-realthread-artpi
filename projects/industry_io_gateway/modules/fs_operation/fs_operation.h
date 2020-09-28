#ifndef __FS_OPERATION_H
#define __FS_OPERATION_H
#include "common.h"

int fs_init(struct global_sundry *g_sundry);
char *fs_read_file(const char *path);
int fs_create_dir(const char *path);
char* fs_normalize_path(char* fullpath);

#endif
