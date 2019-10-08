#
# cmake for mps2-cm3
#

# Define target
leos_target( mps2-cm3 )

# Add automatic dependencies
include( ./auto.cmake )

# Add apps/etc here...
leos_add( app/uart_test )

# Include board config files
leos_fs( pin.map /cfg )

# Create binary image
leos_image(
  mps2-cm3
  leos
  "EXEC=/bin/uart_test"
  )

# Setup debug targets
leos_qemu_target( mps2-cm3 )
leos_jlink_target( mps2-cm3 )
