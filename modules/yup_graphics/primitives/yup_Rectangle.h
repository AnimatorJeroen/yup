/*
  ==============================================================================

   This file is part of the YUP library.
   Copyright (c) 2024 - kunitoki@gmail.com

   YUP is an open source library subject to open-source licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace yup
{

//==============================================================================
/** Represents a rectangle in 2D space.

    This class template provides a way to store and manipulate rectangles defined by their position
    (x, y) and size (width, height). The ValueType template parameter determines the type of these values
    (e.g., int, float). This class includes various methods for rectangle manipulations such as translation,
    scaling, intersection checks, and transformations among others.
*/
template <class ValueType>
class JUCE_API Rectangle
{
public:
    //==============================================================================
    /** Default constructor, initializes a rectangle with default values.

        Creates a rectangle where all the dimensions and positions are set to zero.
    */
    constexpr Rectangle() noexcept = default;

    /** Constructs a rectangle with specified x, y, width, and height.

        @param x The x-coordinate of the top-left corner.
        @param y The y-coordinate of the top-left corner.
        @param width The width of the rectangle.
        @param height The height of the rectangle.
    */
    constexpr Rectangle (ValueType x, ValueType y, ValueType width, ValueType height) noexcept
        : xy (x, y)
        , size (width, height)
    {
    }

    /** Constructs a rectangle with specified x, y coordinates and size.

        @param x The x-coordinate of the top-left corner.
        @param y The y-coordinate of the top-left corner.
        @param size The size of the rectangle as a Size object.
    */
    constexpr Rectangle (ValueType x, ValueType y, const Size<ValueType>& size) noexcept
        : xy (x, y)
        , size (size)
    {
    }

    /** Constructs a rectangle with specified position as a Point and dimensions.

        @param xy The position of the top-left corner as a Point.
        @param width The width of the rectangle.
        @param height The height of the rectangle.
    */
    constexpr Rectangle (const Point<ValueType>& xy, ValueType width, ValueType height) noexcept
        : xy (xy)
        , size (width, height)
    {
    }

    /** Constructs a rectangle with specified position as a Point and size as a Size object.

        @param xy The position of the top-left corner as a Point.
        @param size The size of the rectangle.
    */
    constexpr Rectangle (const Point<ValueType>& xy, const Size<ValueType>& size) noexcept
        : xy (xy)
        , size (size)
    {
    }

    /** Constructs a rectangle by converting from another type.

        This constructor template allows for the creation of a Rectangle from another Rectangle of a different
        ValueType, converting the position and size appropriately.

        @tparam T The type of the value from the source rectangle.

        @param other The source rectangle from which to convert.
    */
    template <class T, class = std::enable_if_t<! std::is_same_v<T, ValueType>>>
    constexpr Rectangle (const Rectangle<T>& other) noexcept
        : xy (other.getPosition().template to<ValueType>())
        , size (other.getSize().template to<ValueType>())
    {
        static_assert (std::numeric_limits<ValueType>::max() >= std::numeric_limits<T>::max(), "Invalid narrow cast");
    }

    //==============================================================================
    /** Copy and move constructors and assignment operators. */
    constexpr Rectangle (const Rectangle& other) noexcept = default;
    constexpr Rectangle (Rectangle&& other) noexcept = default;
    constexpr Rectangle& operator= (const Rectangle& other) noexcept = default;
    constexpr Rectangle& operator= (Rectangle&& other) noexcept = default;

    //==============================================================================
    /** Returns the x-coordinate of the rectangle's top-left corner.

        @return The x-coordinate value.
    */
    [[nodiscard]] constexpr ValueType getX() const noexcept
    {
        return xy.getX();
    }

    // TODO - doxygen
    constexpr Rectangle& setX (ValueType newX) noexcept
    {
        xy.setX (newX);
        return *this;
    }

    /** Returns a new rectangle with the x-coordinate of the top-left corner set to a new value.

        This method creates a new rectangle with the same size and y-coordinate, but with the x-coordinate of the top-left corner changed to the specified value.

        @param newX The new x-coordinate for the top-left corner.

        @return A new rectangle with the updated x-coordinate.
    */
    [[nodiscard]] constexpr Rectangle withX (ValueType newX) const noexcept
    {
        return { xy.withX (newX), size };
    }

    //==============================================================================
    /** Returns the y-coordinate of the rectangle's top-left corner.

        @return The y-coordinate value.
    */
    [[nodiscard]] constexpr ValueType getY() const noexcept
    {
        return xy.getY();
    }

    // TODO - doxygen
    constexpr Rectangle& setY (ValueType newY) noexcept
    {
        xy.setY (newY);
        return *this;
    }

    /** Returns a new rectangle with the y-coordinate of the top-left corner set to a new value.

        This method creates a new rectangle with the same size and x-coordinate, but with the y-coordinate of the top-left corner changed to the specified value.

        @param newY The new y-coordinate for the top-left corner.

        @return A new rectangle with the updated y-coordinate.
    */
    [[nodiscard]] constexpr Rectangle withY (ValueType newY) const noexcept
    {
        return { xy.withY (newY), size };
    }

