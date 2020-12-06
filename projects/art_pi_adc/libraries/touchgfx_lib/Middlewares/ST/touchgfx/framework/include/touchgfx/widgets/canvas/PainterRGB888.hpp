/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.15.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/**
 * @file touchgfx/widgets/canvas/PainterRGB888.hpp
 *
 * Declares the touchgfx::PainterRGB888 class.
 */
#ifndef PAINTERRGB888_HPP
#define PAINTERRGB888_HPP

#include <stdint.h>
#include <touchgfx/Color.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterRGB888.hpp>

namespace touchgfx
{
/**
 * The PainterRGB888 class allows a shape to be filled with a given color and alpha
 * value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterRGB888 : public AbstractPainterRGB888
{
public:
    /**
     * Initializes a new instance of the PainterRGB888 class.
     *
     * @param  color (Optional) the color, default is black.
     * @param  alpha (Optional) the alpha, default is 255 i.e. solid.
     */
    PainterRGB888(colortype color = 0, uint8_t alpha = 255)
        : AbstractPainterRGB888()
    {
        setColor(color);
        setAlpha(alpha);
    }

    /**
     * Sets color and alpha to use when drawing the CanvasWidget.
     *
     * @param  color The color.
     */
    void setColor(colortype color)
    {
        painterRed = Color::getRedColor(color);
        painterGreen = Color::getGreenColor(color);
        painterBlue = Color::getBlueColor(color);
    }

    /**
     * Gets the current color.
     *
     * @return The color.
     */
    colortype getColor() const
    {
        return Color::getColorFrom24BitRGB(painterRed, painterGreen, painterBlue);
    }

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha);

    uint8_t painterRed;   ///< The red part of the color
    uint8_t painterGreen; ///< The green part of the color
    uint8_t painterBlue;  ///< The blue part of the color
};

} // namespace touchgfx

#endif // PAINTERRGB888_HPP
