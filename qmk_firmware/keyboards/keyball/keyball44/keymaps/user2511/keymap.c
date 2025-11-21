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
const uint16_t PROGMEM my_up[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM my_down[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM my_left[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM my_right[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM my_btn1[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM my_btn2[] = {KC_COMMA, KC_DOT, COMBO_END};
const uint16_t PROGMEM my_scrl_mo[] = {KC_DOT, KC_SLSH, COMBO_END};
const uint16_t PROGMEM my_lprn[] = {KC_D, KC_U, COMBO_END};
const uint16_t PROGMEM my_rprn[] = {KC_D, KC_I, COMBO_END};
const uint16_t PROGMEM my_lcbr[] = {KC_D, KC_J, COMBO_END};
const uint16_t PROGMEM my_rcbr[] = {KC_D, KC_K, COMBO_END};
const uint16_t PROGMEM my_lbrc[] = {KC_D, KC_M, COMBO_END};
const uint16_t PROGMEM my_rbrc[] = {KC_D, KC_COMMA, COMBO_END};
const uint16_t PROGMEM my_dquo[] = {KC_D, KC_O, COMBO_END};
const uint16_t PROGMEM my_quot[] = {KC_D, KC_L, COMBO_END};
const uint16_t PROGMEM my_esc[] = {KC_Q, KC_W, COMBO_END};

combo_t key_combos[] = {
    COMBO(my_up, KC_UP),
    COMBO(my_down, KC_DOWN),
    COMBO(my_left, KC_LEFT),
    COMBO(my_right, KC_RGHT),
    COMBO(my_btn1, KC_BTN1),
    COMBO(my_btn2, KC_BTN2),
    COMBO(my_scrl_mo, SCRL_MO),
    COMBO(my_lprn, S(KC_8)),
    COMBO(my_rprn, S(KC_9)),
    COMBO(my_lcbr, S(KC_RBRC)),
    COMBO(my_rcbr, S(KC_NUHS)),
    COMBO(my_lbrc, KC_RBRC),
    COMBO(my_rbrc, KC_NUHS),
    COMBO(my_dquo, S(KC_2)),
    COMBO(my_quot, S(KC_7)),
    COMBO(my_esc, KC_ESC),
};

// # define OVR_TGL KEY_OVERRIDE_TOGGLE
// #include "key_override.h"

// Shift + Backspace -> Delete
const key_override_t delete_on_shift_backspace = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// 必須: オーバーライド配列を定義 (終端は NULL)
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_on_shift_backspace,
    NULL,
};

enum layer_number
{
  _DEFAULT = 0,
  _NUMBER,
  _BRACKET,
  _FUNCTION,
  _CURSOR,
  _MOUSE,
  _MISC
};

enum custom_keycodes
{
  S_ARW = SAFE_RANGE,
  D_ARW
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // keymap for default (VIA)
    [_DEFAULT] = LAYOUT_universal(
        KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
        LSFT_T(KC_TAB), LSFT_T(KC_A), LT(6, KC_S), LT(3, KC_D), LT(2, KC_F), KC_G, KC_H, KC_J, KC_K, KC_L, RSFT_T(KC_MINS), LT(6, KC_ENT),
        LCTL_T(KC_CAPS), LCTL_T(KC_Z), KC_X, KC_C, LT(5, KC_V), KC_B, KC_N, KC_M, KC_COMM, KC_DOT, RCTL_T(KC_SLSH), S(KC_INT1),
        KC_LALT, KC_LGUI, LCTL_T(KC_TAB), KC_LNG8, LT(5, KC_ESC), LSFT_T(KC_BSPC), LT(1, KC_SPC), TG(5), KC_RCTL, TG(_MOUSE)),

    [_NUMBER] = LAYOUT_universal(
        _______, S(KC_1), KC_LBRC, S(KC_3), S(KC_4), S(KC_5), KC_EQL, S(KC_6), S(JP_COLN), KC_MINS, S(KC_MINS), _______,
        _______, LSFT_T(KC_1), LT(6, KC_2), LT(3, KC_3), LT(2, KC_4), KC_5, KC_6, KC_7, KC_8, KC_9, RSFT_T(KC_0), _______,
        _______, LCTL_T(JP_LBRC), JP_RBRC, S(JP_LBRC), S(JP_RBRC), JP_COLN, S(JP_CIRC), S(JP_SCLN), _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_NUMBER), _______, TG(_NUMBER)),

    [_BRACKET] = LAYOUT_universal(
        _______, _______, D_ARW, S_ARW, XXXXXXX, XXXXXXX, S(JP_YEN), S(KC_8), S(KC_9), S(KC_2), S(KC_7), _______,
        _______, KC_DEL, LT(6, KC_PGUP), LT(3, KC_HOME), XXXXXXX, XXXXXXX, S(JP_BSLS), S(JP_LBRC), S(JP_RBRC), JP_SCLN, JP_COLN, _______,
        _______, KC_BSPC, KC_PGDN, KC_END, XXXXXXX, XXXXXXX, XXXXXXX, JP_LBRC, JP_RBRC, S(JP_AT), _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_BRACKET), _______, TG(_BRACKET)),

    [_FUNCTION] = LAYOUT_universal(
        _______, KC_F1, KC_F2, _______, KC_F3, KC_F4, _______, _______, _______, _______, _______, _______,
        _______, KC_F5, KC_F6, _______, KC_F7, KC_F8, _______, S_ARW, D_ARW, _______, _______, _______,
        _______, KC_F9, KC_F10, _______, KC_F11, KC_F12, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_FUNCTION), _______, TG(_FUNCTION)),

    [_CURSOR] = LAYOUT_universal(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_CURSOR), _______, TG(_CURSOR)),

    [_MOUSE] = LAYOUT_universal(
        _______, _______, KC_HOME, KC_UP, KC_PGUP, KC_DEL, TO(0), TG(3), _______, _______, _______, _______,
        _______, KC_LSFT, KC_LEFT, KC_DOWN, KC_RGHT, KC_INS, TG(1), TG(4), _______, _______, _______, _______,
        _______, KC_LCTL, KC_END, KC_DOWN, KC_PGDN, _______, TG(2), TG(6), _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_MOUSE), _______, TG(_MOUSE)),

    [_MISC] = LAYOUT_universal(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_MISC), _______, TG(_MISC)),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state)
{
  // Auto enable scroll mode when the highest layer is 3
  keyball_set_scroll_mode(get_highest_layer(state) == 6);
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
  // case MO6_USCR:
  //   if (record->event.pressed)
  //   {

  //   }
  case S_ARW:
    if (record->event.pressed)
    {
      SEND_STRING("->");
    }
    break;
  case D_ARW:
    if (record->event.pressed)
    {
      SEND_STRING("=>");
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