    //==============================================================================
    /** Returns the width of the rectangle.

        @return The width value.
    */
    [[nodiscard]] constexpr ValueType getWidth() const noexcept
    {
        return size.getWidth();
    }

    // TODO - doxygen
    constexpr Rectangle& setWidth (ValueType newWidth) noexcept
    {
        size.setWidth (newWidth);
        return *this;
    }

    /** Returns a new rectangle with the width set to a new value.

        This method creates a new rectangle with the same position but changes the width to the specified value.
        The height remains unchanged.

        @param newWidth The new width for the rectangle.

        @return A new rectangle with the updated width.
    */
    [[nodiscard]] constexpr Rectangle withWidth (ValueType newWidth) const noexcept
    {
        return { xy, size.withWidth (newWidth) };
    }

    //==============================================================================
    /** Returns the height of the rectangle.

        @return The height value.
    */
    [[nodiscard]] constexpr ValueType getHeight() const noexcept
    {
        return size.getHeight();
    }

    // TODO - doxygen
    constexpr Rectangle& setHeight (ValueType newHeight) noexcept
    {
        size.setHeight (newHeight);
        return *this;
    }

    /** Returns a new rectangle with the height set to a new value.

        This method creates a new rectangle with the same position but changes the height to the specified value.
        The width remains unchanged.

        @param newHeight The new height for the rectangle.

        @return A new rectangle with the updated height.
    */
    [[nodiscard]] constexpr Rectangle withHeight (ValueType newHeight) const noexcept
    {
        return { xy, size.withHeight (newHeight) };
    }

    //==============================================================================
    /** Returns the position of the rectangle's top-left corner as a Point.

        @return The position as a Point object.
    */
    [[nodiscard]] constexpr Point<ValueType> getPosition() const noexcept
    {
        return xy;
    }

    /** Sets the position of the rectangle's top-left corner.

        @param newPosition The new position for the top-left corner as a Point.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& setPosition (const Point<ValueType>& newPosition) noexcept
    {
        xy = newPosition;

        return *this;
    }

    /** Returns a new rectangle with the specified position.

        This method creates a new rectangle with the same size but a different position.

        @tparam T The type of the value for the new position.

        @param newPosition The new position for the rectangle as a Point.

        @return A new rectangle with the updated position.
    */
    template <class T>
    [[nodiscard]] constexpr Rectangle withPosition (const Point<T>& newPosition) const noexcept
    {
        static_assert (std::numeric_limits<ValueType>::max() >= std::numeric_limits<T>::max(), "Invalid narrow cast");

        return { newPosition.template to<ValueType>(), size };
    }

    /** Returns a new rectangle with the specified position coordinates.

        This method creates a new rectangle with the same size but different x and y coordinates.

        @tparam T The type of the value for the new x and y coordinates.

        @param x The new x-coordinate for the top-left corner.
        @param y The new y-coordinate for the top-left corner.

        @return A new rectangle with the updated position.
    */
    template <class T>
    [[nodiscard]] constexpr Rectangle withPosition (T x, T y) const noexcept
    {
        static_assert (std::numeric_limits<ValueType>::max() >= std::numeric_limits<T>::max(), "Invalid narrow cast");

        return { static_cast<ValueType> (x), static_cast<ValueType> (y), size };
    }

    /** Returns a new rectangle with its position set to (0, 0).

        This method creates a new rectangle with the same size but positioned at the origin.

        @return A new rectangle positioned at (0, 0).
    */
    [[nodiscard]] constexpr Rectangle withZeroPosition() const noexcept
    {
        return { 0, 0, size };
    }

    //==============================================================================
    /** Returns the position of the top-left corner of the rectangle.

        @return The position of the top-left corner as a Point.
    */
    [[nodiscard]] constexpr Point<ValueType> getTopLeft() const noexcept
    {
        return xy;
    }

    /** Returns the position of the top-right corner of the rectangle.

        @return The position of the top-right corner as a Point.
    */
    [[nodiscard]] constexpr Point<ValueType> getTopRight() const noexcept
    {
        return xy.translated (getWidth(), ValueType (0));
    }

    /** Returns the position of the bottom-left corner of the rectangle.

        @return The position of the bottom-left corner as a Point.
    */
    [[nodiscard]] constexpr Point<ValueType> getBottomLeft() const noexcept
    {
        return xy.translated (ValueType (0), getHeight());
    }

    /** Returns the position of the bottom-right corner of the rectangle.

        @return The position of the bottom-right corner as a Point.
    */
    [[nodiscard]] constexpr Point<ValueType> getBottomRight() const noexcept
    {
        return xy.translated (getWidth(), getHeight());
    }

    //==============================================================================
    /** Returns the size of the rectangle.

        @return The size of the rectangle as a Size object.
    */
    [[nodiscard]] constexpr Size<ValueType> getSize() const noexcept
    {
        return size;
    }

