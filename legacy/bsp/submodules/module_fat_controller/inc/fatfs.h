#pragma once

#ifdef __cplusplus

#include "ff.h"

#include <memory>
#include <string>

#if !defined( EOK )
#define    EOK    0 /// No error.
#endif

class fatfs {
public:
/*!
 *	\brief		Максимальная длинна строки-пути в fatfs.
 *				Важно: при выделении памяти следует выделять на 1 байт больше.
 */

    static const uint32_t  MAX_PATH_FATFS_STRING_LEN   =             1024;

/*!
     *	\brief		Метод производит инициализацию FatFS
     *				объекта на основе данных с физической
     *				карты памяти.
     *
     *	\param[in]	fatStartDir		-	путь до директории, в которой будет находится браузер файлов
     *									после инициализации. Путь должен быть полным содержать в себе
     *									указание на карту, которая будет инициализирована.
     *									После вызова метода строку можно удалять.
     *
     *	\return		Результат операции.
     *				EOK	-	в случае успеха.
     *				Иначе код ошибки из errno.h
     */
    int initFatFs (std::shared_ptr<char> fatStartDir);

    /*!
     *	\brief		Открывает директорию по заданному пути
     *				с последующим созданием ее объекта FatFS.
     *	\param[in]		path			-	путь до требуемого каталога.
     * 										Примечание:	путь должен быть полным,
     * 													включая карту.
     *	\param[out]		returnResult	-	Результат операции.
     *										EOK	-	в случае успеха.
     *										Иначе код ошибки из errno.h
     *
     *	\return			Объект директории FatFs.
     *					В случае, если не удастся выделить память
     *					или карта не ответила,
     *					то будет возвращен объект, указывающий на
     *					nullptr.
     */
    std::shared_ptr<DIR> openDir (std::shared_ptr<char> path,
                                  int &returnResult);


    int setCurrentDir (const char *const path);


    /*!
     *	\brief		Открывает директорию с возможностью последующего
     *				анализа находящихся внутри нее файлов по маске.
     *
     *	\param[in]		path			-	путь до требуемого каталога.
     * 										Примечание:	путь должен быть полным,
     * 	\param[out]		returnFileInfo	-	объект информации о файле, данные в котором
     * 										будут актуализированы в случае успеха.
     * 	\param[in]		maskFind		-	строка-маска, по которой будет
     * 										производиться поиск.
     *	\param[out]		returnResult	-	Результат операции.
     *										EOK	-	в случае успеха.
     *										Иначе код ошибки из errno.h
     *
     *	\return			Объект директории FatFs.
     *					В случае, если не удастся выделить память,
     *					карта не ответила или файл не был найден,
     *					то будет возвращен объект, указывающий на
     *					nullptr.
     */
    std::shared_ptr<DIR> openDirAndFindFirstFile (std::shared_ptr<char> path,
                                                  std::shared_ptr<FILINFO> returnFileInfo,
                                                  std::shared_ptr<char> maskFind,
                                                  int &returnResult);

    std::shared_ptr<FILINFO> findNextFileInDir (std::shared_ptr<DIR> dir,
                                                int &returnResult);

    /*!
     *	\brief		Метод считывает данные о файле из внутреннего
     *				списка файлов в каталоге.
     *
     *	\param[in]		dir				-	объект ранее открытой методом openDir директории.
     *										Важно: проверки, что объект не был создан - не производится.
     *										Об этом должен заботиться пользователь.
     *	\param[out]		returnResult	-	Результат операции.
     *										EOK	-	в случае успеха.
     *										Иначе код ошибки из errno.h
     *
     *	\return			Объект с информацией о файле.
     *					В случае, если не удастся выделить память
     *					или карта не ответила,
     *					то будет возвращен объект, указывающий на
     *					nullptr.
     *					Важно: в случае, если файлы в директории закончились
     *					- так же будет возвращен объект указывающий на nullptr.
     *					При этом returnResult будет EOK.
     */
    std::shared_ptr<FILINFO> readDir (std::shared_ptr<DIR> dir,
                                      int &returnResult);

    /*!
     *	\brief		Метод проверяет, является ли считанный
     *				файл директорией или нет.
     *
     *	\param[in]		fileInfo		-	объект информации о файле, полученный ранее с
     *										помощью метода readDir.
     *										Важно: проверки, что объект не был создан -
     *										не производится.
     *										Об этом должен заботиться пользователь.
     *
     *	\return			true	-	если файл является директорией.
     */
    bool checkFileIsDir (std::shared_ptr<FILINFO> fileInfo);

    /*!
     *	\brief		Метод закрывает ранее открытую с помощью
     *				метода openDir директорию.
     *
     *	\param[in]		dir				-	объект ранее открытой методом openDir директории.
     *										Важно: проверки, что объект не был создан - не производится.
     *										Об этом должен заботиться пользователь.
     *
     *	\return		Результат операции.
     *				EOK	-	в случае успеха.
     *				Иначе код ошибки из errno.h
     */
    int closeDir (std::shared_ptr<DIR> dir);

