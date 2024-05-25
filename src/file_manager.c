#include "file_manager.h"
#include "conio.h"
#include "file_operations.h"
#include "stdbool.h"

int main() {
	FileInfo left_files[100], right_files[100];
	int left_count, right_count;
	int left_selection = 0;
	int right_selection = 0;
	bool is_left_panel = true;
	char left_path[FILE_NAME_LEN] = ".";
	char right_path[FILE_NAME_LEN] = ".";

	clrscr();
	display_commands();

	left_count = read_directory(left_path, left_files, 100);
	right_count = read_directory(right_path, right_files, 100);

	while (1) {
		display_panel(1, 1, left_files, left_count, left_selection, is_left_panel, left_path);
		display_panel(40, 1, right_files, right_count, right_selection, !is_left_panel, right_path);

		if (is_left_panel) {
			handle_input(left_files, &left_count, &left_selection, &is_left_panel, left_path);
		} else {
			handle_input(right_files, &right_count, &right_selection, &is_left_panel, right_path);
		}
	}

	getch();
	clrscr();

	return 0;
}
