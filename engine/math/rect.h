#ifndef UI_RECT_H_
#define UI_RECT_H_

/*
 * class Point and Rect
 * coordinate x from left to right
 * coordinate y from top to bottom
 */

template <class T>
class BasePoint {
public:
    BasePoint() : x_(0), y_(0) {}

    BasePoint(T x, T y) : x_(x), y_(y) {}

    T x() const { return x_; }
    T y() const { return y_; }

private:
    T x_;
    T y_;
};

template <class T>
class BaseSize {
public:
    BaseSize() : width_(0), height_(0) {}

    BaseSize(T width, T height) : width_(width), height_(height) {}

    T Width() const { return width_; }

    T Height() const { return height_; }

private:
    T width_;
    T height_;
};

template <class T>
class BaseRect {
public:
    BaseRect() : left_(0), top_(0), right_(0), bottom_(0) {}

    BaseRect(T width, T height)
        : left_(0), top_(0), right_(width), bottom_(height) {}

    BaseRect(T left, T top, T right, T bottom)
        : left_(left), top_(top), right_(right), bottom_(bottom) {}

    template <class NumberType>
    bool Contains(const BasePoint<NumberType>& point) const {
        return point.x() >= left_ && point.x() <= right_ && point.y() >= top_ &&
               point.y() <= bottom_;
    }

    void Move(T dx, T dy) {
        left_ += dx;
        top_ += dy;
        right_ += dx;
        bottom_ += dy;
    }

    void MoveTo(T x, T y) {
        T w = right_ - left_;
        T h = bottom_ - top_;
        left_ = x;
        top_ = y;
        right_ = left_ + w;
        bottom_ = top_ + h;
    }

    T Left() const { return left_; }

    void SetLeft(T left) { left_ = left; }

    T Right() const { return right_; }

    T Top() const { return top_; }

    void SetTop(T top) { top_ = top; }

    T Bottom() const { return bottom_; }

    T Width() const { return right_ - left_; }

    void SetWidth(T width) { right_ = left_ + width; }

    T Height() const { return bottom_ - top_; }

    void SetHeight(T height) { bottom_ = top_ + height; }

    BasePoint<T> Origin() const { return BasePoint<T>(left_, top_); }

    BaseSize<T> Size() const {
        return BaseSize<T>(right_ - left_, bottom_ - top_);
    }

    void SetSize(const BaseSize<T>& size) {
        right_ = left_ + size.Width();
        bottom_ = top_ + size.Height();
    }

    void SetSize(T width, T height) {
        right_ = left_ + width;
        bottom_ = top_ + height;
    }

private:
    T left_;
    T top_;
    T right_;
    T bottom_;
};

typedef BasePoint<float> Point;
typedef BaseSize<float> Size;
typedef BaseRect<float> Rect;

#endif  // UI_RECT_H_
