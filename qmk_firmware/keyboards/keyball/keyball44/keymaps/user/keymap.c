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
  _FUNCTION,
  _MOUSE,
  _MISC
};

// 同時押しコンボの定義。2キーの組み合わせを単発のキー/機能に割り当てる。
const uint16_t PROGMEM my_up[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM my_down[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM my_left[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM my_right[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM my_btn1[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM my_btn2[] = {KC_L, RSFT_T(KC_MINS), COMBO_END};
const uint16_t PROGMEM my_scrl_mo[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM my_esc[] = {KC_Y, KC_U, COMBO_END};
const uint16_t PROGMEM my_tab[] = {KC_N, KC_M, COMBO_END};

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
  CTL_USCR,
};

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record)
{
  // 連打時に「タップ優先」で誤判定を減らす対象キーを指定する。
  // ここに列挙したキーは、素早いタップの取りこぼしを避ける意図がある。
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

/////////////////////////
// Tap Dance: 1回/2回/長押しなどの組み合わせで異なる動作を返す仕組み。
typedef enum
{
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_SINGLE_HOLD,
  TD_DOUBLE_TAP,
  TD_DOUBLE_HOLD,
  TD_DOUBLE_SINGLE_TAP, // 単発タップを2回送る
  TD_TRIPLE_TAP,
  TD_TRIPLE_HOLD
} td_state_t;

typedef struct
{
  bool is_press_action;
  td_state_t state;
} td_tap_t;

// Tap Danceの識別子。keymaps内のTD()で参照する。
enum
{
  TD_1,
  TD_2,
  TD_3,
  TD_4,
  TD_5,
  TD_6,
  TD_7,
  TD_8,
  TD_9,
  TD_0,
  TD_00,
  TD_Q_ESC,
  TD_LPRIN,
  TD_RPRIN,
  TD_LBRC,
  TD_RBRC,
  TD_QUOT,
  TD_W_TAB,
  X_CTL,
  SOME_OTHER_DANCE
};

td_state_t cur_dance(tap_dance_state_t *state);

// XのTap Dance用宣言。どのキーマップからでも使えるようにここで宣言する。
void x_finished(tap_dance_state_t *state, void *user_data);
void x_reset(tap_dance_state_t *state, void *user_data);

/* 実行すべきTap Danceの種類に対応する値を返す。
 *
 * Tap Danceの状態判定は interrupted と pressed を使う。
 *
 * Interrupted: tapping term 内に他のキーが押された状態。
 *  通常は「タップしたい意図」を示す。
 *
 * Pressed: キーが押され続けているかどうか。
 *  true の場合、tapping term が終了しても押下が続いており、長押し意図とみなす。
 *
 * QMKのTap Danceでは、permissive hold と同等の挙動を再現できない。
 * 一般に、頻出の文字キーに高度なTap Danceを割り当てると誤作動が起きやすい。
 * 例: "A"。Tap Danceは非文字キーに置く方が安定する。
 *
 * 高度なTap Danceの配置候補:
 *  z,q,x,j,k,v,b, ファンクションキー, home/end, カンマ, セミコロン
 *
 * Tap Danceの配置が良い条件:
 *  文中で頻繁に使うキーではないこと。
 *  2連打されやすいキーではないこと。例: 端末やWebフォームでのTab連打。
 *  よくある単語で連続入力される文字ではないこと。例: "pepper" の p。
 *
 * 3点目には TD_DOUBLE_SINGLE_TAP があるが、十分に検証されていない。
 *
 */
td_state_t cur_dance(tap_dance_state_t *state)
{
  // 入力の回数/割り込み/押下継続の状態から、Tap Danceの状態を判定する。
  // ここでの判定が各finished/resetの分岐に使われる。
  if (state->count == 1)
  {
    if (state->interrupted || !state->pressed)
      return TD_SINGLE_TAP;
    // 割り込みはなく、押下が続いているため長押し(HOLD)として扱う。
    else
      return TD_SINGLE_HOLD;
  }
  else if (state->count == 2)
  {
    // TD_DOUBLE_SINGLE_TAP は "pepper" のような連続入力と、
    // 'pp' のダブルタップ操作を区別するために使う。
    // 目的は、ダブルタップ機能ではなく「同じキーの2回送出」を行うこと。
    if (state->interrupted)
      return TD_DOUBLE_SINGLE_TAP;
    else if (state->pressed)
      return TD_DOUBLE_HOLD;
    else
      return TD_DOUBLE_TAP;
  }

  // 同じ文字を素早く3回連続入力するケースは想定外。
  // 例: Tap DanceがKC_Wで "www." を素早く入力する場合は、
  // TD_TRIPLE_SINGLE_TAP を返す例外を追加し、TD_DOUBLE_SINGLE_TAP同様に定義する必要がある。
  if (state->count == 3)
  {
    if (state->interrupted || !state->pressed)
      return TD_TRIPLE_TAP;
    else
      return TD_TRIPLE_HOLD;
  }
  else
    return TD_UNKNOWN;
}

// Tap Danceの内部状態を保持する。キーごとに独立させることで誤動作を防ぐ。
static td_tap_t xtap_state = {
    .is_press_action = true,
    .state = TD_NONE};
static td_tap_t lprintap_state = {
    .is_press_action = true,
    .state = TD_NONE};
static td_tap_t rprintap_state = {
    .is_press_action = true,
    .state = TD_NONE};
static td_tap_t zerotap_state = {
    .is_press_action = true,
    .state = TD_NONE};

void x_finished(tap_dance_state_t *state, void *user_data)
{
  // XのTap Dance: 1回/長押し/2回で別のキーを出す。
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state)
  {
  case TD_SINGLE_TAP:
    register_code(KC_X);
    break;
  case TD_SINGLE_HOLD:
    register_code(KC_LCTL);
    break;
  case TD_DOUBLE_TAP:
    register_code(KC_ESC);
    break;
  case TD_DOUBLE_HOLD:
    register_code(KC_LALT);
    break;
  // 最後のケースは高速入力向け。キーが `f` の場合を想定。
  // 例: "buffer" を入力するときに `ff` を送りたいのに `Esc` が出るのを防ぐ。
  // 高速入力で `ff` を出すには、次の打鍵が `TAPPING_TERM`（既定200ms）内に必要。
  case TD_DOUBLE_SINGLE_TAP:
    tap_code(KC_X);
    register_code(KC_X);
    break;
  default:
    break;
  }
}

void x_reset(tap_dance_state_t *state, void *user_data)
{
  // 押下解除時に、送出したキーを確実に戻す。
  switch (xtap_state.state)
  {
  case TD_SINGLE_TAP:
    unregister_code(KC_X);
    break;
  case TD_SINGLE_HOLD:
    unregister_code(KC_LCTL);
    break;
  case TD_DOUBLE_TAP:
    unregister_code(KC_ESC);
    break;
  case TD_DOUBLE_HOLD:
    unregister_code(KC_LALT);
    break;
  case TD_DOUBLE_SINGLE_TAP:
    unregister_code(KC_X);
    break;
  default:
    break;
  }
  xtap_state.state = TD_NONE;
}

void lprin_finished(tap_dance_state_t *state, void *user_data)
{
  // 左かっこ系のTap Dance。回数で「()」「[]」「{}」を出し分ける。
  lprintap_state.state = cur_dance(state);
  switch (lprintap_state.state)
  {
  case TD_SINGLE_TAP:
  case TD_SINGLE_HOLD:
    register_code(KC_RSFT);
    register_code(KC_8);
    break;
  case TD_DOUBLE_TAP:
  case TD_DOUBLE_HOLD:
    register_code(JP_LBRC);
    break;
  case TD_TRIPLE_TAP:
  case TD_TRIPLE_HOLD:
    register_code(KC_RSFT);
    register_code(JP_LBRC);
    break;
  default:
    break;
  }
}

void lprin_reset(tap_dance_state_t *state, void *user_data)
{
  // 押下解除時の後始末。押した修飾/記号を元に戻す。
  switch (lprintap_state.state)
  {
  case TD_SINGLE_TAP:
  case TD_SINGLE_HOLD:
    unregister_code(KC_8);
    unregister_code(KC_RSFT);
    break;
  case TD_DOUBLE_TAP:
  case TD_DOUBLE_HOLD:
    unregister_code(JP_LBRC);
    break;
  case TD_TRIPLE_TAP:
  case TD_TRIPLE_HOLD:
    unregister_code(JP_LBRC);
    unregister_code(KC_RSFT);
    break;
  default:
    break;
  }
  lprintap_state.state = TD_NONE;
}

void rprin_finished(tap_dance_state_t *state, void *user_data)
{
  // 右かっこ系のTap Dance。回数で「)」「]」「}」を出し分ける。
  rprintap_state.state = cur_dance(state);
  switch (rprintap_state.state)
  {
  case TD_SINGLE_TAP:
  case TD_SINGLE_HOLD:
    register_code(KC_RSFT);
    register_code(KC_9);
    break;
  case TD_DOUBLE_TAP:
  case TD_DOUBLE_HOLD:
    register_code(JP_RBRC);
    break;
  case TD_TRIPLE_TAP:
  case TD_TRIPLE_HOLD:
    register_code(KC_RSFT);
    register_code(JP_RBRC);
    break;
  default:
    break;
  }
}

void rprin_reset(tap_dance_state_t *state, void *user_data)
{
  // 押下解除時の後始末。押した修飾/記号を元に戻す。
  switch (rprintap_state.state)
  {
  case TD_SINGLE_TAP:
  case TD_SINGLE_HOLD:
    unregister_code(KC_9);
    unregister_code(KC_RSFT);
    break;
  case TD_DOUBLE_TAP:
  case TD_DOUBLE_HOLD:
    unregister_code(JP_RBRC);
    break;
  case TD_TRIPLE_TAP:
  case TD_TRIPLE_HOLD:
    unregister_code(JP_RBRC);
    unregister_code(KC_RSFT);
    break;
  default:
    break;
  }
  rprintap_state.state = TD_NONE;
}

void zero_finished(tap_dance_state_t *state, void *user_data)
{
  // 0キーのTap Dance。長押し時の修飾や記号を分岐する。
  zerotap_state.state = cur_dance(state);
  switch (zerotap_state.state)
  {
  case TD_SINGLE_TAP:
  case TD_DOUBLE_HOLD:
    register_code(KC_0);
    break;
  case TD_SINGLE_HOLD:
    register_code(KC_LSFT);
    break;
  case TD_DOUBLE_TAP:
  case TD_TRIPLE_HOLD:
    register_code(JP_COLN);
    break;
  default:
    break;
  }
}

void zero_reset(tap_dance_state_t *state, void *user_data)
{
  // 押下解除時に、送出したキーを確実に解除する。
  switch (zerotap_state.state)
  {
  case TD_SINGLE_TAP:
  case TD_DOUBLE_HOLD:
    unregister_code(KC_0);
    break;
  case TD_SINGLE_HOLD:
    unregister_code(KC_LSFT);
    break;
  case TD_DOUBLE_TAP:
  case TD_TRIPLE_HOLD:
    unregister_code(JP_COLN);
    break;
  default:
    break;
  }
  zerotap_state.state = TD_NONE;
}

// void dance_lprin(tap_dance_state_t *state, void *user_data)
// {
//   switch (state->count)
//   {
//   case 1:
//     register_code(KC_LSFT);
//     tap_code(KC_8);
//     unregister_code(KC_LSFT);
//     break;
//   case 2:
//     tap_code(JP_LBRC);
//     break;
//   case 3:
//     register_code(KC_LSFT);
//     tap_code(JP_LBRC);
//     unregister_code(KC_LSFT);
//     break;
//   default:
//     tap_code(JP_LBRC);
//   }
// }

// void dance_rprin(tap_dance_state_t *state, void *user_data)
// {
//   switch (state->count)
//   {
//   case 1:
//     register_code(KC_LSFT);
//     tap_code(KC_9);
//     unregister_code(KC_LSFT);
//     break;
//   case 2:
//     tap_code(JP_RBRC);
//     break;
//   case 3:
//     register_code(KC_LSFT);
//     tap_code(JP_RBRC);
//     unregister_code(KC_LSFT);
//     break;
//   default:
//     tap_code(JP_RBRC);
//   }
// }

tap_dance_action_t tap_dance_actions[] = {
    // Tap Danceの実動作定義。IDと動作の対応を一元管理する。
    [TD_1] = ACTION_TAP_DANCE_DOUBLE(KC_1, JP_EXLM),
    [TD_2] = ACTION_TAP_DANCE_DOUBLE(KC_2, JP_AT),
    [TD_3] = ACTION_TAP_DANCE_DOUBLE(KC_3, JP_HASH),
    [TD_4] = ACTION_TAP_DANCE_DOUBLE(KC_4, JP_DLR),
    [TD_5] = ACTION_TAP_DANCE_DOUBLE(KC_5, JP_PERC),
    [TD_6] = ACTION_TAP_DANCE_DOUBLE(KC_6, JP_CIRC),
    [TD_7] = ACTION_TAP_DANCE_DOUBLE(KC_7, JP_AMPR),
    [TD_8] = ACTION_TAP_DANCE_DOUBLE(KC_8, JP_ASTR),
    [TD_9] = ACTION_TAP_DANCE_DOUBLE(KC_9, JP_SCLN),
    [TD_0] = ACTION_TAP_DANCE_DOUBLE(KC_0, JP_COLN),
    [TD_00] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, zero_finished, zero_reset),
    [TD_Q_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC),
    [TD_LPRIN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lprin_finished, lprin_reset),
    [TD_RPRIN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rprin_finished, rprin_reset),
    // [TD_LPRIN] = ACTION_TAP_DANCE_FN(dance_lprin),
    // [TD_RPRIN] = ACTION_TAP_DANCE_FN(dance_rprin),
    [TD_LBRC] = ACTION_TAP_DANCE_DOUBLE(JP_LBRC, S(JP_LBRC)),
    [TD_RBRC] = ACTION_TAP_DANCE_DOUBLE(JP_RBRC, S(JP_RBRC)),
    [TD_QUOT] = ACTION_TAP_DANCE_DOUBLE(JP_DQUO, JP_QUOT),
    [TD_W_TAB] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_TAB),
    [X_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset)};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // デフォルト(VIA)向けのキーマップ
    [_DEFAULT] = LAYOUT_universal(
        // 基本レイヤー: 文字入力の主軸。Tap/Holdやレイヤー移動を多用する。
        KC_ESC, TD(TD_Q_ESC), TD(TD_W_TAB), KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
        LSFT_T(KC_CAPS), LSFT_T(KC_A), KC_S, LT(_FUNCTION, KC_D), LT(_BRACKET, KC_F), LT(_MOUSE, KC_G), KC_H, KC_J, KC_K, KC_L, RSFT_T(KC_MINS), LT(_MISC, KC_ENT),
        LCTL_T(KC_LNG1), LCTL_T(KC_Z), KC_X, KC_C, LT(_MISC, KC_V), KC_B, KC_N, KC_M, KC_COMM, KC_DOT, LCTL_T(KC_SLSH), S(KC_INT1),
        // KC_LALT, KC_LGUI, CTL_USCR, KC_LNG8, LT(_NUMBER, KC_TAB), LSFT_T(KC_BSPC), LT(_NUMBER, KC_SPC), TG(_MOUSE), KC_LCTL, TG(_MOUSE)),
        S(JP_BSLS), KC_LALT, LCTL_T(KC_ESC), KC_LNG7, LGUI_T(KC_ENT), LSFT_T(KC_ENT), LT(_NUMBER, KC_SPC), TG(_MOUSE), KC_LCTL, TG(_MOUSE)),

    [_NUMBER] = LAYOUT_universal(
        // 数字/記号レイヤー: 右手側中心で数字と記号を集約。
        // _______, S(KC_1), KC_LBRC, S(KC_3), S(KC_4), S(KC_5), KC_EQL, S(KC_6), S(JP_COLN), JP_SCLN, JP_COLN, _______,
        // _______, LSFT_T(KC_1), LT(_MOUSE, KC_2), LT(_FUNCTION, KC_3), LT(_BRACKET, KC_4), KC_5, KC_6, KC_7, KC_8, KC_9, RSFT_T(KC_0), _______,
        // _______, LCTL_T(JP_LBRC), TD(TD_QUOT), TD(TD_LBRC), TD(TD_RBRC), KC_MINS, S(JP_CIRC), S(JP_SCLN), _______, _______, _______, _______,
        _______, S(KC_1), JP_AT, S(KC_3), S(KC_4), S(KC_5), TD(TD_QUOT), TD(TD_LBRC), TD(TD_RBRC), S(KC_8), S(KC_9), _______,
        _______, LSFT_T(KC_1), KC_2, LT(_FUNCTION, KC_3), LT(_BRACKET, KC_4), LT(_MOUSE, KC_5), TD(TD_6), TD(TD_7), TD(TD_8), TD(TD_9), TD(TD_00), _______,
        _______, LCTL_T(JP_BSLS), S(JP_AT), S(JP_CIRC), S(JP_BSLS), S(JP_YEN), S(JP_SCLN), KC_MINS, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_NUMBER), _______, TG(_NUMBER)),

    [_BRACKET] = LAYOUT_universal(
        // 括弧/編集系レイヤー: かっこ類や移動系をまとめる。
        _______, KC_ESC, KC_TAB, S_ARW, D_ARW, XXXXXXX, JP_YEN, S(JP_BSLS), KC_INS, KC_DEL, KC_BSPC, _______,
        _______, LSFT_T(KC_CAPS), KC_PGUP, LT(_FUNCTION, KC_HOME), XXXXXXX, XXXXXXX, S(KC_8), TD(TD_LBRC), TD(TD_QUOT), JP_SCLN, JP_COLN, _______,
        _______, LCTL_T(KC_LNG1), KC_PGDN, KC_END, XXXXXXX, XXXXXXX, S(KC_9), TD(TD_RBRC), _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_BRACKET), _______, TG(_BRACKET)),

    [_FUNCTION] = LAYOUT_universal(
        // ファンクションレイヤー: Fキー/記号関連を配置。
        _______, KC_F1, KC_F2, XXXXXXX, KC_F3, KC_F4, KC_PGUP, KC_UP, KC_INS, KC_DEL, KC_BSPC, _______,
        _______, KC_F5, KC_F6, XXXXXXX, KC_F7, KC_F8, KC_LEFT, KC_RIGHT, KC_BTN1, KC_BTN2, KC_BTN3, _______,
        _______, KC_F9, KC_F10, XXXXXXX, KC_F11, KC_F12, KC_PGDN, KC_DOWN, SCRL_MO, KC_BTN4, KC_BTN5, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_FUNCTION), _______, TG(_FUNCTION)),

    [_MOUSE] = LAYOUT_universal(
        // マウスレイヤー: 移動/スクロール/クリックをまとめる。
        _______, XXXXXXX, KC_HOME, KC_UP, KC_PGUP, XXXXXXX, XXXXXXX, TD(TD_LPRIN), KC_INS, KC_DEL, KC_BSPC, _______,
        _______, KC_LSFT, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, TD(TD_LPRIN), S_ARW, D_ARW, JP_SCLN, JP_COLN, _______,
        _______, KC_LCTL, KC_END, KC_DOWN, KC_PGDN, XXXXXXX, TD(TD_RPRIN), TD(TD_LBRC), TD(TD_RBRC), TD(TD_QUOT), XXXXXXX, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_MOUSE), _______, TG(_MOUSE)),

    [_MISC] = LAYOUT_universal(
        // その他レイヤー: 設定系・速度・スナップなど補助機能を集約。
        _______, TD(TD_Q_ESC), TD(TD_LPRIN), TD(TD_RPRIN), TD(TD_LBRC), TD(TD_RBRC), TO(_DEFAULT), TO(_NUMBER), TO(_BRACKET), TO(_FUNCTION), TO(_MOUSE), _______,
        _______, S_ARW, D_ARW, CPI_I1K, CPI_I100, TD(TD_QUOT), SSNP_FRE, SCRL_DVI, SSNP_VRT, KBC_RST, KC_LNG1, _______,
        _______, XXXXXXX, XXXXXXX, CPI_D1K, CPI_D100, TD(TD_W_TAB), XXXXXXX, SCRL_DVD, SSNP_HOR, KBC_SAVE, KC_LNG2, _______,
        _______, _______, _______, _______, _______, _______, _______, TG(_MISC), _______, TG(_MISC)),
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
static bool ctl_uscr_pressed = false;

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
  case KC_LNG7:
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
        tap_code(KC_GRV);
        tap_code(KC_TAB);
        lng8_pressed = false;
      }
      else
      {
        tap_code16(C(KC_M)); // ctrl+mを送る
        tap_code(KC_GRV);
        lng8_pressed = false;
        // tap_code(KC_LNG2);
        // tap_code(KC_LNG1); // 確実に英数にする
      }
    }
    return false;
    break;
  case KC_LNG8:
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
        tap_code(KC_GRV);
        lng8_pressed = false;
        tap_code(KC_ENT);
      }
      else
      {
        tap_code16(C(KC_M)); // ctrl+mを送る
        tap_code(KC_GRV);
        lng8_pressed = false;
        // tap_code(KC_LNG2);
        // tap_code(KC_LNG1); // 確実に英数にする
      }
    }
    return false;
    break;
  case KC_LNG9:
    if (record->event.pressed)
    {
      usr_timer = timer_read();
      lng8_pressed = true;
    }
    else
    {
      if (timer_elapsed(usr_timer) <= TAPPING_TERM)
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
        lng8_pressed = false;
      }
    }
    return false;
    break;
  case CTL_USCR:
    if (record->event.pressed)
    {
      usr_timer = timer_read();
      ctl_uscr_pressed = true;
    }
    else
    {
      if (timer_elapsed(usr_timer) <= TAPPING_TERM)
      {
        unregister_code(KC_RCTL);
        register_code(KC_RSFT);
        tap_code(KC_INT1);
        unregister_code(KC_RSFT);
        ctl_uscr_pressed = false;
      }
      else
      {
        unregister_code(KC_RCTL);
        ctl_uscr_pressed = false;
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
  default:
    break;
  }
  return true;
}

// void matrix_scan_user(void)
// {
//   // キー押下中の長押し判定を、一定時間経過で確定させる。
//   if (lng8_pressed && timer_elapsed(usr_timer) > TAPPING_TERM)
//   {
//     /* 将来拡張用の空き枠 */
//     tap_code(KC_GRV);
//     lng8_pressed = false;
//   }
//   if (ctl_uscr_pressed && timer_elapsed(usr_timer) > TAPPING_TERM)
//   {
//     register_code(KC_RCTL);
//     ctl_uscr_pressed = false;
//   }
// }

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
