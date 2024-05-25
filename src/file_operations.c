#include "file_operations.h"
#include "conio.h"
#include "file_manager.h"

int copy_file(const char *source, const char *destination) {
	FILE *src, *dest;
	char buffer[512];
	size_t bytes;

	if ((src = fopen(source, "rb")) == NULL) {
		gotoxy(1, COMMANDS_ROW);
		cprintf("Error opening source file: %s", strerror(errno));
		return 1;
	}
	if ((dest = fopen(destination, "wb")) == NULL) {
		gotoxy(1, COMMANDS_ROW);
		cprintf("Error opening destination file: %s", strerror(errno));
		fclose(src);
		return 2;
	}

	while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
		fwrite(buffer, 1, bytes, dest);
	}

	fclose(src);
	fclose(dest);

	return 0;
}

void copy_selected_file(FileInfo *files, int selection, char *current_path, char *opposite_path) {
	if (!files[selection].is_dir) {
		if (strcmp(current_path, opposite_path) == 0) {
			gotoxy(1, COMMANDS_ROW + 1);
			clreol();
			cprintf("Cannot copy to the same directory.");
			return;
		}

		char destination[FILE_NAME_LEN];
		snprintf(destination, FILE_NAME_LEN, "%s\\%s", opposite_path, files[selection].name);
		int result = copy_file(files[selection].path, destination);

		gotoxy(1, COMMANDS_ROW + 1);
		clreol();
		if (result == 0) {
			cprintf("File copied successfully.");
		} else {
			cprintf("Error copying file.");
		}
	}
}

int read_directory(const char *path, FileInfo *files, int max_count) {
	struct ffblk ffblk;
	int done, count = 0;

	char search_path[FILE_NAME_LEN];
	snprintf(search_path, FILE_NAME_LEN, "%s\\*.*", path);

	done = findfirst(search_path, &ffblk, FA_DIREC | FA_ARCH);
	while (!done && count < max_count) {
		strncpy(files[count].name, ffblk.ff_name, FILE_NAME_LEN - 1);
		files[count].name[FILE_NAME_LEN - 1] = '\0';
		snprintf(files[count].path, FILE_NAME_LEN, "%s\\%s", path, ffblk.ff_name);
		files[count].is_dir = (ffblk.ff_attrib & FA_DIREC) ? true : false;
		files[count].size = ffblk.ff_fsize;
		count++;
		done = findnext(&ffblk);
	}
	return count;
}

void refresh_directory(FileInfo *files, int *count, int *selection, const char *path) {
	clrscr();// Clear the screen
	*count = read_directory(path, files, 100);
	*selection = 0;
	display_commands();
}


void delete_file(const char *filename) {
	clreol();
	if (remove(filename) == 0) {
		cprintf("File '%s' deleted successfully.", filename);
	} else {
		cprintf("Unable to delete the file '%s'.", filename);
	}
}

void move_selected_file(FileInfo *files, int selection, char *current_path, char *opposite_path) {
	if (!files[selection].is_dir) {
		if (strcmp(current_path, opposite_path) == 0) {
			gotoxy(1, COMMANDS_ROW + 1);
			clreol();
			cprintf("Cannot move to the same directory.");
			return;
		}

		char destination[FILE_NAME_LEN];
		snprintf(destination, FILE_NAME_LEN, "%s\\%s", opposite_path, files[selection].name);
		clreol();
		if (rename(files[selection].path, destination) == 0) {
			gotoxy(1, COMMANDS_ROW + 1);
			cprintf("File moved successfully.");
		} else {
			cprintf("Error moving file: %s", strerror(errno));
		}
	}
}
