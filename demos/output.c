#include <stdio.h>
#include <string.h>
#include "../src/termbox.h"

static const char chars[] = "nnnnnnnnnbbbbbbbbbuuuuuuuuuBBBBBBBBB";

static const tb_color all_attrs[] = {
	0,
	TB_BOLD,
	TB_UNDERLINE,
	TB_BOLD | TB_UNDERLINE,
};

static int next_char(int current) {
	current++;
	if (!chars[current])
		current = 0;

	return current;
}

static void draw_line(int x, int y, tb_color bg) {
	int a, c;
	int current_char = 0;
	for (a = 0; a < 4; a++) {
		for (c = TB_DEFAULT; c <= TB_WHITE; c++) {
			tb_color fg = all_attrs[a] | c;
			tb_char(x, y, fg, bg, chars[current_char]);
			current_char = next_char(current_char);
			x++;
		}
	}
}

static void print_combinations_table(int sx, int sy, const tb_color *attrs, int attrs_n) {
	int i, c;
	for (i = 0; i < attrs_n; i++) {
		for (c = TB_DEFAULT; c <= TB_WHITE; c++) {
			tb_color bg = attrs[i] | c;
			draw_line(sx, sy, bg);
			sy++;
		}
	}
}

static void draw_all() {
	tb_resize();
	tb_select_output_mode(TB_OUTPUT_NORMAL);

	static const tb_color col1[] = {0, TB_BOLD};
	static const tb_color col2[] = {TB_REVERSE};

	print_combinations_table(1, 1, col1, 2);
	print_combinations_table(2 + strlen(chars), 1, col2, 1);
	tb_render();

	int c, x, y = 24;
	tb_select_output_mode(TB_OUTPUT_256);

	for (c = 0, x = 0; c < 256; ++c, ++x) {
		if (!(x%24)) {
			x = 0;
			++y;
		}
		tb_char(x, y, c | ((y & 1) ? TB_UNDERLINE : 0), 0, '+');
		tb_char(x+25, y, 0, c, ' ');
	}

	tb_render();
}

int main(int argc, char **argv) {
	(void)argc; (void)argv;
	int ret = tb_init();
	if (ret) {
		fprintf(stderr, "tb_init() failed with error code %d\n", ret);
		return 1;
	}

	draw_all();

	struct tb_event ev;
	while (tb_poll_event(&ev)) {
		switch (ev.type) {
		case TB_EVENT_KEY:
			switch (ev.key) {
			case TB_KEY_ESC:
				goto done;
				break;
			}
			break;
		case TB_EVENT_RESIZE:
			draw_all();
			break;
		}
	}
done:
	tb_shutdown();
	return 0;
}
