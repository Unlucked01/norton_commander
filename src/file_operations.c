#include "file_operations.h"

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

void delete_file(const char *filename) {
	clreol();
	if (remove(filename) == 0) {
		cprintf("File '%s' deleted successfully.", filename);
	} else {
		cprintf("Unable to delete the file '%s'.", filename);
	}
}

void move_file(const char *source, const char *destination) {
	clreol();
	if (rename(source, destination) == 0) {
		cprintf("File moved successfully.");
	} else {
		cprintf("Error moving file.");
	}
}
