include( ./auto.cmake )
leos_obj( cmsdk_sysctl  cmsdk_sysctl.cpp )
leos_fs( cmsdk_sysctl /drv )
leos_clock_tree( cmsdk_sysctl mps2_clock /cfg )
