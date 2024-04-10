keymap = """
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_split_3x6_3(
        KC_NO,           KC_NO,           KC_W,            KC_E,            KC_R,            KC_T,            KC_Y,            KC_U,            KC_I,            KC_O,            KC_NO,           KC_NO,
        KC_NO,           KC_A,            KC_S,            KC_D,            KC_F,            KC_G,            KC_H,            KC_J,            KC_K,            KC_L,            KC_P,            KC_NO,
        KC_NO,           KC_NO,           KC_X,            KC_C,            KC_V,            KC_B,            KC_N,            KC_M,            KC_Q,            KC_Z,            KC_NO,           KC_NO,
                                          KC_NO,           KC_LSFT,         LA_MOD,          KC_SPC,          LA_SYM,          KC_NO
    ),

    [NUM] = LAYOUT_split_3x6_3(
        KC_NO,           KC_NO,           KC_NO,           KC_UP,           KC_EQL,          KC_NO,           KC_PLUS,         KC_4,            KC_5,            KC_6,            KC_NO,           KC_NO,
        KC_NO,           KC_NO,           KC_LEFT,         KC_DOWN,         KC_RGHT,         KC_NO,           KC_MINS,         KC_1,            KC_2,            KC_3,            KC_ASTR,         KC_NO,
        KC_NO,           KC_NO,           KC_NO,           KC_LT,           KC_GT,           KC_NO,           KC_0,            KC_7,            KC_8,            KC_9,            KC_NO,           KC_NO,
                                          KC_NO,           KC_TRNS,         KC_TRNS,         KC_TRNS,         KC_TRNS,         KC_NO
    ),

    [MOD] = LAYOUT_split_3x6_3(
        KC_NO,           KC_NO,           KC_DEL,          KC_TAB,          KC_NO,           KC_NO,           KC_VOLU,         KC_NO,           KC_ENT,          KC_BSPC,         KC_NO,           KC_NO,
        KC_NO,           OS_SHFT,         OS_GUI,          OS_ALT,          OS_CTRL,         LCTL(KC_Z),      KC_VOLD,         OS_CTRL,         OS_ALT,          OS_GUI,          OS_SHFT,         KC_NO,
        KC_NO,           KC_NO,           KC_F1,           KC_F2,           KC_F3,           KC_F4,           KC_F5,           KC_F6,           KC_F7,           KC_F8,           KC_NO,           KC_NO,
                                          KC_NO,           KC_TRNS,         KC_TRNS,         KC_TRNS,         KC_TRNS,         KC_NO
    ),

    [SYM] = LAYOUT_split_3x6_3(
        KC_NO,           KC_NO,           KC_AMPR,         KC_QUOT,         KC_DQUO,         KC_PERC,         KC_TILD,         KC_LPRN,         KC_RPRN,         KC_AT,           KC_NO,           KC_NO,
        KC_NO,           KC_CIRC,         KC_UNDS,         KC_COMM,         KC_DOT,          KC_HASH,         KC_GRV,          KC_LCBR,         KC_RCBR,         KC_BSLS,         KC_SLSH,         KC_NO,
        KC_NO,           KC_NO,           KC_QUES,         KC_COLN,         KC_SCLN,         KC_DLR,          KC_PIPE,         KC_LBRC,         KC_RBRC,         KC_EXLM,         KC_NO,           KC_NO,
                                          KC_NO,           KC_TRNS,         KC_TRNS,         KC_TRNS,         KC_TRNS,         KC_NO
    )
};

"""


def get_surroundings(s):
    lines = s.split('\n')
    lead, end = lines[0], lines[-1]
    return lead, end, '\n'.join(lines[1:-1])

def get_next_d_key(s):
    key = ""
    start_from = 0
    in_key = False
    for i, c in enumerate(s):
        if c == "[":
            in_key = True
            continue
        if in_key:
            if c == "]":
                start_from = i + 1
                break
            key += c

    return key, s[start_from:]

def get_constructor_class(s):
    cls = ""
    start_from = 0
    in_cls = False
    for i, c in enumerate(s):
        if c.isalnum() or c == "_":
            in_cls = True
            cls += c
            continue
        if in_cls:
            if not c.isalnum() and c != "]":
                start_from = i
                break
            cls += c

    return cls, s[start_from:]

def get_keys(s):
    keys = []
    start_from = 0
    bracket_stack = 0
    in_keys = False
    current_key = ""
    for i, c in enumerate(s):
        if c == "(" and not in_keys:
            in_keys = True
            continue
        if in_keys:
            if c == "(":
                bracket_stack += 1
            if c == ")":
                bracket_stack -= 1
                if bracket_stack < 0:
                  start_from = i + 1
                  break
            if c.isalnum() or c in "_()":
                current_key += c
            else:
                if len(current_key) > 0:
                    keys.append(current_key)
                    current_key = ""


    return keys, s[start_from:]

def ind(n):
    return "".join(["    " for _ in range(n)])

def format(
    lead,
    end,
    keys: dict[str, tuple[str, list[str]]]
    ):
    s = []
    s.append(lead)
    max_keycode_len = 0
    spacing = 1
    for key, value in keys.items():
        cls, keycodes = value
        for keycode in keycodes:
            max_keycode_len = max(max_keycode_len, len(keycode))
    just = max_keycode_len + spacing + 2

    keycode_strs = []
    for key, value in keys.items():
        keycode_str_parts = []
        cls, keycodes = value
        keycode_str_parts.append(f"{ind(1)}[{key}] = {cls}(")

        # for keycode in keycodes:
        rows = [
            keycodes[:12],
            keycodes[12:24],
            keycodes[24:36]
            ]
        bottom_row = keycodes[36:]
        for row in rows:
            keycode_str_parts.append(f"{ind(2)}{''.join([(kc + ',').ljust(just) for kc in row])}".rstrip())
        bottom_row_str = ""
        bottom_row_str += ind(2)
        bottom_row_str += ''.join([' ']*(just)*2)
        bottom_row_str += f"{''.join([(kc + ',').ljust(just) for kc in bottom_row])}".rstrip()[:-1]

        keycode_str_parts.append(bottom_row_str)

        keycode_str_parts.append(f"{ind(1)})")
        keycode_strs.append('\n'.join(keycode_str_parts))
    s.append(',\n\n'.join(keycode_strs))


    s.append(end)
    return '\n'.join(s)


def main(s):
    s = s.strip()
    lead, end, s = get_surroundings(s)
    kd = {}
    while True:
        key, s = get_next_d_key(s)
        if (len(key) == 0):
            break
        cls, s = get_constructor_class(s)
        keys, s = get_keys(s)
        kd[key] = (cls, keys)
    fmt = format(lead, end, kd)

    print(fmt)

main(keymap)
