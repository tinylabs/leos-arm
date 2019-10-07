#
# cmake for mps2-cm3
#

# Define target
leos_target( mps2-cm3 )

# Add automatic dependencies
include( ./auto.cmake )

# Add apps/etc here...
#leos_add( app/xyz )

# Include board config files
leos_include( pins.map /cfg )

# Create binary image
leos_image( mps2-cm3
  leos "RAM=2Q RSZ=1M IRQ=0" )
