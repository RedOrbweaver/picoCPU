#pragma once
#include <stdio.h>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include <memory>
#include <optional>
#include <deque>
#include <random>

using std::vector;
using std::deque;
using std::shared_ptr;
using std::unique_ptr;
using std::optional;

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

#include "pico/multicore.h"
#include "pico/platform.h"
#include "pico/mutex.h"

#include "redpicolib/RedPicoLib.hpp"

#include "../picoGPU/shared.hpp"

#include "dac_out.pio.h"

#include "globals.hpp"

#include "exceptions.hpp"

#include "Audio.hpp"

#include "GPU.hpp"

#include "EntityManager.hpp"
#include "MemoryManager.hpp"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "GeometryManager.hpp"

#include "Objects/Object.hpp"
#include "Objects/Geometric.hpp"
#include "Objects/Shape.hpp"
#include "Objects/Circle.hpp"
#include "Objects/Rectangle.hpp"
#include "Objects/Triangle.hpp"
#include "Objects/MultiTriangle.hpp"
#include "Objects/EmptyCircle.hpp"
#include "Objects/EmptyRectangle.hpp"
#include "Objects/Line.hpp"
#include "Objects/Text.hpp"
#include "Objects/MultiLine.hpp"
#include "Objects/MultiPoint.hpp"
#include "Objects/Bezier.hpp"

#include "Programs/Program.hpp"
#include "Programs/MainMenu.hpp"

#include "tests.hpp"