    /** Sets the size of the rectangle.

        @param newSize The new size for the rectangle as a Size object.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& setSize (const Size<ValueType>& newSize) noexcept
    {
        size = newSize;

        return *this;
    }

    /** Returns a new rectangle with the specified size.

        This method creates a new rectangle with the same position but a different size.

        @tparam T The type of the value for the new size.

        @param newSize The new size for the rectangle as a Size object.

        @return A new rectangle with the updated size.
    */
    template <class T>
    [[nodiscard]] constexpr Rectangle withSize (const Size<T>& newSize) const noexcept
    {
        static_assert (std::numeric_limits<ValueType>::max() >= std::numeric_limits<T>::max(), "Invalid narrow cast");

        return { xy, newSize.template to<ValueType>() };
    }

    /** Returns a new rectangle with the specified dimensions.

        This method creates a new rectangle with the same position but different width and height.

        @tparam T The type of the value for the new width and height.

        @param width The new width for the rectangle.
        @param height The new height for the rectangle.

        @return A new rectangle with the updated size.
    */
    template <class T>
    [[nodiscard]] constexpr Rectangle withSize (T width, T height) const noexcept
    {
        static_assert (std::numeric_limits<ValueType>::max() >= std::numeric_limits<T>::max(), "Invalid narrow cast");

        return { xy, static_cast<ValueType> (width), static_cast<ValueType> (height) };
    }

    /** Returns a new rectangle with its size scaled by a factor.

        This method creates a new rectangle with the same position but its width and height scaled by the specified factor.

        @tparam T The type of the scaling factor (must be a floating-point type).

        @param scaleFactor The factor by which to scale the size of the rectangle.

        @return A new rectangle with the size scaled.
    */
    template <class T>
    [[nodiscard]] constexpr auto withScaledSize (T scaleFactor) const noexcept
        -> std::enable_if_t<std::is_floating_point_v<T>, Rectangle>
    {
        return withSize (size * scaleFactor);
    }

    /** Returns a new rectangle with its size set to zero.

        This method creates a new rectangle with the same position but zero width and height.

        @return A new rectangle with zero size.
    */
    [[nodiscard]] constexpr Rectangle withZeroSize() const noexcept
    {
        return { xy, 0, 0 };
    }

    //==============================================================================
    /** Sets the bounds of the rectangle.

        @param x The new x-coordinate for the top-left corner.
        @param y The new y-coordinate for the top-left corner.
        @param width The new width of the rectangle.
        @param height The new height of the rectangle.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& setBounds (ValueType x, ValueType y, ValueType width, ValueType height) noexcept
    {
        xy = { x, y };
        size = { width, height };

        return *this;
    }

    //==============================================================================
    /** Returns the center point of the rectangle.

        @return The center of the rectangle as a Point, calculated as the midpoint between the top-left and bottom-right corners.
    */
    [[nodiscard]] constexpr Point<ValueType> getCenter() const noexcept
    {
        return { xy.getX() + size.getWidth() / 2.0f, xy.getY() + size.getHeight() / 2.0f };
    }

    /** Sets the center of the rectangle to the specified coordinates.

        This method adjusts the position of the rectangle so that its center matches the given coordinates.

        @param centerX The x-coordinate of the new center.
        @param centerY The y-coordinate of the new center.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& setCenter (ValueType centerX, ValueType centerY) noexcept
    {
        xy = { centerX - size.getWidth() / static_cast<ValueType> (2), centerY - size.getHeight() / static_cast<ValueType> (2) };

        return *this;
    }

    /** Sets the center of the rectangle to the specified point.

        This method adjusts the position of the rectangle so that its center matches the given point.

        @param center The new center as a Point.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& setCenter (const Point<ValueType>& center) noexcept
    {
        setCenter (center.getX(), center.getY());

        return *this;
    }

    /** Returns a new rectangle with its center set to the specified coordinates.

        This method creates a new rectangle with the same size but its position adjusted so that its center matches the given coordinates.

        @param centerX The x-coordinate of the new center.
        @param centerY The y-coordinate of the new center.

        @return A new rectangle with the updated center.
    */
    [[nodiscard]] constexpr Rectangle withCenter (ValueType centerX, ValueType centerY) noexcept
    {
        Rectangle result = *this;
        result.setCenter (centerX, centerY);
        return result;
    }

    /** Returns a new rectangle with its center set to the specified point.

        This method creates a new rectangle with the same size but its position adjusted so that its center matches the given point.

        @param center The new center as a Point.

        @return A new rectangle with the updated center.
    */
    [[nodiscard]] constexpr Rectangle withCenter (const Point<ValueType>& center) noexcept
    {
        Rectangle result = *this;
        result.setCenter (center);
        return result;
    }

    //==============================================================================
    /** Checks if the rectangle's size is zero.

        @return True if both the width and height are zero, otherwise false.
    */
    [[nodiscard]] constexpr bool isEmpty() const noexcept
    {
        return size.isZero();
    }

