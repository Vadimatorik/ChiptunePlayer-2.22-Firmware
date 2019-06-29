function(write_sector SECTOR ADDR_BASE ADDR_START ADDR_END)
    add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
            COMMAND ${ARM_OBJCOPY} --output-target=binary --gap-fill=0xff ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_all.bin
            COMMENT "Creating a binary file of the <<${SECTOR}>> sector"
            COMMAND ${BIN_EXTRACTOR} -p ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_all.bin -o ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_${SECTOR}_new.bin -b ${ADDR_BASE} -s ${ADDR_START} -e ${ADDR_END}
            COMMAND cd ${CMAKE_SOURCE_DIR} && ./cmp.sh ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_${SECTOR}.bin ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_${SECTOR}_new.bin "${STM32PROG} -c port=${STM32PROG_PORT} freq=${STM32PROG_FREQ} -w ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_section_${SECTOR}.bin ${ADDR_START}")
endfunction(write_sector)