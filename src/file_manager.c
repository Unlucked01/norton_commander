#include "file_manager.h"
#include "conio.h"
#include "file_operations.h"

int main() {
	FileInfo left_files[100];
	int left_count;
	int left_selection = 0;
	char current_path[FILE_NAME_LEN] = ".";
	// clrscr();
	// displaylogo();

	clrscr();
	display_commands();

	left_count = read_directory(current_path, left_files, 100);

	while (1) {
		display_panel(1, 1, left_files, left_count, left_selection, current_path);
		handle_input(left_files, &left_count, &left_selection, current_path);
	}

	getch();
	clrscr();

	return 0;
}
