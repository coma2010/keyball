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

// このファイルの位置/役割: keyball44ユーザー配列のキー配置、タップダンス、独自挙動をまとめる
// 目的: キー入力の意味づけと、長押し・同時押し・レイヤー遷移のふるまいを明確化する

enum layer_number
{
  // レイヤーIDの定義。各レイヤーの目的は下のkeymapsで説明する。
  _DEFAULT = 0,
  _NUMBER,
  _BRACKET,
  _ARROW,
  _MOUSE,
  _MISC
};

// 同時押しコンボの定義。2キーの組み合わせを単発のキー/機能に割り当てる。
// const uint16_t PROGMEM my_up[] = {KC_U, KC_I, COMBO_END};
// const uint16_t PROGMEM my_down[] = {LT(_MISC, KC_M), KC_COMM, COMBO_END};
// const uint16_t PROGMEM my_left[] = {KC_H, LT(_MOUSE, KC_J), COMBO_END};
// const uint16_t PROGMEM my_right[] = {LT(_MOUSE, KC_J), LGUI_T(KC_K), COMBO_END};
const uint16_t PROGMEM my_btn1[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM my_btn2[] = {KC_L, RSFT_T(KC_MINS), COMBO_END};
// const uint16_t PROGMEM my_scrl_mo[] = {LGUI_T(KC_K), LALT_T(KC_L), COMBO_END};
// const uint16_t PROGMEM my_scrl_mo2[] = {KC_K, LALT_T(KC_L), COMBO_END};
const uint16_t PROGMEM my_scrl_mo3[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM my_tg3[] = {KC_O, KC_P, COMBO_END};
const uint16_t PROGMEM my_bs[] = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM my_esc[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM my_tab[] = {KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM my_ent[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM my_caps[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM my_caps2[] = {KC_H, KC_J, COMBO_END};

combo_t key_combos[] = {
    // COMBO(my_up, KC_UP),
    // COMBO(my_down, KC_DOWN),
    // COMBO(my_left, KC_LEFT),
    // COMBO(my_right, KC_RGHT),
    COMBO(my_btn1, KC_BTN1),
    COMBO(my_btn2, KC_BTN2),
    // COMBO(my_scrl_mo, SCRL_MO),
    // COMBO(my_scrl_mo2, SCRL_MO),
    COMBO(my_scrl_mo3, SCRL_MO),
    COMBO(my_tg3, TG(_ARROW)),
    COMBO(my_bs, KC_BSPC),
    COMBO(my_esc, KC_ESC),
    COMBO(my_tab, KC_TAB),
    COMBO(my_ent, KC_ENT),
    COMBO(my_caps, KC_CAPS),
    COMBO(my_caps2, KC_CAPS),
};

// # define OVR_TGL KEY_OVERRIDE_TOGGLE
// #include "key_override.h"

// // Shift + Backspace で Delete を送る
// const key_override_t delete_on_shift_backspace = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// // 必須: オーバーライド配列を定義 (終端は NULL)
// const key_override_t **key_overrides = (const key_override_t *[]){
//     &delete_on_shift_backspace,
//     NULL,
// };

enum custom_keycodes
{
  // SAFE_RANGE以降はユーザー独自のキーコード領域。
  S_ARW = SAFE_RANGE, // ユーザー0: ->
  D_ARW,              // ユーザー1: =>
  TRANS,              // ユーザー2: chrome 翻訳
};

enum
{
  TD_Q_ESC,
  TD_QUOT,
  TD_W_TAB,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_Q_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC),
    [TD_QUOT] = ACTION_TAP_DANCE_DOUBLE(JP_DQUO, JP_QUOT),
    [TD_W_TAB] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_TAB),
};

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record)
{
  // 連打時に「タップ優先」で誤判定を減らす対象キーを指定する。
  // ここに列挙したキーは、素早いタップの取りこぼしを避ける意図がある。
  switch (keycode)
  {
  case LCTL_T(KC_Z):
  case LSFT_T(KC_A):
  case LALT_T(KC_S):
    return true;
  default:
    return false;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // デフォルト(VIA)向けのキーマップ
    [_DEFAULT] = LAYOUT_universal(
        // 基本レイヤー: 文字入力の主軸。Tap/Holdやレイヤー移動を多用する。
        KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
        LCTL_T(KC_CAPS), LSFT_T(KC_A), KC_S, KC_D, LT(_BRACKET, KC_F), LT(_ARROW, KC_G), KC_H, KC_J, KC_K, KC_L, RSFT_T(KC_MINS), LT(_ARROW, KC_ENT),
        LALT_T(KC_TAB), LGUI_T(KC_Z), LALT_T(KC_X), KC_C, KC_V, LT(_MOUSE, KC_B), KC_N, KC_M, KC_COMM, LALT_T(KC_DOT), LGUI_T(KC_SLSH), S(KC_INT1),
        S(JP_BSLS), LALT_T(KC_CAPS), LGUI_T(KC_TAB), LCTL_T(KC_ENT), KC_LNG6, LSFT_T(KC_BSPC), LT(_NUMBER, KC_SPC), _______, _______, TO(_ARROW)),

    [_NUMBER] = LAYOUT_universal(
        // 数字/記号レイヤー: 右手側中心で数字と記号を集約。
        _______, S(KC_1), JP_AT, S(KC_3), S(KC_4), S(KC_5), JP_CIRC, S(KC_6), JP_ASTR, S(KC_8), S(KC_9), _______,
        _______, LSFT_T(KC_1), KC_2, KC_3, LT(_BRACKET, KC_4), LT(_MOUSE, KC_5), KC_6, KC_7, KC_8, KC_9, LSFT_T(KC_0), _______,
        _______, LCTL_T(JP_BSLS), S(JP_AT), S(JP_CIRC), S(JP_BSLS), S(JP_YEN), KC_PPLS, KC_MINS, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, TO(_DEFAULT)),

    [_BRACKET] = LAYOUT_universal(
        // 括弧/編集系レイヤー: かっこ類や移動系をまとめる。
        _______, KC_F1, KC_F2, KC_F3, XXXXXXX, KC_F4, KC_ESC, JP_LBRC, JP_RBRC, KC_DEL, KC_BSPC, _______,
        _______, KC_F5, KC_F6, KC_F7, XXXXXXX, KC_F8, S(KC_2), S(JP_LBRC), S(JP_RBRC), JP_SCLN, JP_COLN, _______,
        _______, KC_F9, KC_F10, KC_F11, XXXXXXX, KC_F12, S(KC_7), S(KC_8), S(KC_9), KC_TAB, KC_ENT, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_ARROW] = LAYOUT_universal(
        // ファンクションレイヤー: Fキー/記号関連を配置。
        _______, _______, _______, _______, _______, _______, SCRL_MO, S(KC_F10), KC_BTN1, KC_BTN2, KC_BTN4, _______,
        _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_LSFT, _______,
        _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_LCTL, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_MOUSE] = LAYOUT_universal(
        // マウスレイヤー: 移動/スクロール/クリックをまとめる。
        _______, _______, _______, _______, _______, _______, SCRL_MO, S(KC_F10), KC_BTN1, KC_BTN2, KC_BTN4, _______,
        _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_LSFT, _______,
        _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_LCTL, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_MISC] = LAYOUT_universal(
        // その他レイヤー: 設定系・速度・スナップなど補助機能を集約。
        _______, _______, _______, _______, _______, _______, TG(_NUMBER), TG(_BRACKET), TG(_ARROW), TG(_MOUSE), TG(_MISC), _______,
        _______, S_ARW, D_ARW, CPI_I1K, CPI_I100, _______, _______, _______, _______, _______, S(KC_F10), _______,
        _______, KBC_SAVE, AML_TO, CPI_D1K, CPI_D100, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
};
// clang-format を有効化

layer_state_t layer_state_set_user(layer_state_t state)
{
  // 最上位レイヤーが_MISＣのときにスクロールモードを自動ON。
  keyball_set_scroll_mode(get_highest_layer(state) == _MISC);
  return state;
}

// タップ/ホールド判定に使う共通タイマとフラグ。
// ここでのフラグは process_record_user と matrix_scan_user の連携に使う。
static uint16_t usr_timer = 0; // KC_LNG8 のタップ/ホールド判定用の経過時間を計測する。
static bool lng8_pressed = false;

enum key_state
{
  KEY_STATE_RELEASED = 0,
  KEY_STATE_PRESSED = 1,
  KEY_STATE_LONG_PRESSED = 2
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  // ユーザー独自キーの押下/解放をハンドリングする。
  // ここでは「短押し/長押し」で異なる送出内容に切り替える。
  switch (keycode)
  {
  case KC_LNG6: // 単押しGRV
    if (record->event.pressed)
    {
      tap_code(KC_GRV);
      usr_timer = timer_read();
      lng8_pressed = true;
    }
    else
    {
      if (timer_elapsed(usr_timer) <= TAPPING_TERM)
      {
        lng8_pressed = false;
      }
      else
      {
        // tap_code16(C(KC_M)); // ctrl+mを送る
        // tap_code16(C(KC_M)); // ctrl+mを送る
        tap_code(KC_GRV);
        lng8_pressed = false;
        // tap_code(KC_LNG2);
        // tap_code(KC_LNG1); // 確実に英数にする
      }
    }
    return false;
    break;
  case S_ARW:
    if (record->event.pressed)
    {
      // 矢印文字列を明示送出する。
      SEND_STRING("->");
    }
    break;
  case D_ARW:
    if (record->event.pressed)
    {
      // 矢印文字列を明示送出する。
      SEND_STRING("_>");
    }
    break;
  case TRANS:
    if (record->event.pressed)
    {
      // chrome 翻訳環境を明示送出する。
      tap_code16(S(KC_F1));
      SEND_STRING("h");
    }
    break;
  default:
    break;
  }
  return true;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void)
{
  // OLEDにキー情報/ボール情報/レイヤーを順に描画する。
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();
  keyball_oled_render_layerinfo();
}
#endif