    /*!
     *	\brief		Метод возвращает строку с полным путем,
     *				состоящим из пути + имя файла.
     *
     *	\param[in]		path			-	путь до директории.
     *	\param[in]		fileName		-	имя файла с расширением.
     *	\param[out]		returnResult	-	Результат операции.
     *										EOK	-	в случае успеха.
     *										Иначе код ошибки из errno.h
     *
     *	\return			Строка, содержащая полный путь до файла.
     */
    std::shared_ptr<char> getFullPath (std::shared_ptr<char> path,
                                       const char *const fileName,
                                       int &returnResult);

    /*!
         * Открывает файл по заданному пути/
         * Файл должен существовать.
         * Если файл по данному пути существовал ранее - заменяет его.
         * \param[in]		path		-	путь до файла, который будет открыт
         * 									(полный, включая расширение и карту).
         *
         * \return			{	Указатель на объект файла FatFS
         * 						или nullptr, если открыть не удалось.	}
         */
    std::shared_ptr<FIL> openFile (std::shared_ptr<char> path,
                                   const char *const name,
                                   int &returnResult);

    std::shared_ptr<FIL> openFile (std::shared_ptr<char> fullPath,
                                   int &returnResult);

    std::shared_ptr<FIL> openFile (const char *const path,
                                   int &returnResult);


    /*!
     *	\brief		Открывает файл по заданному пути или создает его заново.
     *				Если файл по данному пути существовал ранее - заменяет его.
     *	\param[in]		fullPath		-	путь до файла, который будет открыт
     * 										(полный, включая расширение и карту).
     *	\param[out]		returnResult	-	Результат операции.
     *										EOK	-	в случае успеха.
     *										Иначе код ошибки из errno.h
     *
     *	\return			FatFS объект файла.
     *
     */
    std::shared_ptr<FIL> openFileListWithRewrite (std::shared_ptr<char> fullPath,
                                                  int &returnResult);

    std::shared_ptr<FIL> openFileListWithRewrite (std::shared_ptr<char> path,
                                                  const char *const fileName,
                                                  int &returnResult);


    /*!
     * Возвращает размер файла.
     * \param[in]		f				-	открытый файл, размер которого необходимо получить.
     * \param[in]		returnSizeByte	-	переменная, в которую будет положен размер трека.
     *
     * \return			{	Длина трека или 0xFFFFFFFF	}
     */
    uint32_t size (std::shared_ptr<FIL> file) const;


    int read (std::shared_ptr<FIL> file,
              uint8_t *returnDataArray,
              uint32_t countReadByte);


    int setOffsetByteInFile (std::shared_ptr<FIL> file,
                             uint32_t offset);


    /*!
        * Открывает директорию и ищет в ней первый элемент по маске.
        * \param[out]		fInfo		-	указатель на указатель на структуру информации о файле FatFS.
        * \prarm[out]		d			-	указатель на указатель на объект директории FatFS.
        * \param[in]		pathDir		-	директория, в которой будет производится поиск.
        * \param[in]		maskFind	-	маска поиска.
        *
        * \return			{	0	-	элемент найден успешно, информация о
        * 								нем находится в объекте информации о файле FatFS
        * 								( fInfo ).
        * 						-1	-	произошла ошибка на уровне FatFS, память была высвобождена.
        * 						1	-	закончились файлы по маске в директории, память была высвобождена.	}
        */
    static FIL *openFileInCurrentDir (const char *const name);
    /*!
     * Закрывает ранее открытый файл и удаляет его объект FatFS.
     * \param[in]		f			-	объект файла FatFS.
     *
     * \return			{	0	-	файл успешно закрыт, объект
     * 								файла FatFS удален.
     * 						-1	-	файл закрыт с ошибкой, объект
     * 								файла FatFS удален.	}
     */
    int close_file (std::shared_ptr<FIL> file);

    /*!
       * Продолжает поиск файла по заданной методом startFindingFileInDir маске.
       * \param[in]		f			-	объект директории FatFS.
       * \param[out]		fInfo		-	объект структуры информации о файле FatFS.
       */
    static int findingFileInDir (DIR *d, FILINFO *fInfo);
    static int setOffsetByteInOpenFile (FIL *f, uint32_t offset);

    /*!
    * Проверяет наличие файла.
    */
    int checkingFileOrDir (std::shared_ptr<char> path, const char *nameFile, FILINFO *fi, FRESULT &fatReturn);
    int removeFile (std::shared_ptr<char> path, const char *nameFile, FRESULT &fatReturn);


    /// Внимание! Нужен большой под path массив!
    int removeDir (std::shared_ptr<char> path, FRESULT &fatReturn);

    int removeDirRecurisve (std::shared_ptr<char> path, FRESULT &fatReturn);
};

#endif
