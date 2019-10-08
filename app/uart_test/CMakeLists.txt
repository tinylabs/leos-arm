#
# cmake for uart_test
#
include( ./auto.cmake )
leos_app( uart_test
  # Add sources here
  uart_test.cpp
  )

# Add to filesystem
leos_fs( uart_test /bin )
