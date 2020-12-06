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
 * @file touchgfx/widgets/canvas/AbstractPainterARGB8888.hpp
 *
 * Declares the touchgfx::AbstractPainterARGB8888 class.
 */
#ifndef ABSTRACTPAINTERARGB8888_HPP
#define ABSTRACTPAINTERARGB8888_HPP

#include <assert.h>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
/**
 * The AbstractPainterARGB8888 class is an abstract class for creating a painter to draw on a
 * ARGB8888 display using CanvasWidgetRenderer.
 *
 * @see AbstractPainter
 */
class AbstractPainterARGB8888 : public AbstractPainter
{
public:
    AbstractPainterARGB8888()
        : AbstractPainter(), currentX(0), currentY(0)
    {
        assert(compatibleFramebuffer(Bitmap::ARGB8888) && "The chosen painter only works with ARGB8888 displays");
    }

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    /**
     * @copydoc AbstractPainterABGR2222::renderInit()
     */
    virtual bool renderInit()
    {
        return true;
    }

    /**
     * @copydoc AbstractPainterABGR2222::renderNext(uint8_t&,uint8_t&,uint8_t&,uint8_t&)
     */
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) = 0;

    /**
     * @copydoc AbstractPainterABGR2222::renderPixel(uint8_t*,uint8_t,uint8_t,uint8_t)
     *
     * @note Will set the alpha value to 255 (solid)
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @copydoc AbstractPainterABGR2222::renderPixel(uint8_t*,uint8_t,uint8_t,uint8_t)
     *
     * @param          alpha The alpha.
     *
     * @note The \a alpha value is written to the 32bit framebuffer, just like the color is.
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
};

} // namespace touchgfx

#endif // ABSTRACTPAINTERARGB8888_HPP
