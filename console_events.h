#ifndef EVENTS_H
#define EVENTS_H

#include "point.h"
#include "rect.h"

typedef enum {
	EV_MOUSE = 0,
	EV_KEY_DOWN = 1,
	EV_TIMER = 2,
	EV_RESIZE = 3,
	EV_COUNT = 4, // the number of total event types
} event_type;

typedef enum {
	MB_NONE = -1,
	MB_LEFT = 0,
	MB_RIGHT = 1,
	MB_MIDDLE = 2,
	MB_COUNT = 3
} mouse_button;

typedef enum {
	ME_BUTTON_DOWN,
	ME_BUTTON_UP,
	ME_WHEEL_DOWN,
	ME_WHEEL_UP,
	ME_MOVE,
} mouse_event_type;

typedef struct {
	point coords;
	mouse_event_type mouse_ev;
	mouse_button button;
} mouse_event;

typedef struct {
	unsigned long counter;
	int active:1; // the timer event is sent to all "subscribed" windows. 
		      //this is to make them aware if the window is active or not.
} timer_event;

typedef struct {
	rect newcoords;
} resize_event;

typedef enum {
	MOD_SHIFT = 1 << 0,
	MOD_ALT   = 1 << 1,
	MOD_CTRL  = 1 << 2,
	MOD_ALTGR = 1 << 3,
	MOD_FLAG  = 1 << 4,
} key_modifier;

typedef struct {
	int ch;
	key_modifier modifiers;
} key_event;

// this struct requires std=gnu89 or gnu99 to work. i am to lazy to make the union non-anonymous.
typedef struct {
	event_type etype;
	union {
		mouse_event mouse_ev;
		timer_event timer_ev;
		key_event key_ev;
		resize_event resize_ev;
	};
} event;


#endif
