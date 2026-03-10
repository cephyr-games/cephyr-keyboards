#include QMK_KEYBOARD_H
#include "oled.h"

#include "admech.h"
#include "mountain.h"
#include "berry.h"
#include "modifier.h"
#include "layer.h"
#ifdef BAD_APPLE
#include "bad_apple.h"
#endif
#define FRAME_TIME 180

extern void render_logo_text(void);
extern void render_space(void);

void render_cog(void) {
    oled_write_raw_P(admech_logo, sizeof(admech_logo));
}

static void render_mountain(void) {
    static uint32_t anim_timer = 0;
    static uint8_t current_frame = 0;
    if (timer_elapsed32(anim_timer) > FRAME_TIME) {
        anim_timer = timer_read32();
        current_frame = (current_frame + 1) % 8;
    } else {
        return;
    }
    oled_write_raw_P(mountain[current_frame], sizeof(mountain[current_frame]));
}

void render_berry(void) {
    static uint32_t anim_timer = 0;
    static uint8_t current_frame = 0;
    if (timer_elapsed32(anim_timer) > FRAME_TIME) {
        anim_timer = timer_read32();
        current_frame = (current_frame + 1) % 8;
    } else {
        return;
    }
    oled_write_raw_P(berry[current_frame], sizeof(berry[current_frame]));
}

#ifdef BAD_APPLE
void render_bad_apple(void) {
    static uint32_t anim_timer = 0;
    static uint32_t current_frame = 0;
    render_space();
    render_space();
    if (timer_elapsed32(anim_timer) > 33) {
        anim_timer = timer_read32();
        current_frame = (current_frame + 1) % 6572;
    } else {
        return;
    }
    oled_write_raw_P(bad_apple[current_frame], sizeof(bad_apple[current_frame]));
}
#endif

#define MOD_BUFFER_SIZE 64
static char mod_buffer[MOD_BUFFER_SIZE];

void render_mod_status_gui_alt_user(uint8_t modifiers) {
    memset(mod_buffer, 0, MOD_BUFFER_SIZE);
    // Modifier
    if (modifiers & MOD_MASK_SHIFT) {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= shift[i];
        }
    }
    if (modifiers & MOD_MASK_GUI) {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= meta[i];
        }
    }
    if (modifiers & MOD_MASK_CTRL) {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= ctrl[i];
        }
    }
    if (modifiers & MOD_MASK_ALT) {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= alt[i];
        }
    }
    // Capslock
    led_t led_usb_state = host_keyboard_led_state();
    if (led_usb_state.caps_lock) {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= caps[i];
        }
    }
    // Language
    if (get_highest_layer(layer_state | default_layer_state) > 7) {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= qwertz[i];
        }
    } else if (get_highest_layer(layer_state | default_layer_state) > 3) {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= lan_ge[i];
        }
    } else {
        for (int i = 0; i < MOD_BUFFER_SIZE; i++) {
            mod_buffer[i] |= lan_us[i];
        }
    }
    // Write
    oled_write_raw_P(mod_buffer, sizeof(mod_buffer));
}

void render_prayer(void) {
    oled_write_raw_P(prayer, sizeof(prayer));
}

void render_layer_state_user(void) {
    switch (get_highest_layer(layer_state)) {
        case 1:
            oled_write_raw_P(lower_layer, sizeof(lower_layer));
            break;
        case 2:
            oled_write_raw_P(raise_layer, sizeof(raise_layer));
            break;
        case 3:
            oled_write_raw_P(adjust_layer, sizeof(adjust_layer));
            break;
        default:
            oled_write_raw_P(default_layer, sizeof(default_layer));
    }
}

void render_master(uint8_t animation) {
    render_cog();
    oled_set_cursor(0, 4);
    render_logo_text();
    oled_set_cursor(0, 5);
    render_layer_state_user();
    oled_set_cursor(0, 9);
    render_mod_status_gui_alt_user(get_mods()|get_oneshot_mods());
    oled_set_cursor(0, 11);
    #ifdef BAD_APPLE
    if (animation == 0) {
        render_mountain();
    } else if (animation == 1) {
        render_bad_apple();
    }
    #else
    render_mountain();
    #endif
}

void render_slave(uint8_t animation) {
    render_cog();
    oled_set_cursor(0, 4);
    render_logo_text();
    oled_set_cursor(0, 6);
    render_prayer();
    oled_set_cursor(0, 10);
    render_berry();
}
