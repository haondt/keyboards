#include QMK_KEYBOARD_H

#include "oneshot.h"

#define LA_MOD MO(MOD)
#define LA_SYM MO(SYM)


enum layers {
    DEF,
    SYM,
    MOD,
    NUM,
};

enum keycodes {
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_split_3x6_3(
        KC_NO,       KC_NO,       KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,        KC_I,        KC_O,        KC_NO,       KC_NO,
        KC_NO,       KC_A,        KC_S,        KC_D,        KC_F,        KC_G,        KC_H,        KC_J,        KC_K,        KC_L,        KC_P,        KC_NO,
        KC_NO,       KC_NO,       KC_X,        KC_C,        KC_V,        KC_B,        KC_N,        KC_M,        KC_Q,        KC_Z,        KC_NO,       KC_NO,
                                  KC_NO,       KC_LSFT,     LA_MOD,      KC_SPC,      LA_SYM,      KC_NO
    ),

    [SYM] = LAYOUT_split_3x6_3(
        KC_NO,       KC_NO,       KC_AMPR,     KC_QUOT,     KC_DQUO,     KC_PERC,     KC_TILD,     KC_LPRN,     KC_RPRN,     KC_AT,       KC_NO,       KC_NO,
        KC_NO,       KC_CIRC,     KC_UNDS,     KC_COMM,     KC_DOT,      KC_HASH,     KC_GRV,      KC_LCBR,     KC_RCBR,     KC_BSLS,     KC_SLSH,     KC_NO,
        KC_NO,       KC_NO,       KC_QUES,     KC_COLN,     KC_SCLN,     KC_DLR,      KC_PIPE,     KC_LBRC,     KC_RBRC,     KC_EXLM,     KC_NO,       KC_NO,
                                  KC_NO,       _______,     _______,     _______,     _______,     KC_NO
    ),

    [MOD] = LAYOUT_split_3x6_3(
        KC_NO,       KC_NO,       KC_DEL,      KC_TAB,      KC_ESC,       KC_NO,       KC_VOLU,     KC_NO,       KC_ENT,      KC_BSPC,     KC_NO,       KC_NO,
        KC_NO,       OS_SHFT,     OS_GUI,      OS_ALT,      OS_CTRL,     LCTL(KC_Z),  KC_VOLD,     OS_CTRL,     OS_ALT,      OS_GUI,      OS_SHFT,     KC_NO,
        KC_NO,       KC_NO,       KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,       KC_F6,       KC_F7,       KC_F8,       KC_NO,       KC_NO,
                                  KC_NO,       _______,     _______,     _______,     _______,     KC_NO
    ),

    [NUM] = LAYOUT_split_3x6_3(
        KC_NO,       KC_NO,       KC_NO,       KC_UP,       KC_EQL,      KC_NO,       KC_PLUS,     KC_4,        KC_5,        KC_6,        KC_NO,       KC_NO,
        KC_NO,       KC_NO,       KC_LEFT,     KC_DOWN,     KC_RGHT,     KC_NO,       KC_MINS,     KC_1,        KC_2,        KC_3,        KC_ASTR,     KC_NO,
        KC_NO,       KC_NO,       KC_NO,       KC_LT,       KC_GT,       KC_NO,       KC_0,        KC_7,        KC_8,        KC_9,        KC_NO,       KC_NO,
                                  KC_NO,       _______,     _______,     _______,     _______,     KC_NO
    )

};


bool is_oneshot_cancel_key(uint16_t keycode) {
    switch(keycode) {
        case LA_SYM:
        case LA_MOD:
            return true;
        default:
            return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_MOD:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
        return true;
    default:
        return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_gui_state, KC_LGUI, OS_GUI,
        keycode, record
    );

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, MOD, NUM);
}
