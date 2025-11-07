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

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_DEL   ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                        KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  , S(KC_7)  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_INT1  ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),                  KC_BSPC,LT(2,KC_ENT), RCTL_T(KC_LNG2),     KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    SSNP_FRE ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    SSNP_HOR ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                         S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                         KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                        S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),S(KC_INT3),
                  KC_0     , KC_DOT  , _______  ,         _______  , _______  ,                   KC_DEL   , _______  , _______       , _______  , _______
  ),

  [3] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                  _______  , _______  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [4] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                  _______  , _______  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [5] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                  _______  , _______  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [6] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                  _______  , _______  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state)
{
  // Auto enable scroll mode when the highest layer is 3
  keyball_set_scroll_mode(get_highest_layer(state) == 6);
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode)
  {
  case KC_LNG8:
    if (record->event.pressed)
    {
      tap_code(KC_LNG1);
    }
    else
    {
      tap_code(KC_ENT);
      tap_code(KC_LNG2);
    }
    return false;
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
