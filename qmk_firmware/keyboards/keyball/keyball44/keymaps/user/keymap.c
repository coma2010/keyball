/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "keymap_japanese.h"

// Keyball44 custom keymap: defines combos, overrides, and KC_LNG8 macro behavior.

// combo setting
// #ifdef COMBO_ENABLE
const uint16_t PROGMEM my_up[] = {KC_Y, KC_U, COMBO_END};
const uint16_t PROGMEM my_down[] = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM my_left[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM my_right[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM my_btn1[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM my_btn2[] = {KC_L, RSFT_T(KC_MINS), COMBO_END};
const uint16_t PROGMEM my_scrl_mo[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM my_esc[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM my_tab[] = {KC_M, KC_COMM, COMBO_END};

combo_t key_combos[] = {
    COMBO(my_up, KC_UP),
    COMBO(my_down, KC_DOWN),
    COMBO(my_left, KC_LEFT),
    COMBO(my_right, KC_RGHT),
    COMBO(my_btn1, KC_BTN1),
    COMBO(my_btn2, KC_BTN2),
    COMBO(my_scrl_mo, SCRL_MO),
    COMBO(my_esc, KC_ESC),
    COMBO(my_tab, KC_TAB),
};

// # define OVR_TGL KEY_OVERRIDE_TOGGLE
// #include "key_override.h"

// // Shift + Backspace -> Delete
// const key_override_t delete_on_shift_backspace = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// // 必須: オーバーライド配列を定義 (終端は NULL)
// const key_override_t **key_overrides = (const key_override_t *[]){
//     &delete_on_shift_backspace,
//     NULL,
// };

enum layer_number
{
  _DEFAULT = 0,
  _NUMBER,
  _BRACKET,
  _FUNCTION,
  _MOUSE,
  _MISC
};

enum custom_keycodes
{
  S_ARW = SAFE_RANGE, // User 0: ->
  D_ARW               // User 1: =>
};

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode)
  {
  case LCTL_T(KC_Z):
  case LSFT_T(KC_A):
  case LT(_MISC, KC_S):
    return true;
  default:
    return false;
  }
}

// Tap Dance declarations
enum
{
  TD_Q_ESC,
  TD_LPRIN,
  TD_RPRIN,
  TD_LBRC,
  TD_RBRC,
  TD_QUOT
};

void dance_lprin(tap_dance_state_t *state, void *user_data)
{
  switch (state->count)
  {
  case 1:
    register_code(KC_LSFT);
    tap_code(KC_8);
    unregister_code(KC_LSFT);
    break;
  case 2:
    tap_code(JP_LBRC);
    break;
  case 3:
    register_code(KC_LSFT);
    tap_code(JP_LBRC);
    unregister_code(KC_LSFT);
    break;
  default:
    tap_code(JP_LBRC);
  }
}

void dance_rprin(tap_dance_state_t *state, void *user_data)
{
  switch (state->count)
  {
  case 1:
    register_code(KC_LSFT);
    tap_code(KC_9);
    unregister_code(KC_LSFT);
    break;
  case 2:
    tap_code(JP_RBRC);
    break;
  case 3:
    register_code(KC_LSFT);
    tap_code(JP_RBRC);
    unregister_code(KC_LSFT);
    break;
  default:
    tap_code(JP_RBRC);
  }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_Q_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC),       // 0x5700
    [TD_LPRIN] = ACTION_TAP_DANCE_FN(dance_lprin),            // 0x5701
    [TD_RPRIN] = ACTION_TAP_DANCE_FN(dance_rprin),            // 0x5702
    [TD_LBRC] = ACTION_TAP_DANCE_DOUBLE(JP_LBRC, S(JP_LBRC)), // 0x5703
    [TD_RBRC] = ACTION_TAP_DANCE_DOUBLE(JP_RBRC, S(JP_RBRC)), // 0x5704
    [TD_QUOT] = ACTION_TAP_DANCE_DOUBLE(JP_DQUO, JP_QUOT),    // 0x5705
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // keymap for default (VIA)
    [_DEFAULT] = LAYOUT_universal(
        KC_ESC, TD(TD_Q_ESC), KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
        LSFT_T(KC_TAB), LSFT_T(KC_A), LT(_MISC, KC_S), LT(_FUNCTION, KC_D), LT(_BRACKET, KC_F), KC_G, KC_H, KC_J, KC_K, KC_L, RSFT_T(KC_MINS), LT(_MISC, KC_ENT),
        LCTL_T(KC_CAPS), LCTL_T(KC_Z), KC_X, KC_C, LT(_MOUSE, KC_V), KC_B, KC_N, KC_M, KC_COMM, KC_DOT, RCTL_T(KC_SLSH), S(KC_INT1),
        KC_LALT, KC_LGUI, LCTL_T(KC_TAB), KC_LNG8, LT(_MOUSE, KC_ESC), LSFT_T(KC_BSPC), LT(_NUMBER, KC_SPC), TG(_MOUSE), KC_RCTL, TG(_MOUSE)),

    [_NUMBER] = LAYOUT_universal(
        _______, S(KC_1), KC_LBRC, S(KC_3), S(KC_4), S(KC_5), KC_EQL, S(KC_6), S(JP_COLN), KC_MINS, S(KC_MINS), _______,
        _______, LSFT_T(KC_1), LT(_MOUSE, KC_2), LT(_FUNCTION, KC_3), LT(_BRACKET, KC_4), KC_5, KC_6, KC_7, KC_8, KC_9, RSFT_T(KC_0), _______,
        _______, LCTL_T(JP_LBRC), JP_RBRC, S(JP_LBRC), S(JP_RBRC), JP_COLN, S(JP_CIRC), S(JP_SCLN), _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_NUMBER), _______, TG(_NUMBER)),

    [_BRACKET] = LAYOUT_universal(
        _______, TD(TD_Q_ESC), D_ARW, S_ARW, XXXXXXX, XXXXXXX, S(JP_YEN), S(KC_8), S(KC_9), S(JP_AT), XXXXXXX, KC_DEL,
        _______, KC_DEL, LT(_MISC, KC_PGUP), LT(_FUNCTION, KC_HOME), XXXXXXX, XXXXXXX, S(JP_BSLS), TD(TD_LBRC), TD(TD_RBRC), JP_SCLN, JP_COLN, _______,
        _______, KC_BSPC, KC_PGDN, KC_END, XXXXXXX, XXXXXXX, XXXXXXX, TD(TD_QUOT), _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_BRACKET), _______, TG(_BRACKET)),

    [_FUNCTION] = LAYOUT_universal(
        _______, KC_F1, KC_F2, XXXXXXX, KC_F3, KC_F4, XXXXXXX, TD(TD_LPRIN), TD(TD_RPRIN), XXXXXXX, XXXXXXX, KC_BSPC,
        _______, KC_F5, LT(_MISC, KC_F6), XXXXXXX, KC_F7, KC_F8, XXXXXXX, S_ARW, D_ARW, XXXXXXX, XXXXXXX, _______,
        _______, KC_F9, KC_F10, XXXXXXX, KC_F11, KC_F12, XXXXXXX, TD(TD_LBRC), TD(TD_RBRC), TD(TD_QUOT), XXXXXXX, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_FUNCTION), _______, TG(_FUNCTION)),

    [_MOUSE] = LAYOUT_universal(
        _______, XXXXXXX, KC_HOME, KC_UP, KC_PGUP, KC_DEL, TO(_DEFAULT), TG(_FUNCTION), XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, KC_LSFT, KC_LEFT, KC_DOWN, KC_RGHT, KC_INS, TG(_NUMBER), XXXXXXX, KC_BTN1, KC_BTN2, SCRL_MO, _______,
        _______, KC_LCTL, KC_END, KC_DOWN, KC_PGDN, _______, TG(_BRACKET), TG(_MISC), KC_BTN3, KC_BTN4, KC_BTN5, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_MOUSE), _______, TG(_MOUSE)),

    [_MISC] = LAYOUT_universal(
        _______, TD(TD_Q_ESC), TD(TD_LPRIN), TD(TD_RPRIN), TD(TD_LBRC), TD(TD_RBRC), TD(TD_QUOT), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, S_ARW, D_ARW, CPI_I1K, CPI_I100, XXXXXXX, SSNP_FRE, SCRL_DVI, SSNP_VRT, KBC_RST, XXXXXXX, _______,
        _______, XXXXXXX, XXXXXXX, CPI_D1K, CPI_D100, XXXXXXX, XXXXXXX, SCRL_DVD, SSNP_HOR, KBC_SAVE, XXXXXXX, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_MISC), _______, TG(_MISC)),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state)
{
  // Auto enable scroll mode when the highest layer is 3
  keyball_set_scroll_mode(get_highest_layer(state) == _MISC);
  return state;
}

