#pragma once

#include "pin.h"
#include "fat.h"
#include "ay_ym_file_play.h"
#include "ay_ym_low_lavel.h"
#include <memory>
#include "user_os.h"

#define AY_YM_FILE_PLAY_FLASH_BUFFER_SIZE_BYTE                1024 * 10

struct AyYmFilePlayCfg {
    AyYmLowLavel *ayLow;
    void ( *setPwr ) (uint32_t chipNumber,
                      bool state);
};

class AyYmFilePlay  {
public:
    AyYmFilePlay (const AyYmFilePlayCfg *const cfg);



    ///void	setUsingChip				(	uint32_t	chipNumber	);


    void setPause (bool state);
    void stop (void);

private:
    int get_len_file (uint32_t &len);
    int open_file (std::shared_ptr<char> f);
    int close_file (void);
    int set_offset (const uint32_t offset);
    int read (uint8_t *buf, const uint32_t num);
    int set_pwr_chip (const bool state);
    int init_chip (void);
    int sleep (const uint32_t num);
    int set_reg (const uint8_t reg, const uint8_t data);

private:
    void resetFlags (void);
    void abort (void);

private:
    const AyYmFilePlayCfg *const cfg;
    std::shared_ptr<FIL> file;
    AyPlayer::Fat fat;

    uint32_t usingChip = 0;
    bool flagStop;

    /// Т.к. методы зачастую читают по 1 байту, то чтобы ускорить этот процесс
    /// сразу копируется значительный кусок трека.
    __attribute__((__aligned__(4)))
    uint8_t flashBuffer[AY_YM_FILE_PLAY_FLASH_BUFFER_SIZE_BYTE];

    /// Смещение, с которого было скопирован кусок.
    uint32_t pointStartSeekBuffer;

    /// Смещение, с которого будет считан следующий байт/последовательность
    /// байт (относительно буффера).
    uint32_t pointInBuffer;
};


