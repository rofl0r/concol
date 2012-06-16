#ifndef POINT_H
#define POINT_H

typedef struct {
	int x;
	int y;
} point;

#define point_equal(A, B) ((A).x == (B).x && (A).y == (B).y)
#define make_point(A, B) ((point) {(A), (B)})

#endif
