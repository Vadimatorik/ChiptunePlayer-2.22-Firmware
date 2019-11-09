#define USER_OS_STATIC_TIMER_CHANGE_PERIOD(TIMER, PERIOD)                                        xTimerChangePeriod(TIMER,PERIOD,portMAX_DELAY)
#define USER_OS_STATIC_TIMER_GET_PERIOD(TIMER)                                                    xTimerGetPeriod(TIMER)
#define USER_OS_STATIC_TIMER_STOP(TIMER)                                                        xTimerStop(TIMER,portMAX_DELAY)
#define USER_OS_STATIC_TIMER_RESET(TIMER)                                                        xTimerReset(TIMER,portMAX_DELAY)

//**********************************************************************
//								Определения, не касающиеся основных модулей.
//**********************************************************************
#define BIT_BAND_SRAM_REF 0x20000000
#define BIT_BAND_SRAM_BASE 0x22000000

//Получаем адрес бита RAM в Bit Banding области.
#define MACRO_GET_BB_P_SRAM(reg, bit) ((BIT_BAND_SRAM_BASE + (reg - BIT_BAND_SRAM_REF)*32 + (bit * 4)))

#define BIT_BAND_PER_REF ((uint32_t)0x40000000)
#define BIT_BAND_PER_BASE ((uint32_t)0x42000000)

// Получаем адрес бита периферии в Bit Banding области.
#define M_GET_BB_P_PER(ADDRESS, BIT) ((BIT_BAND_PER_BASE + (ADDRESS - BIT_BAND_PER_REF)*32 + (BIT * 4)))

// Преобразует число в uint32_t переменной в указатель на uint32_t.
// Причем запрещает переписывать то, что по указателю (только чтение).
#define M_U32_TO_P_CONST(point)         ( ( const uint32_t *const )( point ) )

// Преобразует число в uint32_t переменной в указатель на uint32_t.
// Данные по указателю можно изменять.
#define M_U32_TO_P(point)                ( ( uint32_t * )point )

// Преобразует enum class в uint8_t.
#define M_EC_TO_U8(ENUM_VALUE)            ( ( uint8_t )ENUM_VALUE )

// Преобразует enum class в uint32_t.
#define M_EC_TO_U32(ENUM_VALUE)         ( ( uint32_t )ENUM_VALUE )


// Возвращаем размер массива.
#define M_SIZE_ARRAY(ARRAY)             (sizeof(ARRAY)/sizeof(ARRAY[0]))

// Служит для записи таблицы векторов прерываний.
#define M_ISR_TO_CONST_P_VOID(POINT)    ((void*)(POINT))

#if !defined(EOK)
#define EOK 0 /// No error.
#endif

#define    errnoCheckAndReturn(r)                \
            if ( r != EOK ) {                    \
                return r;                        \
            }
