#include "oled_driver.h"
#include QMK_KEYBOARD_H
#include "gpio.h"
#include "keymap_german.h"
#include "oled.h"

static uint8_t anim_state = 0;
static uint32_t last_activity;

enum custom_keycodes {
    CU_ANIM = SAFE_RANGE,
    UD_CIRC = SAFE_RANGE + 1,
    UD_GRV  = SAFE_RANGE + 2,
    UD_ACUT = SAFE_RANGE + 3,
};

// Turn off the power LED
void keyboard_pre_init_user(void) {
    gpio_set_pin_output(24);
    gpio_write_pin_high(24);
}

#define CONTROL_LAYER(nx) LAYOUT(\
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
        PDF(nx), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     CU_ANIM, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU,\
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     RM_TOGG, RM_PREV, RM_HUED, RM_SATD, RM_VALD, RM_SPDD,\
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
                          XXXXXXX, XXXXXXX, _______, _______, _______,   _______, _______, _______, XXXXXXX, XXXXXXX\
)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_CAPS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_ESC,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                        KC_J,    KC_L,    KC_U,    KC_Y,    KC_DQT,  KC_LALT,
        KC_LSFT, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_RSFT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE,   KC_MPLY, KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_QUOT, KC_RCTL,
                          MO(2),   MO(1),   KC_SPC,  KC_TAB,  KC_LGUI,   KC_RGUI, KC_ENT,  KC_UNDS, MO(1),   MO(2)
    ),
    [1] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, KC_AT,   KC_PLUS, KC_SCLN, KC_COLN, KC_1,                        KC_0,    KC_QUES, KC_LBRC, KC_RBRC, KC_GRV,  _______,
        _______, KC_HASH, KC_CIRC, KC_DLR,  KC_ASTR, KC_SLSH,                     KC_BSPC, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, _______,
        _______, KC_LT,   KC_MINS, KC_EQL,  KC_GT,   KC_BSLS, _______,   _______, KC_PERC, KC_EXLM, KC_AMPR, KC_PIPE, KC_TILD, _______,
                          MO(3),   XXXXXXX, _______, _______, _______,   _______, _______, _______, XXXXXXX, MO(3)
    ),
    [2] = LAYOUT(
        KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,                      KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     KC_DEL,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          XXXXXXX, MO(3),   _______, _______, _______,   _______, _______, _______, MO(3),   XXXXXXX
    ),
    [3] = CONTROL_LAYER(4),
    [4] = LAYOUT(
        KC_CAPS, DE_1,    DE_2,    DE_3,    DE_4,    DE_5,                        DE_6,    DE_7,    DE_8,    DE_9,    DE_0,    KC_BSPC,
        KC_ESC,  DE_Q,    DE_W,    DE_F,    DE_P,    DE_G,                        DE_J,    DE_L,    DE_U,    DE_Y,    OSL(6),  KC_LALT,
        KC_LSFT, DE_A,    DE_R,    DE_S,    DE_T,    DE_D,                        DE_H,    DE_N,    DE_E,    DE_I,    DE_O,    KC_RSFT,
        KC_LCTL, DE_Z,    DE_X,    DE_C,    DE_V,    DE_B,    KC_MUTE,   KC_MPLY, DE_K,    DE_M,    DE_COMM, DE_DOT,  DE_QUOT, KC_RCTL,
                          MO(6),   MO(5),   KC_SPC,  KC_TAB,  KC_LGUI,   KC_RGUI, KC_ENT,  DE_UNDS, MO(5),   MO(6)
    ),
    [5] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, DE_AT,   DE_PLUS, DE_SCLN, DE_COLN, DE_1,                        DE_0,    DE_QUES, DE_LBRC, DE_RBRC, DE_DQUO, _______,
        _______, DE_HASH, UD_CIRC, DE_DLR,  DE_ASTR, DE_SLSH,                     KC_BSPC, DE_LCBR, DE_LPRN, DE_RPRN, DE_RCBR, _______,
        _______, DE_LABK, DE_MINS, DE_EQL,  DE_RABK, DE_BSLS, _______,   _______, DE_PERC, DE_EXLM, DE_AMPR, DE_PIPE, DE_TILD, _______,
                          MO(7),   XXXXXXX, _______, _______, _______,   _______, _______, _______, XXXXXXX, MO(7)
    ),
    [6] = LAYOUT(
        KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,                      KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
        _______, DE_CIRC, DE_ACUT, KC_UP,   DE_GRV,  XXXXXXX,                     XXXXXXX, UD_ACUT, DE_UDIA, UD_GRV,  QK_LLCK, _______,
        _______, DE_ADIA, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,                     KC_DEL,  XXXXXXX, XXXXXXX, XXXXXXX, DE_ODIA, _______,
        _______, DE_SS,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          XXXXXXX, MO(7),   _______, _______, _______,   _______, _______, _______, MO(7),   XXXXXXX
    ),
    [7] = CONTROL_LAYER(0),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        last_activity = timer_read32();
        switch (keycode) {
            case CU_ANIM:
                anim_state = (anim_state + 1) % 2;
                return false;
            // Undead keycodes for german layout
            // SEND_STRING assumes QWERTY, thus the nonsense below
            case UD_CIRC:
                SEND_STRING("``");
                return false;
            case UD_GRV:
                SEND_STRING("++");
                return false;
            case UD_ACUT:
                SEND_STRING("==");
                return false;
        }
    }
    return true;
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    if (is_keyboard_master()) {
        if (timer_elapsed32(last_activity) > OLED_TIMEOUT) {
            oled_off();
            return false;
        }
        render_master(anim_state);
    }
    else{
        render_slave(anim_state);
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    // 0 is left-half encoder,
    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    // 1 is right-half encoder
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }

    }
    return false;
}
#endif
