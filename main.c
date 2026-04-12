#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include "tc/tc.h"

void sigint_handler(int sig);
void sigwinch_handler(int sig);

void initial_setup(void);

void parse_input(void);
void redraw_screen(void);

const uint8_t menu_entries_size = 5;
// const char menu_entries[menu_entries_size][5] = {
// 	"ciao1",
// 	"ciao2",
// 	"ciao3",
// 	"ciao4",
// 	"ciao5"
// };

static uint16_t terminal_size_x;
static uint16_t terminal_size_y;

static uint8_t state = 0;
static bool need_refresh = true;

int main(void) {
	initial_setup();

	while(true) {
		if(need_refresh == true) {
			redraw_screen();
			need_refresh = false;
		}
		parse_input();
	}
	
	tc_restore_defaults();
	return 0;
}


void initial_setup(void) {
	signal(SIGINT, sigint_handler);
	signal(SIGWINCH, sigwinch_handler);

	tc_init();
	tc_set_echo_mode(false);
	tc_set_canonical_mode(false);
	tc_hide_cursor(true);

	tc_get_terminal_size(&terminal_size_y, &terminal_size_x);

	tc_erase_to_origin();
	fflush(stdout);
}
void parse_input(void) {
	static tc_keyboard_input_d buff;
	tc_get_pressed_keys(&buff);
	bool found = false;
	size_t i;
	for(i = 0;i < buff.escape_input_size && found == false;i++) {
		switch(buff.escape_input[i]) {
			case TC_ARROW_UP:
			case TC_ARROW_DOWN: {
				found = true;
			} break;
			default: {
			}
		}
	}
	if(found == true) {
		if(buff.escape_input[i - 1] == TC_ARROW_UP) {
			if (state <= 0) {
				state = menu_entries_size;
			}
			state--;
		} else {
			state++;
			if(state >= menu_entries_size) {
				state = 0;
			}
		}
		need_refresh = true;
	}
}
void redraw_screen(void) {
	tc_erase_to_origin();
	for(uint16_t i = 0;i < menu_entries_size;i++) {
		if(i == state) {
			tc_set_color_standard(BLACK);
			tc_set_bg_color_standard(BLUE);
		} else {
			tc_set_color_default();
		}
		printf("scemo in culo %d                         \n", i);
	}
	fflush(stdout);
	tc_set_color_default();
}

//callbacks
void sigint_handler(int sig) {
	tc_reset_font();
	tc_erase_to_origin();
	tc_hide_cursor(false);
	printf("termined with SIGINT\n");
	tc_restore_defaults();
	exit(0);
}

void sigwinch_handler(int sig) {
	uint16_t row, col;
	tc_get_terminal_size(&row, &col);
	terminal_size_x = col;
	terminal_size_y = row;
}
