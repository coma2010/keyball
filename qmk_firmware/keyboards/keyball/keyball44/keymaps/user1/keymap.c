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

// Custom keycodes for user-specific functionality.
// enum custom_keycodes {
//     BS_DEL = SAFE_RANGE, // Backspace on tap, Delete while any Shift modifier is active.
// };

// combo setting
// #ifdef COMBO_ENABLE
const uint16_t PROGMEM my_ui[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM my_mco[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM my_hj[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM my_jk[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM my_kl[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM my_cosl[] = {KC_COMMA, KC_SLASH, COMBO_END};
const uint16_t PROGMEM my_codot[] = {KC_COMMA, KC_DOT, COMBO_END};
const uint16_t PROGMEM my_du[] = {KC_D, KC_U, COMBO_END};
const uint16_t PROGMEM my_dj[] = {KC_D, KC_J, COMBO_END};
const uint16_t PROGMEM my_dm[] = {KC_D, KC_M, COMBO_END};
const uint16_t PROGMEM my_di[] = {KC_D, KC_I, COMBO_END};
const uint16_t PROGMEM my_dk[] = {KC_D, KC_K, COMBO_END};
const uint16_t PROGMEM my_dsc[] = {KC_D, KC_SCLN, COMBO_END};
const uint16_t PROGMEM my_dh[] = {KC_D, KC_H, COMBO_END};
const uint16_t PROGMEM my_dn[] = {KC_D, KC_N, COMBO_END};

combo_t key_combos[] = {
    COMBO(my_ui, KC_UP),
    COMBO(my_mco, KC_DOWN),
    COMBO(my_hj, KC_LEFT),
    COMBO(my_jk, KC_RGHT),
    COMBO(my_kl, KC_BTN1),
    COMBO(my_cosl, SCRL_MO),
    COMBO(my_codot, KC_BTN2),
    // COMBO(my_lsc, KC_BTN2),
    // COMBO(my_codot, SCRL_MO),
    // COMBO(my_du, S(KC_8)),
    // COMBO(my_di, S(KC_9)),
    // COMBO(my_dj, S(KC_LBRC)),
    // COMBO(my_dk, S(KC_RBRC)),
    // COMBO(my_dm, KC_LBRC),
    // COMBO(my_dsc, KC_RBRC),
    // COMBO(my_dh, S(KC_QUOT)),
    // COMBO(my_dn, KC_QUOT),
};

// # define OVR_TGL KEY_OVERRIDE_TOGGLE

// const key_override_t at_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_2, JP_AT);
// const key_override_t rprn_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_0, JP_RPRN);
// const key_override_t lcbr_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, JP_LCBR);
// const key_override_t dquo_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUOT, JP_DQUO);
// const key_override_t lbrc_key_override = ko_make_with_layers_and_negmods(0, KC_LBRC, JP_LBRC, ~0, (uint8_t) MOD_MASK_SHIFT);
// const key_override_t quot_key_override = ko_make_with_layers_and_negmods(0, KC_QUOT, JP_QUOT, ~0, (uint8_t) MOD_MASK_SHIFT);

// const key_override_t **key_overrides = (const key_override_t *[]){
//     &at_key_override,
//     &rprn_key_override,
//     &lcbr_key_override,
//     &dquo_key_override,
//     &lbrc_key_override,
//     &quot_key_override,
//     NULL
// };

// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     [_DEFAULT] = LAYOUT(
//         KC_2,    KC_0,    KC_LBRC, KC_QUOT,
//         KC_LSFT, KC_LCTL, OVR_TGL, KC_NO
//     ),
// };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , BS_DEL   ,
    KC_TAB   , LSFT_T(KC_A), KC_S  , KC_D     , LT(2, KC_F), LT(4, KC_G),                                     KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  , S(KC_7)  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , LT(3, KC_V), KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_INT1  ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),                  KC_BSPC,LT(2,KC_ENT), RCTL_T(KC_LNG2),     KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    _______ ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , _______   ,
    _______ ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , _______   ,
    _______ ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                         S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                         KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , _______  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                        S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),_______,
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
  // Auto enable scroll mode when the highest layer is 6
  keyball_set_scroll_mode(get_highest_layer(state) == 6);
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BS_DEL:
            if (record->event.pressed) {
                uint8_t saved_mods      = get_mods();
                uint8_t saved_oneshots  = get_oneshot_mods();
                uint8_t saved_weak_mods = get_weak_mods();
                bool shifted = (saved_mods | saved_oneshots | saved_weak_mods) & MOD_MASK_SHIFT;
                if (shifted) {
                    clear_mods();
                    clear_oneshot_mods();
                    clear_weak_mods();
                    tap_code(KC_DEL);
                    set_mods(saved_mods);
                    set_oneshot_mods(saved_oneshots);
                    set_weak_mods(saved_weak_mods);
                } else {
                    tap_code(KC_BSPC);
                }
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
