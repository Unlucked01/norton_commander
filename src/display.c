#include "file_manager.h"
#include "logos.h"
#include "stdio.h"

void draw_border(int left, int top, int width, int height, const char *path) {
	int right = left + width - 1;
	int bottom = top + height - 1;
	int path_len = strlen(path);
	int path_start = left + (width - path_len) / 2;

	for (int x = left; x <= right; x++) {
		if (x >= path_start && x < path_start + path_len) {
			gotoxy(x, top);
			putch(path[x - path_start]);
		} else {
			gotoxy(x, top);
			putch('-');
		}
		gotoxy(x, bottom);
		putch('-');
	}
	for (int y = top; y <= bottom; y++) {
		gotoxy(left, y);
		putch('|');
		gotoxy(right, y);
		putch('|');
	}

	gotoxy(left, top);
	putch('+');
	gotoxy(right, top);
	putch('+');
	gotoxy(left, bottom);
	putch('+');
	gotoxy(right, bottom);
	putch('+');
}

void display_panel(int left, int top, FileInfo *files, int count, int current_selection, const char *current_path) {
	draw_border(left, top, PANEL_WIDTH, PANEL_HEIGHT, current_path);
	for (int i = 0; i < count && i < PANEL_HEIGHT - 2; i++) {
		gotoxy(left + 1, top + 1 + i);
		if (i == current_selection) {
			textbackground(BLUE);
			textcolor(WHITE);
		} else {
			textbackground(BLACK);
			textcolor(LIGHTGRAY);
		}
		if (files[i].is_dir) {
			cprintf("[DIR] %s", files[i].name);
		} else {
			cprintf("%-8s %lu", files[i].name, files[i].size);
		}
		textbackground(BLACK);
		textcolor(LIGHTGRAY);
	}
}

void display_commands() {
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	gotoxy(1, COMMANDS_ROW);
	clreol();
	cprintf("Enter - Open, Ctrl+C - Copy, Ctrl+D - Delete, Ctrl+F - Move, Esc - Exit");
}

// void displaylogo() {
// 	printf(
// 			"*     ____    ______   __       ____                 ______  __  __  ______  ____    ____    ____         *"
// 			"*    /\  _`\ /\__  _\ /\ \     /\  _`\       /'\_/`\/\  _  \/\ \/\ \/\  _  \/\  _`\ /\  _`\ /\  _`\       *"
// 			"*    \ \ \L\_\/_/\ \/ \ \ \    \ \ \L\_\    /\      \ \ \L\ \ \ `\\ \ \ \L\ \ \ \L\_\ \ \L\_\ \ \L\ \     *"
// 			"*     \ \  _\/  \ \ \  \ \ \  __\ \  _\L    \ \ \__\ \ \  __ \ \ , ` \ \  __ \ \ \L_L\ \  _\L\ \ ,  /     *"
// 			"*      \ \ \/    \_\ \__\ \ \L\ \\ \ \L\ \   \ \ \_/\ \ \ \/\ \ \ \`\ \ \ \/\ \ \ \/, \ \ \L\ \ \ \\ \    *"
// 			"*       \ \_\    /\_____\\ \____/ \ \____/    \ \_\\ \_\ \_\ \_\ \_\ \_\ \_\ \_\ \____/\ \____/\ \_\ \_\  *"
// 			"*        \/_/    \/_____/ \/___/   \/___/      \/_/ \/_/\/_/\/_/\/_/\/_/\/_/\/_/\/___/  \/___/  \/_/\/ /  *"
// 			"*                                                                                                         *");
// 	printf("Press any key to continue...");
// 	getch();
// }