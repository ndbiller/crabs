#include "stdafx.h"
#include "Manager.h"

// Returns the list of points from (x0, y0) to (x1, y1)
std::vector<Vector2> Helper::BresenhamLine(int x0, int y0, int x1, int y1) {
    // Optimization: it would be preferable to calculate in
    // advance the size of "result" and to use a fixed-size array
    // instead of a list.
    std::vector<Vector2> result;
	int tmp;

    bool steep = MathUtil::Abs(y1 - y0) > MathUtil::Abs(x1 - x0);
    if (steep) {
		// swap x0 and y0
		tmp = x0;
		x0 = y0;
		y0 = tmp;
		// swap x1 and y1
		tmp = x1;
		x1 = y1;
		y1 = tmp;
	}
    if (x0 > x1) {
		// swap x0 and x1
 		tmp = x0;
		x0 = x1;
		x1 = tmp;
		// swap y0 and y1
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

    int deltax = x1 - x0;
    int deltay = MathUtil::Abs(y1 - y0);
    int error = 0;
    int ystep;
    int y = y0;
    if (y0 < y1) ystep = 1; else ystep = -1;
    for (int x = x0; x <= x1; x++) {
		if (steep) result.push_back(Vector2(y, x));
        else result.push_back(Vector2(x, y));
        error += deltay;
        if (2 * error >= deltax) {
            y += ystep;
            error -= deltax;
        }
    }

    return result;
}
