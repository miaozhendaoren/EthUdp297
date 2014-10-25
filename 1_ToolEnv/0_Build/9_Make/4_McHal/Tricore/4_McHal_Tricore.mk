###############################################################################
#                                                                             #
#        Copyright © 2011 Infineon Technologies AG. All rights reserved.      #
#                                                                             #
#                                                                             #
#                              IMPORTANT NOTICE                               #
#                                                                             #
#                                                                             #
# Infineon Technologies AG (Infineon) is supplying this file for use          #
# exclusively with Infineon’s microcontroller products. This file can be      #
# freely distributed within development tools that are supporting such        #
# microcontroller products.                                                   #
#                                                                             #
# THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED #
# OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF          #
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.#
# INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,#
# OR CONSEQUENTIAL DAMAGES, FOR	ANY REASON WHATSOEVER.                        #
#                                                                             #
###############################################################################
# Subdirectory make file for 0_Src/4_McHal/Tricore
# This is for core type "MAIN" and Gnuc toolchain !
###############################################################################


include 1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/Cpu/4_McHal_Cpu.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/Dma/4_McHal_Dma.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/Eth/4_McHal_Eth.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/Port/4_McHal_Port.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/Scu/4_McHal_Scu.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/Src/4_McHal_Src.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/Stm/4_McHal_Stm.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/_Impl/4_McHal__Impl.mk \
		1_ToolEnv/0_Build/9_Make/4_McHal/Tricore/_PinMap/4_McHal__PinMap.mk


