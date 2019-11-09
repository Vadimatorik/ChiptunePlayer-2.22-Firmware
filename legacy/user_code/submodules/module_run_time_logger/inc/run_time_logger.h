#pragma once

#include "project_config.h"

#ifdef MODULE_RUN_TIME_LOGGER_ENABLED

#ifdef __cplusplus

#include <stdint.h>
#include "user_os.h"
#include "mc_base.h"

#if !defined( EOK )
#define	EOK					0							/// No error.
#endif

enum class RTL_TYPE_M {
    INIT_OK = 0,                            // Инициализация идет нормально.
    INIT_ISSUE = 1,                            // Есть некоторые затруднения, но можно и без этого жить.
    INIT_ERROR = 2,                            // Критическая проблема. Без этого дальнейшее действие уже невозможно (или откад для повторной попытки/пропуск).
    RUN_MESSAGE_OK = 3,                            // Сообщение времени выполнения.
    RUN_MESSAGE_ISSUE = 4,                            // Что-то пошло не так во время выполнения, но в целом все хорошо.
    RUN_MESSAGE_ERROR = 5                                // Что-то в реальном времени пошло не так.
};

const char ANSI_RED[] = "\x1B[31m";
const char ANSI_GRN[] = "\x1B[32m";
const char ANSI_YEL[] = "\x1B[33m";
const char ANSI_BLU[] = "\x1B[34m";
const char ANSI_MAG[] = "\x1B[35m";
const char ANSI_CYN[] = "\x1B[36m";
const char ANSI_WHT[] = "\x1B[37m";

const char ANSI_RESET[] = "\x1B[0m";

// Цвета для разных типов сообщений.
struct colorMessageStruct {
    const char *initOkColorString;
    const char *initIssueColorString;
    const char *initErrorColorString;
    const char *runMessageOkColorString;
    const char *runMessageIssueColorString;
    const char *runMessageErrorColorString;
};

struct RunTimeLoggerCfg {
    colorMessageStruct color;
    
    mc_interfaces::res ( *outBuffer ) (const char *string);
};

#define MAX_MESSAGE_LEN                1024

class RunTimeLogger {
public:
    RunTimeLogger (const RunTimeLoggerCfg *const cfg);
    
    /*!
     *	\brief		Метод формирует выходную строку согласно
     *				типу сообщения и производит ее отправку
     *				через указанный пользователем при
     *				инициализации метод.
     *
     *	\param[in]	type		-	тип записываемого в log сообщения.
     *	\param[in]	message		-	строка с текстом записываемого сообщения.
     *								Примечание:	после вызова метода
     *											может быть удалена.
     *
     *	\return		EOK			-	Успешность операции.
     *								В противном случае код из errno.h.
     */
    int sendMessage (RTL_TYPE_M type, const char *message);

private:
    const RunTimeLoggerCfg *const cfg;
    
    SemaphoreHandle_t m = nullptr;
    StaticSemaphore_t mb;
    const char *color[6];
    
    char bufMessage[MAX_MESSAGE_LEN];
    
};

#endif

#endif