    /** Checks if the rectangle's size is zero (i.e., it is a point).

        @return True if both the width and height are zero, otherwise false.
    */
    [[nodiscard]] constexpr bool isPoint() const noexcept
    {
        return size.isZero();
    }

    /** Checks if the rectangle is a line (either horizontal or vertical).

        @return True if the rectangle is strictly horizontal or vertical (i.e., width or height is zero but not both), otherwise false.
    */
    [[nodiscard]] constexpr bool isLine() const noexcept
    {
        return isVerticalLine() || isHorizontalLine();
    }

    /** Checks if the rectangle is a vertical line.

        @return True if the height is non-zero and the width is zero, otherwise false.
    */
    [[nodiscard]] constexpr bool isVerticalLine() const noexcept
    {
        return size.isHorizontallyEmpty();
    }

    /** Checks if the rectangle is a horizontal line.

        @return True if the width is non-zero and the height is zero, otherwise false.
    */
    [[nodiscard]] constexpr bool isHorizontalLine() const noexcept
    {
        return size.isVerticallyEmpty();
    }

    //==============================================================================
    /** Returns the line representing the left side of the rectangle.

        @return A Line object representing the left vertical edge of the rectangle.
    */
    [[nodiscard]] constexpr Line<ValueType> leftSide() const noexcept
    {
        return { xy, xy.translated (ValueType (0), getHeight()) };
    }

    /** Returns the line representing the top side of the rectangle.

        @return A Line object representing the top horizontal edge of the rectangle.
    */
    [[nodiscard]] constexpr Line<ValueType> topSide() const noexcept
    {
        return { xy, xy.translated (getWidth(), ValueType (0)) };
    }

    /** Returns the line representing the right side of the rectangle.

        @return A Line object representing the right vertical edge of the rectangle.
    */
    [[nodiscard]] constexpr Line<ValueType> rightSide() const noexcept
    {
        return { xy.translated (getWidth(), ValueType (0)), xy.translated (getWidth(), getHeight()) };
    }

    /** Returns the line representing the bottom side of the rectangle.

        @return A Line object representing the bottom horizontal edge of the rectangle.
    */
    [[nodiscard]] constexpr Line<ValueType> bottomSide() const noexcept
    {
        return { xy.translated (ValueType (0), getHeight()), xy.translated (getWidth(), getHeight()) };
    }

    /** Returns the diagonal line from the top-left to the bottom-right corner of the rectangle.

        @return A Line object representing the diagonal crossing the rectangle from top-left to bottom-right.
    */
    [[nodiscard]] constexpr Line<ValueType> diagonalTopToBottom() const noexcept
    {
        return { xy, xy.translated (getWidth(), getHeight()) };
    }

    /** Returns the diagonal line from the bottom-left to the top-right corner of the rectangle.

        @return A Line object representing the diagonal crossing the rectangle from bottom-left to top-right.
    */
    [[nodiscard]] constexpr Line<ValueType> diagonalBottomToTop() const noexcept
    {
        return { xy.translated (ValueType (0), getHeight()), xy.translated (getWidth(), ValueType (0)) };
    }

    //==============================================================================
    /** Translates the rectangle by the specified x and y offsets.

        This method adjusts the position of the rectangle by adding the specified deltas to the x and y coordinates.

        @param deltaX The amount to add to the x-coordinate.
        @param deltaY The amount to add to the y-coordinate.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& translate (ValueType deltaX, ValueType deltaY) noexcept
    {
        xy.translate (deltaX, deltaY);
        return *this;
    }

    /** Translates the rectangle by the specified point.

        This method adjusts the position of the rectangle by adding the point's x and y values to the rectangle's position.

        @param delta The point containing the amounts to add to the rectangle's position.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& translate (const Point<ValueType>& delta) noexcept
    {
        xy.translate (delta);
        return *this;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle<ValueType> translated (ValueType deltaX, ValueType deltaY) const noexcept
    {
        return { xy.translated (deltaX, deltaY), size };
    }

    /** Returns a new rectangle translated by the specified point.

        This method creates a new rectangle with the same size but its position adjusted by the point's x and y values.

        @param delta The point containing the amounts to add to the rectangle's position.

        @return A new rectangle with the updated position.
    */
    [[nodiscard]] constexpr Rectangle<ValueType> translated (const Point<ValueType>& delta) const noexcept
    {
        return { xy.translated (delta), size };
    }

    //==============================================================================
    /** Scales the size of the rectangle uniformly by a given factor.

        This method multiplies both the x, y, width and height of the rectangle by the specified factor.

        @param factor The scaling factor to apply to both the x, y, width and height of the rectangle.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& scale (float factor) noexcept
    {
        xy.scale (factor);
        size.scale (factor);
        return *this;
    }

    /** Scales the size of the rectangle by the specified x and y factors.

        This method adjusts the size of the rectangle by multiplying its x, width and y, height by the given factors.

        @param factorX The factor to multiply the x and width by.
        @param factorY The factor to multiply the y and height by.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& scale (float factorX, float factorY) noexcept
    {
        xy.scale (factorX, factorY);
        size.scale (factorX, factorY);
        return *this;
    }

    /** Returns a new rectangle with its size scaled uniformly by the specified factor.

        This method creates a new rectangle with the same position but with both the x, width and y, height scaled by the given factor.

        @param factor The scaling factor to apply to both the x, width and y, height of the rectangle.

        @return A new rectangle with the scaled size.
    */
    [[nodiscard]] constexpr Rectangle scaled (float factor) const noexcept
    {
        return { xy.scaled (factor), size.scaled (factor) };
    }

