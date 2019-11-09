#pragma once

#include "ay_ym_low_lavel.h"
#include "user_os.h"

// В этих очередях содержатся данные для каждого AY.
// Они (данные) будут выдаваться в прерывании с заданной частотой.
#define QB_AY_LOW_SIZE                    64

struct FreeRtosObj {
    /// Queue.
    QueueHandle_t qAyLow[2];
    QueueHandle_t qAyButton;
    
    /// Queue buffer.
    uint8_t qbAyLow[2][sizeof(ayLowOutDataStruct) * QB_AY_LOW_SIZE];
    uint8_t qbAyButton[1];
    
    /// Queue struct.
    StaticQueue_t qsAyLow[2];
    StaticQueue_t qsAyButton;
    
    /// Mutex.
    SemaphoreHandle_t mBoardSpi;
    
    
    /// Mutex buffer.
    StaticSemaphore_t mbBoardSpi;
    
    
    /// Semaphore.
    SemaphoreHandle_t sPlayTic;
    SemaphoreHandle_t sStartPlay;
    
    /// Semaphore buffer.
    StaticSemaphore_t sbPlayTic;
    StaticSemaphore_t sbStartPlay;            /// Выдается, когда надо запустить воспроизведение,
    ///а все данные о треке уже указаны (внутри объекта ayplayer).
};

extern FreeRtosObj osData;

