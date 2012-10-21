#ifndef CONSOLE_KEYS_H
#define CONSOLE_KEYS_H

/* good old DOS...
#define CURSOR_UP 72
#define CURSOR_DOWN 80
#define CURSOR_LEFT 75
#define CURSOR_RIGHT 77
*/

// we use first 9 bits for keys and special keys.
#define CK_UNDEF 0
#define CK_ERR CK_UNDEF
#define CK_QUIT 1
#define CK_TAB 9
#define CK_RETURN '\r'
#define CK_ESCAPE 27
// +/- on numeric pad
#define CK_PLUS 43
#define CK_MINUS 45

#define CK_PAGE_UP 0x100
#define CK_PAGE_DOWN 0x101
#define CK_CURSOR_UP 0x102
#define CK_CURSOR_DOWN 0x103
#define CK_CURSOR_LEFT 0x104
#define CK_CURSOR_RIGHT 0x105
#define CK_LSHIFT 0x106
#define CK_RSHIFT 0x107
#define CK_LALT 0x108
#define CK_RALT 0x109
#define CK_ALTGR 0x10A
#define CK_LFLAG 0x10B
#define CK_RFLAG 0x10C
#define CK_CONTEXT 0x10D
#define CK_LCTRL 0x10E
#define CK_RCTRL 0x10F
#define CK_F1 0x110
#define CK_F2 0x111
#define CK_F3 0x112
#define CK_F4 0x113
#define CK_F5 0x114
#define CK_F6 0x115
#define CK_F7 0x116
#define CK_F8 0x117
#define CK_F9 0x118
#define CK_F10 0x119
#define CK_F11 0x11A
#define CK_F12 0x11B
#define CK_DEL 0x11C
#define CK_BACKSPACE 0x11D
//#define CK_RETURN 0x11E
// 0x11E is free for usage
#define CK_HOME 0x11F
#define CK_END 0x120
#define CK_INS 0x121


#define CK_MASK ((1 << 9) - 1)

// use higher bits for events and modifiers.
#define CK_EVENT_BIT_FIRST 9
#define CK_RESIZE_EVENT (1 << CK_EVENT_BIT_FIRST)
#define CK_EVENT_BIT_LAST (CK_EVENT_BIT_FIRST + 1)
#define CK_MOUSE_EVENT (1 << (CK_EVENT_BIT_LAST))
#define CK_EVENT_MASK (1 << CK_EVENT_BIT_FIRST) || (1 << (CK_EVENT_BIT_LAST))

#define CK_MOD_BIT_FIRST (CK_EVENT_BIT_LAST + 1)
#define CK_AND_EVENT_MASK ((1 << CK_MOD_BIT_FIRST) - 1)

#define CK_MOD_ALT (1 << (CK_MOD_BIT_FIRST))
#define CK_MOD_ALTGR (1 << (CK_MOD_BIT_FIRST + 1))
#define CK_MOD_SHIFT (1 << (CK_MOD_BIT_FIRST + 2))
#define CK_MOD_CTRL (1 << (CK_MOD_BIT_FIRST + 3))
#define CK_MOD_FLAG (1 << (CK_MOD_BIT_FIRST + 4))
#define CK_MOD_CONTEXT (1 << (CK_MOD_BIT_FIRST + 5))
#define CK_MOD_NUMERICPAD (1 << (CK_MOD_BIT_FIRST + 6))

#endif
