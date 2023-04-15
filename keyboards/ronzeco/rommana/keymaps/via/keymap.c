/* Copyright 2021 Leonardus Dipa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum custom_layers {
    _DEF,
    _SYM,
    _NAV,
    _FUN
};

enum custom_keycodes {
  ALT_TAB,
  CTRL_TAB
};

bool is_alt_tab_active = false;
bool is_ctrl_tab_active = false;
uint16_t alt_tab_timer = 0;
uint16_t ctrl_tab_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if(is_alt_tab_active && record->event.pressed && keycode!=ALT_TAB && keycode!=KC_LALT && keycode!=KC_LSFT && keycode!=KC_RSFT)
  {
        unregister_code(KC_LALT);
        is_alt_tab_active=false;
  }
  switch (keycode) { // This will do most of the grunt work with the keycodes.
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
    case CTRL_TAB:
      if (record->event.pressed) {
        if (!is_ctrl_tab_active) {
          is_ctrl_tab_active = true;
          register_code(KC_LCTL);
        }
        ctrl_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
  }
  return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 500) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
  if (is_ctrl_tab_active) {
    if (timer_elapsed(ctrl_tab_timer) > 500) {
      unregister_code(KC_LCTL);
      is_ctrl_tab_active = false;
    }
  }
}

#define LTNB LT(_NAV,KC_BSPC)
#define LTSE LT(_SYM,KC_ENT)
#define LTFB LT(_FUN,KC_BSPC)
#define ____ KC_TRNS
#define ALF4 LALT(KC_F4)
#define CTF4 LCTL(KC_F4)
#define OSCT OSM(MOD_LCTL)
#define OSAT OSM(MOD_LALT)
#define OSGU OSM(MOD_LGUI)
#define COPY LCTL(KC_C)
#define PAST LCTL(KC_V)
#define WSLF MEH(KC_R)
#define WSRG MEH(KC_S)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* DEFAULT
   *        ,----------------------------------.             ,----------------------------------.
   *        |   W  |   C  |   G  |   D  |   K  |             |   Y  |   U  |   I  |   O  |   P  |
   *        |------+------+------+------+------|             |------+------+------+------+------|
   *        |   R  |   S  |   T  |   H  |   B  |             |   H  |   J  |   K  |   L  |   ;  |
   *        `------+------+------+------+------|             |------+------+------+------+------'
   *               |   F  |   M  |   P  |      |             |      |   M  |   ,  |   .  |
   *               |------+------+------+------|             |------+------+------+------|
   *               |     Space   |   Shift     |             |  BSPC (nav) | Enter (sym) |
   *               `---------------------------'             `---------------------------'
   */
    [_DEF] = LAYOUT(
        KC_W, KC_C, KC_G, KC_D, KC_K,         KC_Q, KC_L, KC_U,  KC_O, KC_Y,
        KC_R, KC_S, KC_T, KC_H, KC_B,         KC_J, KC_N, KC_E,  KC_A, KC_I,
              KC_F, KC_M, KC_P,                     KC_Z, KC_X,  KC_V,
                      KC_SPC, KC_LSFT,        LTNB, LTSE
    ),
  /* SYMBOLS
   *        ,----------------------------------.             ,----------------------------------.
   *        |   |  |   Ñ  |   [  |   ]  |   `  |             |   \  |   7  |   8  |   9  |   ,  |
   *        |------+------+------+------+------|             |------+------+------+------+------|
   *        |   -  |   '  |   ;  |   :  |   =  |             |   0  |   4  |   5  |   6  |   .  |
   *        `------+------+------+------+------|             |------+------+------+------+------'
   *               |   /  |   {  |   }  |      |             |      |   1  |   2  |   3  |
   *               |------+------+------+------|             |------+------+------+------|
   *               |     Sfift   |   RAlt      |             |  BSPC (fun) |             |
   *               `---------------------------`             `---------------------------'
   */
    [_SYM] = LAYOUT(
        KC_PIPE, RALT(KC_N), KC_LBRC, KC_RBRC, KC_GRV,         KC_BSLS, KC_7, KC_8,  KC_9, KC_COMM,
        KC_MINS, KC_QUOT,    KC_SCLN, KC_COLN, KC_EQL,         KC_0,    KC_4, KC_5,  KC_6, KC_DOT,
                 KC_SLSH,    KC_LPRN, KC_RPRN,                          KC_1, KC_2,  KC_3,
                                      KC_LSFT, KC_RALT,        LTFB,    ____
    ),
  /* NAVIGATION
   *        ,----------------------------------.             ,----------------------------------.
   *        | CTF4 | CTTB | ALTB | ALF4 | VOLU |             | HOME | PGDN | PGUP | PLAY |  INS |
   *        |------+------+------+------+------|             |------+------+------+------+------|
   *        |  TAB | LCTL | LGUI | LALT | VOLD |             |  END |  <-  |  DN  |  UP  |  ->  |
   *        `------+------+------+------+------|             |------+------+------+------+------'
   *               | WSLF | WSRG | MUTE |      |             |      |  DEL |  <<  |  >>  |
   *               |------+------+------+------|             |------+------+------+------|
   *               |     Copy    |    Paste    |             |             |             |
   *               `---------------------------`             `---------------------------'
   */
    [_NAV] = LAYOUT(
        CTF4,   CTRL_TAB, ALT_TAB, ALF4, KC_VOLU,       KC_HOME, KC_PGDN, KC_PGUP,  KC_MPLY, KC_INS,
        KC_TAB, OSCT,     OSGU,    OSAT, KC_VOLD,       KC_END,  KC_LEFT, KC_DOWN,  KC_UP, KC_RIGHT,
                WSLF,     WSRG,    KC_MUTE,                      KC_DEL,  KC_MPRV,  KC_MNXT,
                                   COPY, PAST,             ____, ____
    ),
  /* FUNCTION
   *        ,----------------------------------.             ,----------------------------------.
   *        |  F1  |  F2  |  F3  |  F4  |  F5  |             | HOME | PGDN | PGUP | PLAY |  INS |
   *        |------+------+------+------+------|             |------+------+------+------+------|
   *        |  F6  |  F7  |  F8  |  F9  |  F10 |             |  END |  <-  |  DN  |  UP  |  ->  |
   *        `------+------+------+------+------|             |------+------+------+------+------'
   *               | CAPS |  F12 |  F11 |      |             |      |  DEL |  <<  |  >>  |
   *               |------+------+------+------|             |------+------+------+------|
   *               |             |             |             |             |             |
   *               `---------------------------`             `---------------------------'
   */
    [_FUN] = LAYOUT(
        KC_F1, KC_F2,   KC_F3,  KC_F4, KC_F5,          ____, ____, ____, ____, ____,
        KC_F6, KC_F7,   KC_F8,  KC_F9, KC_F10,         ____, ____, ____, ____, ____,
               KC_CAPS, KC_F12, KC_F11,                      ____, ____, ____,
                                ____,  ____,           ____, ____
    ),
