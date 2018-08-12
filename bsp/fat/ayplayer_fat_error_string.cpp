#include "ayplayer_fat_error_string.h"

extern const char	sErrDiskErr[]					=	"DISK ERR!";			// Произошла невосстановимая ошибка на низком уровне (функции I/O диска).

extern const char	sErrIntErr[]					=	"INT ERR!";				// Ошибка Assert. Внутри процесса что-то пошло не так. Возможны следующие варианты.
																				// Любая ошибка структуры FAT на томе.
																				// Рабочая область (file system object,
																				// file object или что-то подобное) испорчена из-за переполнения стека или другого приложения.
																				// Эта причина ошибки бывает чаще всего.

extern const char	sErrNotReady[]					=	"NOT READY!";			// Драйв диска не может работать из-за некорректного извлечения носителя или ошибки функции disk_initialize.

extern const char	sErrNoFile[]					=	"NO FILE!";				// Файл не найден.

extern const char	sErrnoPath[]					=	"NO PATH!";				// Не найден путь.

extern const char	sErrInvalidName[]				=	"INVALID NAME!";		// Указанная строка содержит недопустимое имя пути (path name).

extern const char	sErrDenied[]					=	"DENIED!";				// Требуемый доступ был отклонен по одной из следующих причин:

																				// Открытие файла в режиме записи, когда файл имеет атрибут read-only (только чтение).
																				// Удаление файла или директории, которые имеют атрибут read-only.
																				// Удаление непустой директории или текущей директории.
																				// Чтение файла, открытого без флага FA_READ.
																				// Любая модификация файла, открытого без флага FA_WRITE.
																				// Невозможно создать файл или директорию из-за того, что таблица директорий переполнена.
																				// Невозможно создать директорию из-за того, что том переполнен.

extern const char	sErrExist[]						=	"EXIST!";				// Уже есть любой объект (файл или папка) с таким же именем.

extern const char	sErrInvalidObject[]				=	"INVALID OBJECT!";		// Предоставленная структура объекта файла/директории ошибочна.

extern const char	sErrWriteProtected[]			=	"WRITE PROTECTED!";		// Любое действие записи произведено на защищенном от записи носителе данных (write-protected media).

extern const char	sErrInvalidDrive[]				=	"INVALID DRIVE!";		// Указан недопустимый номер драйва (диска). См. также опцию _VOLUMES.

extern const char	sErrNotEnabled[]				=	"NOT ENABLED!";			// Рабочая область логического диска не зарегистрирована вызовом функции f_mount.

extern const char	sErrnoFilesystem[]				=	"NO FILESYSTEM!";		// На диске нет допустимого тома с файловой системой FAT.

extern const char	sErrMkfsAborted[]				=	"MKFS ABORTED!";		// Функция f_mkfs была остановлена (aborted) перед началом форматирования по одной из следующих причин:
																				// Размер диска слишком мал.
																				// Нет допустимого размера кластера для этого диска. Это может произойти, когда количество кластеров становится близким к значению 0xFF7 и 0xFFF7.


extern const char	sErrTimeout[]					=	"TIMEOUT!";				// Функция остановлена из-за таймаута в безопасном управлении потоками (thread-safe control). См. также опцию _TIMEOUT.

extern const char	sErrLocked[]					=	"LOCKED!";				// Доступ к файлу отклонен управлением совместного доступа к файлу (file sharing control). См. также опцию _FS_SHARE.

extern const char	sErrNotEnoughCore[]				=	"NOT ENOUGH_CORE!";		// Недостаточно памяти для выполнения операции. Это произошло по следующим причинам:
																				// Нельзя выделить память для рабочего буфера LFN (Long File Name, длинное имя файла). См. также опцию _USE_LFN.
																				// Указанный размер таблицы недостаточен.

extern const char	sErrTooManyOpenFiles[]			=	"TOO MANY OPEN FILES!";	// Количество открытых файлов достигло максимального значения и больше файлов открыть нельзя. См. также опцию _FS_SHARE.

extern const char	sErrInvalidParameter[]			=	"INVALID PARAMETER!";	// казанный параметр недопустим или в параметре имеется любое несоответствие.

