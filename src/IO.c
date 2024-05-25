#include "conio.h"
#include "file_manager.h"
#include "file_operations.h"
#include "inttypes.h"

void handle_input(FileInfo *files, int *count, int *selection, bool *is_left_panel, char *current_path, char *opposite_path) {
	int ch = getch();
	switch (ch) {
		case 0:
		case 224:// Special keys (arrows, function keys)
			ch = getch();
			switch (ch) {
				case 72:// Up arrow
					if (*selection > 0) (*selection)--;
					break;
				case 80:// Down arrow
					if (*selection < *count - 1) (*selection)++;
					break;
				case 75:// Left arrow (switch to left panel)
					*is_left_panel = true;
					break;
				case 77:// Right arrow (switch to right panel)
					*is_left_panel = false;
					break;
			}
			break;
		case 9:// Tab key
			*is_left_panel = !*is_left_panel;
			break;
		case 13:// Enter key
			if (files[*selection].is_dir) {
				if (strcmp(files[*selection].name, "..") == 0) {
					// Move up one directory
					char *last_slash = strrchr(current_path, '\\');
					if (last_slash) {
						*last_slash = '\0';
					}
				} else if (strcmp(files[*selection].name, ".") != 0) {
					strcat(current_path, "\\");
					strcat(current_path, files[*selection].name);
				}
				refresh_directory(files, count, selection, current_path);
			}
			break;
		case 3:// CTRL+C
			copy_selected_file(files, *selection, current_path, opposite_path);
			getch();
			refresh_directory(files, count, selection, current_path);
			break;
		case 4:// CTRL+D
			if (!files[*selection].is_dir) {
				gotoxy(1, COMMANDS_ROW + 1);
				delete_file(files[*selection].path);
				clreol();
				getch();
				clrscr();
				refresh_directory(files, count, selection, current_path);
				display_commands();
			}
			break;
		case 6://CTRL+F
			move_selected_file(files, *selection, current_path, opposite_path);
			getch();
			refresh_directory(files, count, selection, current_path);
			break;
		case 27:// Escape key
			clrscr();
			exit(0);
	}
}