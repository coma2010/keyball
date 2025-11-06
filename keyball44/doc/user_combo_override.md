# Keyball44 ユーザーキーマップ: コンボ／オーバーライド概要

- 所在: `qmk_firmware/keyboards/keyball/keyball44/keymaps/user/keymap.c`
- 対象: ユーザー配列で有効なコンボキーとオーバーライドの挙動
- 目的: 改修時に機能を見落とさないよう、入力トリガーと出力内容を一覧化
- 最終確認日: 2025-11-06

## コンボ定義一覧

`rules.mk` で `COMBO_ENABLE = yes` が有効化されており、下表の同時押しが成立すると指定したキーコードが送出されます。特に指定がない限りベースレイヤーのキー配列が参照されます。`SCRL_MO` は押下中のみトラックボールをスクロールモードに切り替える Keyball 独自コードです（`keyboards/keyball/lib/keyball/keycodes.md` を参照）。

### カーソル／ポインタ操作

| トリガー | 出力 | 用途メモ |
| --- | --- | --- |
| `U` + `I` | `KC_UP` | 上方向カーソル移動 |
| `M` + `,` | `KC_DOWN` | 下方向カーソル移動 |
| `H` + `J` | `KC_LEFT` | 左方向カーソル移動 |
| `J` + `K` | `KC_RGHT` | 右方向カーソル移動 |
| `K` + `L` | `KC_BTN1` | マウス左クリック |
| `,` + `.` | `KC_BTN2` | マウス右クリック |
| `.` + `/` | `SCRL_MO` | 押下中のみスクロールモードを有効化 |

### 括弧・引用符入力

| トリガー | 出力 | 用途メモ |
| --- | --- | --- |
| `D` + `Y` | `KC_LPRN` | `(` |
| `D` + `U` | `KC_RPRN` | `)` |
| `D` + `H` | `KC_LCBR` | `{` |
| `D` + `J` | `KC_RCBR` | `}` |
| `D` + `N` | `KC_LBRC` | `[` |
| `D` + `M` | `KC_RBRC` | `]` |
| `D` + `K` | `KC_DQUO` | `"` |
| `D` + `I` | `KC_QUOT` | `'` |

### その他

| トリガー | 出力 | 用途メモ |
| --- | --- | --- |
| `Q` + `I` | `KC_ESC` | Escape |

## オーバーライド定義

`rules.mk` で `KEY_OVERRIDE_ENABLE = yes` が設定されており、以下の基本オーバーライドが定義されています（`ko_make_basic` を使用）。

| トリガー | 出力 | 備考 |
| --- | --- | --- |
| `Shift` + `Backspace` | `KC_DEL` | 修飾キーに Shift を含む Backspace を Delete に置換 |

## 更新時の注意

- コンボを追加／変更する際は、配列末尾の `COMBO_END` を忘れずに指定し、既存名称と衝突しないよう管理してください。
- オーバーライドは配列 `key_overrides` に追加した順で評価されます。複数を定義する場合は優先順位を意識してください。
- 大規模な変更を行う前に [`docs/features/combo.md`](https://docs.qmk.fm/#/features/combo) および [`docs/features/key_overrides.md`](https://docs.qmk.fm/#/features/key_overrides) を参照し、挙動の確認を推奨します。
