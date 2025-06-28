#pragma once

#include "hmain.hpp"

DEFINE_EXCEPTION(I2CReadFailureException);
DEFINE_EXCEPTION(I2CWriteFailureException);
DEFINE_EXCEPTION(InvalidTGAFileException);
DEFINE_EXCEPTION(UnsupportedTGAFileException);