static uint16_t lng8_timer = 0; // Tracks tap vs hold window for KC_LNG8.
static bool lng8_pressed = false;

enum key_state
{
  KEY_STATE_RELEASED = 0,
  KEY_STATE_PRESSED = 1,
  KEY_STATE_LONG_PRESSED = 2
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode)
  {
  case KC_LNG8:
    if (record->event.pressed)
    {
      lng8_timer = timer_read();
      lng8_pressed = true;
    }
    else
    {
      if (timer_elapsed(lng8_timer) <= TAPPING_TERM)
      {
        // tap_code(KC_GRV);
        lng8_pressed = false;
        tap_code(KC_ENT);
      }
      else
      {
        tap_code16(C(KC_M)); // ctrl+mを送る
        tap_code(KC_GRV);
        // tap_code(KC_LNG2);
        // tap_code(KC_LNG1); // 確実に英数にする
      }
    }
    return false;
    break;
  case KC_LNG9:
    if (record->event.pressed)
    {
      lng8_timer = timer_read();
      lng8_pressed = true;
    }
    else
    {
      if (timer_elapsed(lng8_timer) <= TAPPING_TERM)
      {
        // tap_code(KC_GRV);
        lng8_pressed = false;
        tap_code(KC_ENT);
      }
      else
      {
        // tap_code16(C(KC_M));
        tap_code(KC_GRV);
        // tap_code(KC_LNG2);
        // tap_code(KC_LNG1); // 確実に英数にする
      }
    }
    return false;
    break;
  case S_ARW:
    if (record->event.pressed)
    {
      SEND_STRING("->");
    }
    break;
  case D_ARW:
    if (record->event.pressed)
    {
      SEND_STRING("_>");
    }
    break;
  default:
    if (record->event.pressed)
    {
      if (lng8_pressed)
      {
        if (timer_elapsed(lng8_timer) > TAPPING_TERM)
        {
          /* code */
          tap_code(KC_GRV);
          lng8_pressed = false;
        }
      }
    }
    break;
  }
  return true;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void)
{
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();
  keyball_oled_render_layerinfo();
}
#endif
