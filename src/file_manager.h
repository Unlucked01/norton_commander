#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdbool.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <errno.h>


#define PANEL_WIDTH 40
#define PANEL_HEIGHT 23
#define FILE_NAME_LEN 256
#define COMMANDS_ROW (PANEL_HEIGHT + 1)

#define TEXT_MODE 0x03

typedef struct {
	char name[FILE_NAME_LEN];
	char path[FILE_NAME_LEN];
	unsigned long size;
	bool is_dir;
} FileInfo;

void draw_border(int left, int top, int width, int height, const char *path);
void display_panel(int left, int top, FileInfo *files, int count, int current_selection, bool is_left_panel, const char *current_path);
void display_commands();
void handle_input(FileInfo *files, int *count, int *selection, bool *is_left_panel, char *current_path);
void displaylogo();

#endif
