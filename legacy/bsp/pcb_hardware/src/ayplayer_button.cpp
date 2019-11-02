#include "ayplayer_button.h"

#include "buttons_base.h"
#include "user_os.h"
#include "ayplayer_os_object.h"
#include "ayplayer_mc_hardware.h"

bool getKey (uint8_t id) {
    keyboardStrings[0].set();
    keyboardStrings[1].set();
    keyboardStrings[2].set();
    
    switch (id) {
        case 0:
            keyboardStrings[0].reset();
            return !keyboardColumns[1].read();
        case 1:
            keyboardStrings[2].reset();
            return !keyboardColumns[1].read();
        case 2:
            keyboardStrings[1].reset();
            return !keyboardColumns[0].read();
        case 3:
            keyboardStrings[1].reset();
            return !keyboardColumns[2].read();
        case 4:
            keyboardStrings[1].reset();
            return !keyboardColumns[1].read();
        case 5:
            keyboardStrings[0].reset();
            return !keyboardColumns[2].read();
        case 6:
            keyboardStrings[0].reset();
            return !keyboardColumns[0].read();
    }
    
    return false;
}

// При опуске коротком или длинном - одна и та же очередь.
button::one_button_cfg cfgButtons[7] = {
    {
        .id                            =    0,
        .stabilization_time_ms        =    50,
        .long_press_detection_time_sec    =    1,
        .press                        =    {nullptr, nullptr, 0},
        .click                        =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::UP_CLICK)
        },
        .long_press                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::UP_LONG_PRESS)
        },
        .long_click                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::UP_LONG_CLICK)
        }
    },
    {
        .id                            =    1,
        .stabilization_time_ms        =    50,
        .long_press_detection_time_sec    =    1,
        .press                        =    {nullptr, nullptr, 0},
        .click                        =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::DOWN_CLICK)
        },
        .long_press                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::DOWN_LONG_PRESS)
        },
        .long_click                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::DOWN_LONG_CLICK)
        }
    },
    {
        .id                            =    2,
        .stabilization_time_ms        =    50,
        .long_press_detection_time_sec    =    1,
        .press                        =    {nullptr, nullptr, 0},
        .click                        =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::LEFT_CLICK)
        },
        .long_press                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::LEFT_LONG_PRESS)
        },
        .long_click                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::LEFT_LONG_CLICK)
        }
    },
    {
        .id                            =    3,
        .stabilization_time_ms        =    50,
        .long_press_detection_time_sec    =    1,
        .press                        =    {nullptr, nullptr, 0},
        .click                        =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::RIGHT_CLICK)
        },
        .long_press                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::RIGHT_LONG_PRESS)
        },
        .long_click                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::RIGHT_LONG_CLICK)
        }
    },
    {
        .id                            =    4,
        .stabilization_time_ms        =    50,
        .long_press_detection_time_sec    =    1,
        .press                        =    {nullptr, nullptr, 0},
        .click                        =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::ENTER_CLICK)
        },
        .long_press                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::ENTER_LONG_PRESS)
        },
        .long_click                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::ENTER_LONG_CLICK)
        }
    },
    {
        .id                            =    5,
        .stabilization_time_ms        =    50,
        .long_press_detection_time_sec    =    1,
        .press                        =    {nullptr, nullptr, 0},
        .click                        =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::BACK_CLICK)
        },
        .long_press                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::BACK_LONG_PRESS)
        },
        .long_click                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::BACK_LONG_CLICK)
        }
    },
    {
        .id                            =    6,
        .stabilization_time_ms        =    50,
        .long_press_detection_time_sec    =    1,
        .press                        =    {nullptr, nullptr, 0},
        .click                        =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::MENU_CLICK)
        },
        .long_press                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::MENU_LONG_PRESS)
        },
        .long_click                    =    {
            .s                        =    nullptr,
            .q                        =    &osData.qAyButton,
            .v                        =    M_EC_TO_U8(EC_BUTTON_NAME::MENU_LONG_CLICK)
        }
    }
};

button::base_cfg cfgButton = {
    .task_delay_ms                    =    10,
    .task_prio                        =    3,
    .cfg_array                        =    cfgButtons,
    .cfg_array_size                    =    7,
    .get_button_state                    =    getKey
};

button::base buttons(&cfgButton);
