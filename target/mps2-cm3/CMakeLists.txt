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
  "EXEC=/bin/uart_test OSCCLK0=50m OSCCLK1=24576k OSCCLK2=25m"
  )

# Setup debug targets
#leos_qemu_target( mps2-cm3 )
leos_qemu_target( mps2-cm3 "-serial tcp:localhost:1234" )
leos_jlink_target( mps2-cm3 )
# Create target to run
#leos_qemu_run( mps2-cm3 "-serial tcp:localhost:1234 -serial stdio" )
#leos_qemu_run( mps2-cm3 "-chardev stdio,path=./log.txt,id=uart0 -serial chardev:uart0  -trace enable=cmsdk_apb_uart_*" )
#leos_qemu_run( mps2-cm3 "-serial mon:stdio -trace enable=cmsdk_apb_uart_set_params" )
#leos_qemu_run( mps2-cm3 "-serial /dev/pts/5 -trace enable=cmsdk_apb_uart_set_params" )
#leos_qemu_run( mps2-cm3 "-serial stdio -trace events=/home/elliot/projects/leos-arm/build/events.txt" )
#leos_qemu_run( mps2-cm3 "-serial pty -trace events=/home/elliot/projects/leos-arm/build/events.txt" )
leos_qemu_run( mps2-cm3 "-serial stdio" )
