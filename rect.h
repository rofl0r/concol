#ifndef RECT_H
#define RECT_H

#include "point.h"

typedef struct {
	point topleft;
	point bottomright;
} rect;

#define rect_zero {{0,0},{0,0}}

#define width_from_rect(RECT_PTR) ((RECT_PTR)->bottomright.x + 1 - (RECT_PTR)->topleft.x)
#define height_from_rect(RECT_PTR) ((RECT_PTR)->bottomright.y + 1 - (RECT_PTR)->topleft.y)
#define make_rect(RECT_PTR, X1, Y1, X2, Y2) do { (RECT_PTR)->topleft.x = X1; (RECT_PTR)->topleft.y = Y1; \
		 (RECT_PTR)->bottomright.x = X2; (RECT_PTR)->bottomright.y = Y2; } while(0)
#define point_in_rect(POINT_PTR, RECT_PTR) ((POINT_PTR)->x >= (RECT_PTR)->topleft.x && \
					    (POINT_PTR)->x <= (RECT_PTR)->bottomright.x && \
					    (POINT_PTR)->y >= (RECT_PTR)->topleft.y && \
					    (POINT_PTR)->y <= (RECT_PTR)->bottomright.y )
#define point_member_in_rect(POINT_PTR, RECT_PTR, IS_X) (\
					    ((IS_X) ? ( \
					     (POINT_PTR)->x >= (RECT_PTR)->topleft.x && \
					     (POINT_PTR)->x <= (RECT_PTR)->bottomright.x \
					    ) : ( \
					     (POINT_PTR)->y >= (RECT_PTR)->topleft.y && \
					     (POINT_PTR)->y <= (RECT_PTR)->bottomright.y \
					    )))

#endif
