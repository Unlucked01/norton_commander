#include "file_manager.h"
#include "conio.h"
#include "file_operations.h"
#include "stdbool.h"

int main() {
	FileInfo left_files[100], right_files[100];
	int left_count, right_count;
	int selection = 0;
	bool is_left_panel = true;
	char left_path[FILE_NAME_LEN] = ".";
	char right_path[FILE_NAME_LEN] = ".";

	displaylogo();
	clrscr();
	display_commands();

	left_count = read_directory(left_path, left_files, 100);
	right_count = read_directory(right_path, right_files, 100);

	while (1) {
		display_panel(LEFT_PANEL_X, LEFT_PANEL_Y, left_files, left_count, selection, is_left_panel, left_path);
		display_panel(RIGHT_PANEL_X, RIGHT_PANEL_Y, right_files, right_count, selection, !is_left_panel, right_path);

		if (is_left_panel) {
			handle_input(left_files, &left_count, &selection, &is_left_panel, left_path, right_path);
		} else {
			handle_input(right_files, &right_count, &selection, &is_left_panel, right_path, left_path);
		}
	}

	getch();
	clrscr();

	return 0;
}
