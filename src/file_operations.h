#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "file_manager.h"

int copy_file(const char *source, const char *destination);
void delete_file(const char *filename);
int read_directory(const char *path, FileInfo *files, int max_count);
void move_file(const char *source, const char *destination);

#endif
