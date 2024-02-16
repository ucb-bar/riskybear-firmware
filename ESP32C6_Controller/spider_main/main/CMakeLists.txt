# Edit following two lines to set component requirements (see docs)
idf_component_register(SRCS 
    "main.c"
    "motor.c"
    "motor.h"
    "encoder.c"
    "encoder.h"
    "switch.c"
    "switch.h"
    "i2c.h"
    "i2c.c"
    INCLUDE_DIRS "."
    REQUIRES rotary_encoder esp_timer)

set(COMPONENT_ADD_INCLUDEDIRS "")