    /** Returns a new rectangle with its size scaled by the specified x and y factors.

        This method creates a new rectangle with the same position but its x, width and y, height adjusted by the given factors.

        @param factorX The factor to multiply the x and width by.
        @param factorY The factor to multiply the y and height by.
        @return A new rectangle with the scaled size.
    */
    [[nodiscard]] constexpr Rectangle scaled (float factorX, float factorY) const noexcept
    {
        return { xy.scaled (factorX, factorY), size.scaled (factorX, factorY) };
    }

    //==============================================================================
    /** Removes a specified height from the top of the rectangle, adjusting its size and position.

        This method reduces the height of the rectangle from the top by the specified amount, moving the top edge downwards.

        @param delta The height to remove from the top of the rectangle.

        @return A new rectangle representing the portion removed from the top.
    */
    constexpr Rectangle removeFromTop (ValueType delta) noexcept
    {
        const Rectangle result { xy, size.withHeight (jmax (ValueType (0), delta)) };

        xy = xy.withY (xy.getY() + delta);
        size = size.withHeight (jmax (ValueType (0), size.getHeight() - delta));

        return result;
    }

    /** Removes a specified width from the left side of the rectangle, adjusting its size and position.

        This method reduces the width of the rectangle from the left by the specified amount, moving the left edge to the right.

        @param delta The width to remove from the left of the rectangle.

        @return A new rectangle representing the portion removed from the left.
    */
    constexpr Rectangle removeFromLeft (ValueType delta) noexcept
    {
        const Rectangle result { xy, size.withWidth (jmax (ValueType (0), delta)) };

        xy = xy.withX (xy.getX() + delta);
        size = size.withWidth (jmax (ValueType (0), size.getWidth() - delta));

        return result;
    }

    /** Removes a specified height from the bottom of the rectangle, adjusting its size and position.

        This method reduces the height of the rectangle from the bottom by the specified amount, moving the bottom edge upwards.

        @param delta The height to remove from the bottom of the rectangle.

        @return A new rectangle representing the portion removed from the bottom.
    */
    constexpr Rectangle removeFromBottom (ValueType delta) noexcept
    {
        const Rectangle result { xy.withY (jmax (ValueType (0), xy.getY() + size.getHeight() - delta)), size.withHeight (jmax (ValueType (0), delta)) };

        size = size.withHeight (jmax (ValueType (0), size.getHeight() - delta));

        return result;
    }

    /** Removes a specified width from the right side of the rectangle, adjusting its size and position.

        This method reduces the width of the rectangle from the right by the specified amount, moving the right edge to the left.

        @param delta The width to remove from the right of the rectangle.

        @return A new rectangle representing the portion removed from the right.
    */
    constexpr Rectangle removeFromRight (ValueType delta) noexcept
    {
        const Rectangle result { xy.withX (jmax (ValueType (0), xy.getX() + size.getWidth() - delta)),
                                 size.withWidth (jmax (ValueType (0), delta)) };

        size = size.withWidth (jmax (ValueType (0), size.getWidth() - delta));

        return result;
    }

    //==============================================================================
    /** Reduces the size of the rectangle by a uniform amount on all sides.

        This method shrinks the rectangle's width and height by the specified delta, equally reducing the size from all edges.

        @param delta The amount to reduce each side by.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& reduce (ValueType delta) noexcept
    {
        xy = { xy.getX() + delta, xy.getY() + delta };
        size = { jmax (ValueType (0), size.getWidth() - ValueType (2) * delta),
                 jmax (ValueType (0), size.getHeight() - ValueType (2) * delta) };

        return *this;
    }

    /** Reduces the width and height of the rectangle by different amounts on all sides.

        This method shrinks the rectangle's width by deltaX and height by deltaY, reducing the size from all edges by these respective amounts.

        @param deltaX The amount to reduce the width by on each side.
        @param deltaY The amount to reduce the height by on each side.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& reduce (ValueType deltaX, ValueType deltaY) noexcept
    {
        xy = { xy.getX() + deltaX, xy.getY() + deltaY };
        size = { jmax (ValueType (0), size.getWidth() - ValueType (2) * deltaX),
                 jmax (ValueType (0), size.getHeight() - ValueType (2) * deltaY) };

        return *this;
    }

    // TODO - doxygen
    constexpr Rectangle& reduce (ValueType left, ValueType top, ValueType right, ValueType bottom) noexcept
    {
        xy = { xy.getX() + left, xy.getY() + top };
        size = { jmax (ValueType (0), size.getWidth() - (left + right)),
                 jmax (ValueType (0), size.getHeight() - (top + bottom)) };

        return *this;
    }

    /** Returns a new rectangle with its size reduced by a uniform amount on all sides.

        This method creates a new rectangle with the same position but its width and height shrunk by the specified delta, equally from all edges.

        @param delta The amount to reduce each side by.

        @return A new rectangle with the reduced size.
    */
    [[nodiscard]] constexpr Rectangle reduced (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.reduce (delta);
        return result;
    }

