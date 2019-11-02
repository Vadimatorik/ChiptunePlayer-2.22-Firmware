#pragma once

#include "project_config.h"

#ifdef MODULE_AY_YM_LOW_LAVEL_ENABLED

#include "shift_register_8bit_port.h"
#include "mc_port_8bit.h"
#include "pin.h"
#include "user_os.h"
#include "string.h"

// Данная структура показывает, как подключен чип AY к сдвиговому регистру.
// На каждый чип должна быть своя такая структура.

// Пример использования:
// При "прямом подключении": { 0, 1, 2, 3, 4, 5, 6, 7 }.
// При "обратном": { 7, 6, 5, 4, 3, 2, 1 }
struct ayYmConnectionChipCfg {
	const uint8_t	bias_bit[8];
};

/*
 * Структура описывает формат подключения AY/YM чипов к микроконтроллеру и
 * предоставляет низкоуровневые функции работы с ними.
 *
 * Предполагается, что к микроконтроллеру подключен модуль shift_register,
 * представляющий из себя сборку последовательно соединенных сдвиговых регистров.
 */
struct ayYmLowLavelCfg {

	USER_OS_STATIC_MUTEX*								const mutex;			/// Мутекс заберается на время передачи данных в AY/YM.
	mc_interfaces::Port8bit**					ports;					/// Порты, к которым подключены AY. Минимум 1 должен быть указан.

	USER_OS_STATIC_BIN_SEMAPHORE*						const semaphoreSecOut;	// Этим симафором будем показывать, что прошла секунда воспроизведения. Опционально (можно nullptr).

	// Выводы управления AY должны быть указаны обязательно (включение всех AY - параллельное).
	// Выводы должны быть указано явно.
	mc_interfaces::Pin*						const bdir;				// Выводы управления AY чипами (чипы включать в параллель).
	mc_interfaces::Pin*						const bc1;

	//
	// Для каждого AY своя очередь.
	// Здесь указатель на массив указателей на эти очереди.
	// Очередь должна содержать ay_low_out_data элементы!!!
	//
	USER_OS_STATIC_QUEUE*			queueArray;
	const uint8_t					ayNumber;							// Колличество AY.
	ayYmConnectionChipCfg*			const connectCfg;					// Способ подключения каждого чипа.
	const uint8_t					taskPrio;							// Приоритет задачи-обработчика данных из очереди.

	void	( *funcFrequencyAyControl )				( bool state );		/// Включить/выключить генерацию опорной частоты AY.
	void	( *funcTimInterruptTask )				( bool state );		/// Управление генерацией прерываний c частотой обновления AY (по умолчанию 50 гц).
};


struct ayLowOutDataStruct {
	uint8_t	 reg;		// Если сюда положат 0xFF, значит, что нужно больше для конкретного чипа в этом интервале времени посылок нет!
	uint8_t	 data;
};

/*
 * Очередь элементов для выдачи в AY.
 */
struct ayQueueStruct {
	uint8_t	 numberChip;
	uint8_t	 reg;		// Когда здесь 0xFF - поле data игнорируется, считается командой паузы в одно прерывание!
	uint8_t	 data;
};

struct __attribute__( ( packed ) ) ayChipReg {
	uint8_t reg[16];
};

#define AY_YM_LOW_LAVEL_TASK_STACK_SIZE			 2000

// Очередь должна быть как минимум 1 элемент (в идеале - по 16*2 b и более для каждого чипа).
// Очердь общая для все чипов!
class AyYmLowLavel {
public:
	AyYmLowLavel ( const ayYmLowLavelCfg* const cfg );

	// Добавляет элемент в очередь. Элемент будет выдан в АУ во время прерывания.
	int		queueAddElement					( ayQueueStruct* data );

	// Метод для приостановки воспроизвдеения и последущего возобновления с того же места.
	void	playStateSet					( uint8_t state );

	// Данный handler должен
	// быть вызван в прерывании по прохождении 50 мс
	// (частота может быть изменена другими методами, в зависимости от конфигурации воспроизведения).
	void	timerInterruptHandler			( void );

	//**********************************************************************
	// Метод возвращает текущее состояние очередей (всех).
	// Возвраащемое значение:
	// true	 - все очереди пусты.
	// false	- хотя бы в одной (в случае нескольких AY) есть данные.
	//**********************************************************************
	bool	queueEmptyCheck					( void );

	void	cleanBufferAyReg				( void );

	void	hardwareClear					( void );	 // Очищает все чипы (начальными значениями).
	void	queueClear						( void );

private:
	void	setReg							( void );
	void	setData							( void );

	// Метод преобразует байт (который был бы валидным для подключения
	// 0-й бит сдвигового регистра = 0-му биту шины AY. В валидный вид с учетом
	// реального подключения (например, 0-й бит сдвигового регисра = 7-му биту
	// шины данных AY... На тот случай, когда подключение производилось не соответсвующими
	// линиями.
	uint8_t		connectionTransformation	( const uint8_t chip, const uint8_t& data );

	void		sendBuffer					( void );

	// Сбрасывает флаги "паузы" на чипе.
	void	resetFlagWait	( bool* flagArray );
	bool	chackFlagWait	( bool* flagArray );

	static void task		( void* p_this );

	const ayYmLowLavelCfg*			const cfg;

	/*
	 * Этим симафором будем показывать, что пора передать следущую порцию данных.
	 * Мы ждем его в задаче ay_queue_out_task и отдаем в ay_timer_handler.
	 */
	USER_OS_STATIC_BIN_SEMAPHORE_BUFFER sb;
	USER_OS_STATIC_BIN_SEMAPHORE		s	 = nullptr;

	/*
	 * Для создания задачи.
	 */
	USER_OS_STATIC_STACK_TYPE			tb[ AY_YM_LOW_LAVEL_TASK_STACK_SIZE ] = { 0 };
	USER_OS_STATIC_TASK_STRUCT_TYPE		ts;

	uint8_t tic_ff		= 0;						 // Считаем время воспроизведения (колличество прерываний).

	ayChipReg*							buf_data_chip = nullptr;						// Массив структур данных регистров.
};

#endif
