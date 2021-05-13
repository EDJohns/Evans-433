#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../../../Downloads/123/ST7789.c ../../../../../../Downloads/123/i2c_master_noint.c ../../../../../../Downloads/123/spi.c main11v2.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1807030798/ST7789.o ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o ${OBJECTDIR}/_ext/1807030798/spi.o ${OBJECTDIR}/main11v2.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1807030798/ST7789.o.d ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o.d ${OBJECTDIR}/_ext/1807030798/spi.o.d ${OBJECTDIR}/main11v2.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1807030798/ST7789.o ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o ${OBJECTDIR}/_ext/1807030798/spi.o ${OBJECTDIR}/main11v2.o

# Source Files
SOURCEFILES=../../../../../../Downloads/123/ST7789.c ../../../../../../Downloads/123/i2c_master_noint.c ../../../../../../Downloads/123/spi.c main11v2.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX170F256B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1807030798/ST7789.o: ../../../../../../Downloads/123/ST7789.c  .generated_files/e35e9dcfa2740fb290e749aa9f372b6c868e077f.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1807030798" 
	@${RM} ${OBJECTDIR}/_ext/1807030798/ST7789.o.d 
	@${RM} ${OBJECTDIR}/_ext/1807030798/ST7789.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1807030798/ST7789.o.d" -o ${OBJECTDIR}/_ext/1807030798/ST7789.o ../../../../../../Downloads/123/ST7789.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o: ../../../../../../Downloads/123/i2c_master_noint.c  .generated_files/3c7faa9c661f28aa235f66b934d1c70c6a384cc4.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1807030798" 
	@${RM} ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o.d 
	@${RM} ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o.d" -o ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o ../../../../../../Downloads/123/i2c_master_noint.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1807030798/spi.o: ../../../../../../Downloads/123/spi.c  .generated_files/f326b5e26bb0629189ca3ef5bf548c26adf8f66f.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1807030798" 
	@${RM} ${OBJECTDIR}/_ext/1807030798/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1807030798/spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1807030798/spi.o.d" -o ${OBJECTDIR}/_ext/1807030798/spi.o ../../../../../../Downloads/123/spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main11v2.o: main11v2.c  .generated_files/33b2af8f70daeacef58a010494df9db4bd7ae704.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main11v2.o.d 
	@${RM} ${OBJECTDIR}/main11v2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main11v2.o.d" -o ${OBJECTDIR}/main11v2.o main11v2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1807030798/ST7789.o: ../../../../../../Downloads/123/ST7789.c  .generated_files/6c5c3744af63f6db0392b0109de1799f9b636eeb.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1807030798" 
	@${RM} ${OBJECTDIR}/_ext/1807030798/ST7789.o.d 
	@${RM} ${OBJECTDIR}/_ext/1807030798/ST7789.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1807030798/ST7789.o.d" -o ${OBJECTDIR}/_ext/1807030798/ST7789.o ../../../../../../Downloads/123/ST7789.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o: ../../../../../../Downloads/123/i2c_master_noint.c  .generated_files/f143d2e72ad40dad7395301a37cb0d31ed96de29.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1807030798" 
	@${RM} ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o.d 
	@${RM} ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o.d" -o ${OBJECTDIR}/_ext/1807030798/i2c_master_noint.o ../../../../../../Downloads/123/i2c_master_noint.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1807030798/spi.o: ../../../../../../Downloads/123/spi.c  .generated_files/9fb66f1e910c120de3724ee41f2891a3acdf3ca5.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1807030798" 
	@${RM} ${OBJECTDIR}/_ext/1807030798/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1807030798/spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1807030798/spi.o.d" -o ${OBJECTDIR}/_ext/1807030798/spi.o ../../../../../../Downloads/123/spi.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main11v2.o: main11v2.c  .generated_files/163282805289c7353dc6b43e83b0213740697ef5.flag .generated_files/9c38dd73fa47c8727ca672a3ac38932765b77050.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main11v2.o.d 
	@${RM} ${OBJECTDIR}/main11v2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main11v2.o.d" -o ${OBJECTDIR}/main11v2.o main11v2.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/HW11v2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