    /** Returns a new rectangle with its width and height reduced by different amounts on all sides.

        This method creates a new rectangle with the same position but its width and height shrunk by the specified deltaX and deltaY, respectively.

        @param deltaX The amount to reduce the width by on each side.
        @param deltaY The amount to reduce the height by on each side.

        @return A new rectangle with the reduced size.
    */
    [[nodiscard]] constexpr Rectangle reduced (ValueType deltaX, ValueType deltaY) const noexcept
    {
        Rectangle result = *this;
        result.reduce (deltaX, deltaY);
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle reduced (ValueType left, ValueType top, ValueType right, ValueType bottom) const noexcept
    {
        Rectangle result = *this;
        result.reduce (left, top, right, bottom);
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle reducedLeft (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.reduce (delta, ValueType (0), ValueType (0), ValueType (0));
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle reducedTop (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.reduce (ValueType (0), delta, ValueType (0), ValueType (0));
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle reducedRight (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.reduce (ValueType (0), ValueType (0), delta, ValueType (0));
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle reducedBottom (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.reduce (ValueType (0), ValueType (0), ValueType (0), delta);
        return result;
    }

    //==============================================================================
    /** Enlarges the size of the rectangle by a uniform amount on all sides.

        This method expand the rectangle's width and height by the specified delta, equally increasing the size on all edges.

        @param delta The amount to enlarge each side by.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& enlarge (ValueType delta) noexcept
    {
        xy = { xy.getX() - delta, xy.getY() - delta };
        size = { jmax (ValueType (0), size.getWidth() + ValueType (2) * delta),
                 jmax (ValueType (0), size.getHeight() + ValueType (2) * delta) };

        return *this;
    }

    /** Enlarges the width and height of the rectangle by different amounts on all sides.

        This method expand the rectangle's width by deltaX and height by deltaY, increasing the size on all edges by these respective amounts.

        @param deltaX The amount to enlarge the width by on each side.
        @param deltaY The amount to enlarge the height by on each side.

        @return A reference to this rectangle to allow method chaining.
    */
    constexpr Rectangle& enlarge (ValueType deltaX, ValueType deltaY) noexcept
    {
        xy = { xy.getX() - deltaX, xy.getY() - deltaY };
        size = { jmax (ValueType (0), size.getWidth() + ValueType (2) * deltaX),
                 jmax (ValueType (0), size.getHeight() + ValueType (2) * deltaY) };

        return *this;
    }

    // TODO - doxygen
    constexpr Rectangle& enlarge (ValueType left, ValueType top, ValueType right, ValueType bottom) noexcept
    {
        xy = { xy.getX() - left, xy.getY() - top };
        size = { jmax (ValueType (0), size.getWidth() + (left + right)),
                 jmax (ValueType (0), size.getHeight() + (top + bottom)) };

        return *this;
    }

    /** Returns a new rectangle with its size reduced by a uniform amount on all sides.

        This method creates a new rectangle with the same position but its width and height shrunk by the specified delta, equally from all edges.

        @param delta The amount to reduce each side by.

        @return A new rectangle with the reduced size.
    */
    [[nodiscard]] constexpr Rectangle enlarged (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.enlarge (delta);
        return result;
    }

    /** Returns a new rectangle with its width and height reduced by different amounts on all sides.

        This method creates a new rectangle with the same position but its width and height shrunk by the specified deltaX and deltaY, respectively.

        @param deltaX The amount to reduce the width by on each side.
        @param deltaY The amount to reduce the height by on each side.

        @return A new rectangle with the reduced size.
    */
    [[nodiscard]] constexpr Rectangle enlarged (ValueType deltaX, ValueType deltaY) const noexcept
    {
        Rectangle result = *this;
        result.enlarge (deltaX, deltaY);
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle enlargedLeft (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.enlarge (delta, ValueType (0), ValueType (0), ValueType (0));
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle enlargedTop (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.enlarge (ValueType (0), delta, ValueType (0), ValueType (0));
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle enlargedRight (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.enlarge (ValueType (0), ValueType (0), delta, ValueType (0));
        return result;
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle enlargedBottom (ValueType delta) const noexcept
    {
        Rectangle result = *this;
        result.enlarge (ValueType (0), ValueType (0), ValueType (0), delta);
        return result;
    }

    //==============================================================================
    /** Checks if the specified point lies within the bounds of the rectangle.

        @param x The x-coordinate of the point to check.
        @param y The y-coordinate of the point to check.

        @return True if the point is within the rectangle, otherwise false.
    */
    [[nodiscard]] constexpr bool contains (ValueType x, ValueType y) const noexcept
    {
        return x >= xy.getX()
            && y >= xy.getY()
            && x <= (xy.getX() + size.getWidth())
            && y <= (xy.getY() + size.getHeight());
    }

    /** Checks if the specified point lies within the bounds of the rectangle.

        @param p The point to check.

        @return True if the point is within the rectangle, otherwise false.
    */
    [[nodiscard]] constexpr bool contains (const Point<ValueType>& p) const noexcept
    {
        return contains (p.getX(), p.getY());
    }

    //==============================================================================
    /** Calculates the area of the rectangle.

        @return The area of the rectangle, computed as width multiplied by height.
    */
    [[nodiscard]] constexpr ValueType area() const noexcept
    {
        return size.area();
    }

    //==============================================================================
    /** Checks if this rectangle intersects with another rectangle.

        This method checks for overlap between this rectangle and another, returning true if there is any overlap.

        @param other The other rectangle to check against.

        @return True if the rectangles intersect, otherwise false.
    */
    [[nodiscard]] constexpr bool intersects (const Rectangle& other) const noexcept
    {
        const auto bottomRight = getBottomRight();
        const auto otherBottomRight = other.getBottomRight();

        return ! (getX() > otherBottomRight.getX() || bottomRight.getX() < other.getX() || getY() > otherBottomRight.getY() || bottomRight.getY() < other.getY());
    }

    // TODO - doxygen
    [[nodiscard]] constexpr Rectangle intersection (const Rectangle& other) const noexcept
    {
        const auto x1 = jmax (getX(), other.getX());
        const auto x2 = jmin (getX() + getWidth(), other.getX() + other.getWidth());

        if (x1 < x2)
        {
            const auto y1 = jmax (getY(), other.getY());
            const auto y2 = jmin (getY() + getHeight(), other.getY() + other.getHeight());

            if (y1 < y2)
                return { x1, y1, x2 - x1, y2 - y1 };
        }

        return {};
    }

    //==============================================================================
    /** Returns the largest square that fits within the rectangle.

        This method calculates and returns the largest square that can be inscribed within the rectangle.

        @return A Rectangle representing the largest square that fits inside this rectangle.
    */
    [[nodiscard]] constexpr Rectangle largestFittingSquare() const noexcept
    {
        if (getWidth() == getHeight())
            return *this;

        if (getWidth() > getHeight())
        {
            const auto newPosX = static_cast<ValueType> ((getWidth() - getHeight()) / 2.0f);
            return { xy.getX() + newPosX, xy.getY(), getHeight(), getHeight() };
        }
        else
        {
            const auto newPosY = static_cast<ValueType> ((getHeight() - getWidth()) / 2.0f);
            return { xy.getX(), xy.getY() + newPosY, getWidth(), getWidth() };
        }
    }

    //==============================================================================
    /** Returns the smallest containing rectangle between this and another rectangle.

        @return A Rectangle representing the smallest containing rectangle of two rectangles.
    */
    [[nodiscard]] constexpr Rectangle smallestContainingRectangle (const Rectangle& other) const noexcept
    {
        const auto x1 = jmin (getX(), other.getX());
        const auto x2 = jmax (getX() + getWidth(), other.getX() + other.getWidth());

        if (x1 < x2)
        {
            const auto y1 = jmin (getY(), other.getY());
            const auto y2 = jmax (getY() + getHeight(), other.getY() + other.getHeight());

            if (y1 < y2)
                return { x1, y1, x2 - x1, y2 - y1 };
        }

        return {};
    }

    //==============================================================================
    // TODO - doxygen
    [[nodiscard]] Rectangle& transform (const AffineTransform& t) noexcept
    {
        auto x1 = static_cast<float> (getX());
        auto y1 = static_cast<float> (getY());
        auto x2 = static_cast<float> (getX() + getWidth());
        auto y2 = static_cast<float> (getY() + getHeight());

        t.transformPoints (x1, y1, x2, y2);

        auto rx1 = jmin (x1, x2);
        auto rx2 = jmax (x1, x2);
        auto ry1 = jmin (y1, y2);
        auto ry2 = jmax (y1, y2);

        xy = xy
                 .withX (static_cast<ValueType> (rx1))
                 .withY (static_cast<ValueType> (ry1));

        size = size
                   .withWidth (static_cast<ValueType> (rx2 - rx1))
                   .withHeight (static_cast<ValueType> (ry2 - ry1));

        return *this;
    }

    // TODO - doxygen
    [[nodiscard]] Rectangle transformed (const AffineTransform& t) const noexcept
    {
        Rectangle result (*this);
        result.transform (t);
        return result;
    }

    //==============================================================================
    /** Converts the rectangle to a different type.

        This method template converts the rectangle's position and size to another type, and returns a new rectangle of that type.

        @tparam T The type to convert the rectangle to.

        @return A new rectangle of type T, with the position and size converted.
    */
    template <class T>
    [[nodiscard]] constexpr Rectangle<T> to() const noexcept
    {
        return { xy.template to<T>(), size.template to<T>() };
    }

    //==============================================================================
    /** Multiplies the size and position of the rectangle by a scale factor.

        This operator returns a new rectangle with both the position and size scaled by the given factor.

        @tparam T The type of the scaling factor (must be a floating-point type).

        @param scaleFactor The factor by which to scale the rectangle.

        @return A new rectangle scaled by the factor.
    */
    template <class T>
    constexpr auto operator* (T scaleFactor) const noexcept
        -> std::enable_if_t<std::is_floating_point_v<T>, Rectangle>
    {
        Rectangle r (*this);
        r *= scaleFactor;
        return r;
    }

    /** Scales the size and position of the rectangle in place by a scale factor.

        This operator modifies this rectangle, scaling both the position and size by the given factor.

        @tparam T The type of the scaling factor (must be a floating-point type).

        @param scaleFactor The factor by which to scale the rectangle.

        @return A reference to this rectangle to allow method chaining.
    */
    template <class T>
    constexpr auto operator*= (T scaleFactor) noexcept
        -> std::enable_if_t<std::is_floating_point_v<T>, Rectangle&>
    {
        xy = { static_cast<ValueType> (xy.getX() * scaleFactor), static_cast<ValueType> (xy.getY() * scaleFactor) };
        size = { static_cast<ValueType> (size.getWidth() * scaleFactor), static_cast<ValueType> (size.getHeight() * scaleFactor) };
        return *this;
    }

    /** Divides the size and position of the rectangle by a scale factor.

        This operator returns a new rectangle with both the position and size divided by the given factor.

        @tparam T The type of the scaling factor (must be a floating-point type).

        @param scaleFactor The factor by which to divide the rectangle.

        @return A new rectangle divided by the factor.
    */
    template <class T>
    constexpr auto operator/ (T scaleFactor) const noexcept
        -> std::enable_if_t<std::is_floating_point_v<T>, Rectangle>
    {
        Rectangle r (*this);
        r /= scaleFactor;
        return r;
    }

    /** Divides the size and position of the rectangle in place by a scale factor.

        This operator modifies this rectangle, dividing both the position and size by the given factor.

        @tparam T The type of the scaling factor (must be a floating-point type).

        @param scaleFactor The factor by which to divide the rectangle.

        @return A reference to this rectangle to allow method chaining.
    */
    template <class T>
    constexpr auto operator/= (T scaleFactor) noexcept
        -> std::enable_if_t<std::is_floating_point_v<T>, Rectangle&>
    {
        xy = { static_cast<ValueType> (xy.getX() / scaleFactor), static_cast<ValueType> (xy.getY() / scaleFactor) };
        size = { static_cast<ValueType> (size.getWidth() / scaleFactor), static_cast<ValueType> (size.getHeight() / scaleFactor) };
        return *this;
    }

    //==============================================================================
    /** Checks for equality between this rectangle and another rectangle.

        This operator returns true if both the position and size of this rectangle are equal to those of the other rectangle.

        @param other The other rectangle to compare against.

        @return True if the rectangles are equal, otherwise false.
    */
    constexpr bool operator== (const Rectangle& other) const noexcept
    {
        return xy == other.xy && size == other.size;
    }

    /** Checks for inequality between this rectangle and another rectangle.

        This operator returns true if either the position or size of this rectangle differs from those of the other rectangle.

        @param other The other rectangle to compare against.

        @return True if the rectangles are not equal, otherwise false.
    */
    constexpr bool operator!= (const Rectangle& other) const noexcept
    {
        return ! (*this == other);
    }

private:
    Point<ValueType> xy;
    Size<ValueType> size;
};

/** Stream insertion operator for rectangles.

    This operator allows for rectangles to be easily printed to a stream. It outputs the rectangle's position and size in the format "x, y, width, height".

    @tparam ValueType The type of the values stored in the rectangle.

    @param string1 The stream to which the rectangle will be inserted.
    @param r The rectangle to be inserted into the stream.

    @return A reference to the modified stream.
*/
template <class ValueType>
JUCE_API String& JUCE_CALLTYPE operator<< (String& string1, const Rectangle<ValueType>& r)
{
    auto [x, y, w, h] = r;

    string1 << x << ", " << y << ", " << w << ", " << h;

    return string1;
}

template <std::size_t I, class ValueType>
constexpr ValueType get (const Rectangle<ValueType>& point) noexcept
{
    if constexpr (I == 0)
        return point.getX();
    else if constexpr (I == 1)
        return point.getY();
    else if constexpr (I == 2)
        return point.getWidth();
    else if constexpr (I == 3)
        return point.getHeight();
    else
        static_assert (dependentFalse<I>);
}

} // namespace yup

namespace std
{

template <class ValueType>
struct tuple_size<yup::Rectangle<ValueType>>
{
    inline static constexpr std::size_t value = 4;
};

template <std::size_t I, class ValueType>
struct tuple_element<I, yup::Rectangle<ValueType>>
{
    using type = ValueType;
};

} // namespace std
