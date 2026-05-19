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
    SD_QUOT = SAFE_RANGE + 4,
    SU_COMM = SAFE_RANGE + 5,
    SU_DOT = SAFE_RANGE + 6,
};

// Turn off the power LED
void keyboard_pre_init_user(void) {
    gpio_set_pin_output(24);
    gpio_write_pin_high(24);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // German QWERTZ
    [0] = LAYOUT(
        KC_LALT, DE_1,    DE_2,    DE_3,    DE_4,    DE_5,                                 DE_6,    DE_7,    DE_8,    DE_9,    DE_0,    KC_RALT,
        KC_ESC,  DE_Q,    DE_W,    DE_F,    DE_P,    DE_G,                                 DE_J,    DE_L,    DE_U,    DE_Y,    DE_MINS, MO(2),
        KC_LSFT, DE_A,    DE_R,    DE_S,    DE_T,    DE_D,                                 DE_H,    DE_N,    DE_E,    DE_I,    DE_O,    KC_RSFT,
        KC_LCTL, DE_Z,    DE_X,    DE_C,    DE_V,    DE_B,    KC_MUTE,   KC_MPLY, DE_K,    DE_M,    DE_COMM, DE_DOT,  SD_QUOT, KC_RCTL,
                          MO(3),   KC_DEL,  KC_SPC,  KC_TAB,  KC_LGUI,   KC_RGUI, KC_ENT,  MO(1),   KC_BSPC, KC_CAPS
    ),
    [1] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
        _______, DE_AT,   DE_EXLM, DE_AMPR, DE_PIPE, DE_TILD,                     DE_UDIA, DE_QUES, DE_LBRC, DE_RBRC, DE_ODIA, _______,
        _______, DE_ADIA, DE_DLR,  DE_PLUS, DE_ASTR, DE_SLSH,                     DE_PERC, DE_LCBR, DE_LPRN, DE_RPRN, DE_RCBR, _______,
        _______, DE_SS,   DE_LABK, DE_RABK, DE_EQL,  DE_BSLS, _______,   _______, UD_ACUT, DE_HASH, UD_CIRC, UD_GRV,  KC_ESC,  _______,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    [2] = LAYOUT(
        _______, KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,                      KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  _______,
        _______, XXXXXXX, KC_PGDN, KC_UP,   KC_PGUP, KC_HOME,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_LLCK, _______,
        _______, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, KC_FIND,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, DE_ACUT, XXXXXXX, DE_CIRC, DE_GRV,  XXXXXXX, _______,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    [3] = LAYOUT(
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DF(4),
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     CU_ANIM, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     RM_TOGG, RM_PREV, RM_HUED, RM_SATD, RM_VALD, RM_SPDD,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    // US QWERTY
    [4] = LAYOUT(
        KC_LALT, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_ESC,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                        KC_J,    KC_L,    KC_U,    KC_Y,    KC_DQUO, MO(6),
        KC_LSFT, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_RSFT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE,   KC_MPLY, KC_K,    KC_M,    SU_COMM, SU_DOT,  KC_QUOT, KC_RCTL,
                          MO(7),   KC_DEL,  KC_SPC,  KC_TAB,  KC_LGUI,   KC_RGUI, KC_ENT,  MO(5),   KC_BSPC, KC_RALT
    ),
    [5] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
        _______, KC_AT,   KC_EXLM, KC_AMPR, KC_PIPE, KC_TILD,                     XXXXXXX, KC_QUES, KC_LBRC, KC_RBRC, XXXXXXX, _______,
        _______, _______, KC_DLR,  KC_PLUS, KC_ASTR, KC_SLSH,                     KC_PERC, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, _______,
        _______, _______, KC_LABK, KC_RABK, KC_EQL,  KC_BSLS, _______,   _______, XXXXXXX, KC_HASH, UD_CIRC, KC_GRV,  KC_ESC,  _______,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    [6] = LAYOUT(
        _______, KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,                      KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  _______,
        _______, XXXXXXX, KC_PGDN, KC_UP,   KC_PGUP, KC_HOME,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_LLCK, _______,
        _______, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, KC_FIND,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    [7] = LAYOUT(
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DF(8),
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     CU_ANIM, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     RM_TOGG, RM_PREV, RM_HUED, RM_SATD, RM_VALD, RM_SPDD,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
    ),
    // Gaming
    [8] = LAYOUT(
        KC_LALT, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    DF(0),
        KC_ESC,  KC_P,    KC_Q,    KC_W,    KC_E,    KC_R,                               KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_RALT,
        KC_LSFT, KC_M,    KC_A,    KC_S,    KC_D,    KC_F,                               KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_RSFT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_MUTE,   KC_MPLY, KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_QUOT, KC_RCTL,
                          MO(7),   KC_BSPC, KC_SPC,  KC_TAB, KC_LGUI,   KC_RGUI, KC_ENT, XXXXXXX, KC_BSPC, KC_RALT
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        last_activity = timer_read32();
        bool shift = get_mods() & MOD_MASK_SHIFT;
        switch (keycode) {
            case CU_ANIM:
                anim_state = (anim_state + 1) % 2;
                return false;
            // Control the send key for shifted keys
            case SD_QUOT:
                if (shift) {
                    // send " on qwertz
                    tap_code(DE_2);
                } else {
                    // send ' on qwertz
                    register_mods(MOD_MASK_SHIFT);
                    tap_code(DE_HASH);
                    unregister_mods(MOD_MASK_SHIFT);
                }
                return false;
            case SU_COMM:
                if (shift) {
                    // send ; on qwerty
                    unregister_mods(MOD_MASK_SHIFT);
                    tap_code(KC_SCLN);
                    register_mods(MOD_MASK_SHIFT);
                } else {
                    // send , on qwerty
                    tap_code(KC_COMM);
                }
                return false;
            case SU_DOT:
                if (shift) {
                    // send : on qwerty
                    tap_code(KC_SCLN);
                } else {
                    // send . on qwerty
                    tap_code(KC_DOT);
                }
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
