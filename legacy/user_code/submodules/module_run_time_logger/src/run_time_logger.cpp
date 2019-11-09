#include "run_time_logger.h"

#ifdef MODULE_RUN_TIME_LOGGER_ENABLED

#include <stdio.h>
#include <string.h>
#include <errno.h>

const char INIT_OK[] = "[ INIT_OK ]\t\t";
const char INIT_ISSUE[] = "[ INIT_ISSUE ]\t\t";
const char INIT_ERROR[] = "[ INIT_ERROR ]\t\t";
const char RUN_MESSAGE_OK[] = "[ RUN_MESSAGE_OK ]\t";
const char RUN_MESSAGE_ISSUE[] = "[ RUN_MESSAGE_ISSUE ]\t";
const char RUN_MESSAGE_ERROR[] = "[ RUN_MESSAGE_ERROR ]\t";

const char *startString[6] = {
    INIT_OK, INIT_ISSUE, INIT_ERROR, RUN_MESSAGE_OK, RUN_MESSAGE_ISSUE, RUN_MESSAGE_ERROR
};

RunTimeLogger::RunTimeLogger (const RunTimeLoggerCfg *const cfg) : cfg(cfg) {
    this->m = xSemaphoreCreateMutexStatic(&mb);
    this->color[0] = this->cfg->color.initOkColorString;
    this->color[1] = this->cfg->color.initIssueColorString;
    this->color[2] = this->cfg->color.initErrorColorString;
    this->color[3] = this->cfg->color.runMessageOkColorString;
    this->color[4] = this->cfg->color.runMessageIssueColorString;
    this->color[5] = this->cfg->color.runMessageErrorColorString;
}

int RunTimeLogger::sendMessage (RTL_TYPE_M type, const char *message) {
    xSemaphoreTake(this->m, portMAX_DELAY);
    
    snprintf(this->bufMessage, MAX_MESSAGE_LEN, "%s %s %s \n\r", this->color[(uint8_t)type], startString[(uint8_t)type],
             message);
    
    mc_interfaces::res r;
    r = this->cfg->outBuffer((char *)this->bufMessage);
    
    xSemaphoreGive(this->m);
    
    if (r == mc_interfaces::res::err_ok) {
        return EOK;
    } else {
        return EIO;
    }
}

#endif
