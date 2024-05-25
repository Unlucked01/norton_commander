#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "file_manager.h"

void copy_selected_file(FileInfo *files, int selection, char *current_path, char *opposite_path);
void delete_file(const char *filename);
int read_directory(const char *path, FileInfo *files, int max_count);
void move_selected_file(FileInfo *files, int selection, char *current_path, char *opposite_path);
void refresh_directory(FileInfo *files, int *count, int *selection, const char *path);


#endif
