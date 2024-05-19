# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "libraries/UART_TSI.c" "libraries/switch.c" "libraries/i2c.c" "main.c" "joints.c" "home.c" "controller.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
