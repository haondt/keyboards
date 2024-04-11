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
        _______,     _______,     KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,        KC_I,        KC_O,        _______,     _______,
        _______,     KC_A,        KC_S,        KC_D,        KC_F,        KC_G,        KC_H,        KC_J,        KC_K,        KC_L,        KC_P,        _______,
        _______,     _______,     KC_X,        KC_C,        KC_V,        KC_B,        KC_N,        KC_M,        KC_Q,        KC_Z,        _______,     _______,
                                  KC_LCTL,     KC_LSFT,     LA_MOD,      KC_SPC,      LA_SYM,      KC_LALT
    ),

    [SYM] = LAYOUT_split_3x6_3(
        _______,     _______,     KC_AMPR,     KC_QUOT,     KC_DQUO,     KC_PERC,     KC_SLSH,    KC_LPRN,     KC_RPRN,     KC_GRV,      _______,     _______,
        _______,     KC_CIRC,     KC_UNDS,     KC_COMM,     KC_DOT,      KC_DLR,      KC_TILD,    KC_LCBR,     KC_RCBR,     KC_BSLS,     KC_AT,       _______,
        _______,     _______,     KC_QUES,     KC_SCLN,     KC_COLN,     KC_HASH,     KC_PIPE,    KC_LBRC,     KC_RBRC,     KC_EXLM,     _______,     _______,
                                  _______,     _______,     _______,     _______,     _______,   _______
    ),

    [MOD] = LAYOUT_split_3x6_3(
        _______,     _______,     KC_DEL,      KC_TAB,      KC_ESC,      _______,     _______,     KC_VOLD,     KC_VOLU,     KC_BSPC,     _______,     _______,
        _______,     OS_SHFT,     OS_GUI,      OS_ALT,      OS_CTRL,     LCTL(KC_Z),  KC_ENT,      OS_CTRL,     OS_ALT,      OS_GUI,      OS_SHFT,     _______,
        _______,     _______,     KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,       KC_F6,       KC_F7,       KC_F8,       _______,     _______,
                                  _______,     _______,     _______,     _______,     _______,   _______
    ),

    [NUM] = LAYOUT_split_3x6_3(
        _______,     _______,     _______,     KC_UP,       KC_EQL,      _______,     KC_PLUS,     KC_4,        KC_5,        KC_6,        _______,     _______,
        _______,     _______,     KC_LEFT,     KC_DOWN,     KC_RGHT,     _______,     KC_MINS,     KC_1,        KC_2,        KC_3,        KC_ASTR,     _______,
        _______,     _______,     _______,     KC_LT,       KC_GT,       _______,     KC_0,        KC_7,        KC_8,        KC_9,        _______,     _______,
                                  _______,     _______,     _______,     _______,     _______,   _______
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
