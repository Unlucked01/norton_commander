#include "conio.h"
#include "file_manager.h"
#include "logos.h"
#include "stdbool.h"
#include "stdio.h"
#include <sys/nearptr.h>

#define VIDEO_INT 0x10          /* the BIOS video interrupt. */
#define SET_MODE 0x00           /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE 0x13 /* use to set 256-color mode. */
#define TEXT_MODE 0x03          /* use to set 80x25 text mode. */

#define SCREEN_WIDTH 320  /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT 200 /* height in pixels of mode 0x13 */
#define NUM_COLORS 256    /* number of colors in mode 0x13 */

typedef unsigned char byte;

byte *VGA = (byte *) 0xA0000; /* this points to video memory. */

#define sgn(x) ((x < 0) ? -1 : ((x > 0) ? 1 : 0)) /* macro to return the sign of a
                                         number */

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

void set_mode(byte mode) {
	union REGS regs;

	regs.h.ah = SET_MODE;
	regs.h.al = mode;
	int86(VIDEO_INT, &regs, &regs);
}

void plot_pixel(int x, int y, byte color) {
	/*  y*320 = y*256 + y*64 = y*2^8 + y*2^6   */
	VGA[(y << 8) + (y << 6) + x] = color;
}

void line_slow(int x1, int y1, int x2, int y2, byte color) {
	int dx, dy, sdx, sdy, px, py, dxabs, dyabs, i;
	float slope;

	dx = x2 - x1; /* the horizontal distance of the line */
	dy = y2 - y1; /* the vertical distance of the line */
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	if (dxabs >= dyabs) /* the line is more horizontal than vertical */
	{
		slope = (float) dy / (float) dx;
		for (i = 0; i != dx; i += sdx) {
			px = i + x1;
			py = slope * i + y1;
			plot_pixel(px, py, color);
		}
	} else /* the line is more vertical than horizontal */
	{
		slope = (float) dx / (float) dy;
		for (i = 0; i != dy; i += sdy) {
			px = slope * i + x1;
			py = i + y1;
			plot_pixel(px, py, color);
		}
	}
}

void displaylogo() {
	__djgpp_nearptr_enable();
	set_mode(VGA_256_COLOR_MODE); /* set the video mode. */
	VGA += __djgpp_conventional_base;
	int x1, y1, x2, y2, color;
	color = 1;
	//Draw left line
	y1 = 0;
	y2 = SCREEN_HEIGHT;
	for (x1 = 0, x2 = 0; x1 < 3 && x2 < 3; x1++, x2++) {
		line_slow(x1, y1, x2, y2, color);
	}
	//Draw up line
	x1 = 0;
	x2 = SCREEN_WIDTH;
	for (y1 = 0, y2 = 0; y1 < 3 && y2 < 3; y1++, y2++) {
		line_slow(x1, y1, x2, y2, color);
	}
	//Draw right line
	y1 = 0;
	y2 = SCREEN_HEIGHT;
	for (x1 = SCREEN_WIDTH - 3, x2 = SCREEN_WIDTH - 3; x1 < SCREEN_WIDTH && x2 < SCREEN_WIDTH; x1++, x2++) {
		line_slow(x1, y1, x2, y2, color);
	}
	//Draw down line
	x1 = 0;
	x2 = SCREEN_WIDTH;
	for (y1 = SCREEN_HEIGHT - 3, y2 = SCREEN_HEIGHT - 3; y1 < SCREEN_HEIGHT && y2 < SCREEN_HEIGHT; y1++, y2++) {
		line_slow(x1, y1, x2, y2, color);
	}
	set_mode(TEXT_MODE);
	// printf("Press any key to continue...");
	getch();
}