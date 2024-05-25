#include "file_manager.h"


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

void display_panel(int left, int top, FileInfo *files, int count, int current_selection, bool is_panel_active, const char *current_path) {
	draw_border(left, top, PANEL_WIDTH, PANEL_HEIGHT, current_path);
	for (int i = 0; i < count && i < PANEL_HEIGHT - 2; i++) {
		gotoxy(left + 1, top + 1 + i);
		if (is_panel_active && i == current_selection) {
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

void displaylogo() {
	const char *logo[] = {
			"                                                              | |     \n",
			"             ___ ___  _   _ _ __ ___  ___  __       _____ _ __| | __  \n",
			"            / __/ _ \\| | | | '__/ __|/ _ \\ \\ \\ /\\ / / _ \\| '__| |/ /  \n",
			"           | (_| (_) | |_| | |  \\__ \\  __/  \\ V  V / (_) | |  |   <   \n",
			"            \\___\\___/ \\__,_|_|  |___/\\___|   \\_/\\_/ \\___/|_|  |_|\\_\\ \n",
			"                                                                     ",
			"                                                                     "};
	printf("\n\n");
	const char *file_manager[] = {
			"       (()/(  )\\ ((_) ))\\     (     ( /(   (     ( /(  )\\))(   ))\\  )(   \n",
			"        /(_))((_) _  /((_)    )\\  ' )(_))  )\\ )  )(_))((_))\\  /((_)(()\\  \n",
			"       (_) _| (_)| |(_))    _((_)) ((_)_  _(_/( ((_)_  (()(_)(_))   ((_) \n",
			"        |  _| | || |/ -_)  | '  \\()/ _` || ' \\))/ _` |/ _` | / -_) | '_| \n",
			"        |_|   |_||_|\\___|  |_|_|_| \\__,_||_||_| \\__,_|\\__, | \\___| |_|   \n",
			"                                                      |___/              \n"};
	const char *by_surname[] = {
			"     ******   **    **       **     **  ********   *******   **      **\n",
			"    /*////** //**  **       /**    /** **//////   **/////** /**     /**\n",
			"    /*   /**  //****        /**    /**/**        **     //**/**     /**\n",
			"    /******    //**         /**    /**/*********/**      /**//**    ** \n",
			"    /*//// **   /**         /**    /**////////**/**      /** //**  **  \n",
			"    /*    /**   /**         /**    /**       /**//**     **   //****   \n",
			"    /*******    /**         //*******  ********  //*******     //**    \n",
			"    ///////     //           ///////  ////////    ///////       //     \n"};

	for (int row = 0; row < 7; row++) {
		const char *line = logo[row];
		printf("%s", line);
	}
	for (int row = 0; row < 6; row++) {
		const char *line = file_manager[row];
		printf("%s", line);
	}
	for (int row = 0; row < 8; row++) {
		const char *line = by_surname[row];
		printf("%s", line);
	}
	printf("\nPress any key to continue...");
	getch();
}