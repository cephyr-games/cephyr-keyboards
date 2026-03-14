#include "keycodes.h"
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_CAPS, DE_1,    DE_2,    DE_3,    DE_4,    DE_5,                              DE_6,          DE_7,           DE_8,    DE_9,    DE_0,    KC_BSPC,
        KC_ESC,  DE_Q,    DE_W,    DE_F,    DE_P,    DE_G,                              DE_J,          DE_L,           DE_U,    DE_Y,    DE_DQUO, KC_LALT,
        KC_LSFT, DE_A,    DE_R,    DE_S,    DE_T,    DE_D,                              DE_H,          DE_N,           DE_E,    DE_I,    DE_O,    KC_RSFT,
        KC_LCTL, DE_Z,    DE_X,    DE_C,    DE_V,    DE_B,          KC_MUTE,   KC_MPLY, DE_K,          DE_M,           DE_COMM, DE_DOT,  DE_QUOT, KC_RCTL,
                          MO(3),   KC_DEL,  KC_SPC,  LT(2, KC_TAB), KC_LGUI,   KC_RGUI, LT(2, KC_ENT), LT(1, DE_UNDS), KC_BSPC, KC_RALT
    ),
    [1] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, DE_AT,   DE_EXLM, DE_AMPR, DE_PIPE, DE_TILD,                     DE_1,    DE_QUES, DE_LBRC, DE_RBRC, DE_0,    _______,
        _______, DE_COLN, DE_PLUS, DE_MINS, DE_ASTR, DE_SLSH,                     DE_SCLN, DE_LCBR, DE_LPRN, DE_RPRN, DE_RCBR, _______,
        _______, DE_PERC, DE_LABK, DE_RABK, DE_EQL,  DE_BSLS, _______,   _______, UD_ACUT, DE_HASH, UD_CIRC, DE_DLR,  UD_GRV,  _______,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    [2] = LAYOUT(
        KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,                      KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
        _______, XXXXXXX, KC_PGDN, KC_UP,   KC_PGUP, KC_HOME,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_LLCK, _______,
        _______, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, KC_FIND,                     KC_DEL,  DE_SS,   DE_ADIA, DE_UDIA, DE_ODIA, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, DE_ACUT, XXXXXXX, DE_GRV,  XXXXXXX, DE_GRV,  _______,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    [3] = LAYOUT(
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     CU_ANIM, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU,\
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     RM_TOGG, RM_PREV, RM_HUED, RM_SATD, RM_VALD, RM_SPDD,\
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,\
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        last_activity = timer_read32();
        switch (keycode) {
            // Workaround limitation of mod tap
            case LT(1, DE_UNDS):
                if (record->tap.count) {
                    tap_code16(DE_UNDS);
                    return false;
                }
                break;
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
            if (is_oled_on())
                oled_off();
            return false;
        }
        oled_on();
        render_master(anim_state);
    }
    else {
        if (is_oled_on())
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
