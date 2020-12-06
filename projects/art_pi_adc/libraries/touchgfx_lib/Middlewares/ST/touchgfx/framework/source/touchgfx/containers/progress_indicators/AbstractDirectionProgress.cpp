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

#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>

namespace touchgfx
{
AbstractDirectionProgress::AbstractDirectionProgress()
    : AbstractProgressIndicator()
{
    AbstractDirectionProgress::setDirection(RIGHT);
}

void AbstractDirectionProgress::setDirection(DirectionType direction)
{
    progressDirection = direction;
    progressIndicatorContainer.invalidate();
    setValue(getValue());
}

touchgfx::AbstractDirectionProgress::DirectionType AbstractDirectionProgress::getDirection() const
{
    return progressDirection;
}
} // namespace touchgfx
