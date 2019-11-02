#include "fatfs.h"

#include <string.h>

int fatfs::initFatFs (std::shared_ptr<char> fatStartDir) {
    FRESULT fr;

    static FATFS f;

    fr = f_mount(&f, fatStartDir.get(), 1);

    if (fr == FRESULT::FR_OK) {
        return EOK;
    } else {
        return EIO;
    }
}

std::shared_ptr<DIR> fatfs::openDir (std::shared_ptr<char> path,
                                   int &returnResult) {
    FRESULT r;
    std::shared_ptr<DIR> d(new DIR);

    if (d.get() == nullptr) {
        returnResult = ENOMEM;
        return d;
    }

    /// Открываем директорию.
    r = f_opendir(d.get(), path.get());

    if (r != FRESULT::FR_OK) {
        returnResult = EIO;
        return std::shared_ptr<DIR>(nullptr);
    } else {
        returnResult = EOK;
        return d;
    }
}

int fatfs::setCurrentDir (const char *const path) {
    FRESULT r;
    r = f_chdir(path);

    if (r == FRESULT::FR_OK) {
        return EOK;
    } else {
        return EIO;
    }

    return EIO;
}

std::shared_ptr<FILINFO> fatfs::readDir (std::shared_ptr<DIR> dir,
                                       int &returnResult) {
    FRESULT r;
    std::shared_ptr<FILINFO> f(new FILINFO);

    if (f.get() == nullptr) {
        returnResult = ENOMEM;
        return f;
    }

    r = f_readdir(dir.get(), f.get());

    if (r != FRESULT::FR_OK) {
        returnResult = EIO;
        return std::shared_ptr<FILINFO>(nullptr);
    }

    returnResult = EOK;

    if (f->fname[0] == 0) {
        return std::shared_ptr<FILINFO>(nullptr);
    }

    return f;
}

bool fatfs::checkFileIsDir (std::shared_ptr<FILINFO> fileInfo) {
    if (fileInfo->fattrib & AM_DIR) {
        return true;
    } else {
        return false;
    }
}

int fatfs::closeDir (std::shared_ptr<DIR> dir) {
    FRESULT r;

    r = f_closedir(dir.get());

    if (r == FRESULT::FR_OK) {
        return EOK;
    } else {
        return EIO;
    }
}


std::shared_ptr<DIR> fatfs::openDirAndFindFirstFile (std::shared_ptr<char> path,
                                                   std::shared_ptr<FILINFO> returnFileInfo,
                                                   std::shared_ptr<char> maskFind,
                                                   int &returnResult) {
    FRESULT r;
    std::shared_ptr<DIR> d(new DIR);
    if (d.get() == nullptr) {
        returnResult = ENOMEM;
        return d;
    }

    /// Открываем директорию и ищем в ней файл по маске.
    r = f_findfirst(d.get(), returnFileInfo.get(), path.get(), maskFind.get());

    /// Этим мы указываем на то, что с самой Fat и картой все нормально.
    if (r == FR_OK) {
        returnResult = EOK;
    } else {
        returnResult = EIO;
        return std::shared_ptr<DIR>(nullptr);
    }

    /// Нет такого файла.
    if (returnFileInfo->fname[0] == 0) {
        return std::shared_ptr<DIR>(nullptr);
    }

    return d;
}

std::shared_ptr<FILINFO> fatfs::findNextFileInDir (std::shared_ptr<DIR> dir,
                                                 int &returnResult) {
    FRESULT r;
    std::shared_ptr<FILINFO> f(new FILINFO);
    if (f.get() == nullptr) {
        returnResult = ENOMEM;
        return f;
    }

    r = f_findnext(dir.get(), f.get());

    /// Этим мы указываем на то, что с самой Fat и картой все нормально.
    if (r == FR_OK) {
        returnResult = EOK;
    } else {
        returnResult = EIO;
        return std::shared_ptr<FILINFO>(nullptr);
    }

    if (f->fname[0] == 0) {
        return std::shared_ptr<FILINFO>(nullptr);
    }

    return f;
}

std::shared_ptr<char> fatfs::getFullPath (std::shared_ptr<char> path,
                                        const char *const fileName,
                                        int &returnResult) {
    std::shared_ptr<char> fatFsPath(new char[fatfs::MAX_PATH_FATFS_STRING_LEN + 1], std::default_delete<char[]>());

    if (fatFsPath.get() == nullptr) {
        returnResult = ENOMEM;
        return fatFsPath;
    } else {
        returnResult = EOK;
    }

    snprintf(fatFsPath.get(), fatfs::MAX_PATH_FATFS_STRING_LEN, "%s/%s", path.get(), fileName);

    return fatFsPath;
}

std::shared_ptr<FIL> fatfs::openFileListWithRewrite (std::shared_ptr<char> path,
                                                   const char *const fileName,
                                                   int &returnResult) {
    std::shared_ptr<char> fullPath(this->getFullPath(path, fileName, returnResult));
    if (returnResult != EOK) {
        returnResult = ENOMEM;
        return std::shared_ptr<FIL>(nullptr);
    }

    return this->openFileListWithRewrite(fullPath, returnResult);
}

std::shared_ptr<FIL> fatfs::openFileListWithRewrite (std::shared_ptr<char> fullPath,
                                                   int &returnResult) {
    FRESULT r;
    std::shared_ptr<FIL> f(new FIL);

    if (f.get() == nullptr) {
        returnResult = ENOMEM;
        return f;
    } else {
        returnResult = EOK;
    }

    r = f_open(f.get(), fullPath.get(), FA_CREATE_ALWAYS | FA_READ | FA_WRITE);

    if (r != FR_OK) {
        returnResult = EIO;
        return std::shared_ptr<FIL>(nullptr);
    }

    return f;
}


int fatfs::close_file (std::shared_ptr<FIL> file) {
    FRESULT r;

    r = f_close(file.get());

    if (r == FRESULT::FR_OK) {
        return EOK;
    } else {
        return EIO;
    }
}


std::shared_ptr<FIL> fatfs::openFile (std::shared_ptr<char> fullPath,
                                    int &returnResult) {
    FRESULT r;
    std::shared_ptr<FIL> f(new FIL);

    if (f.get() == nullptr) {
        returnResult = ENOMEM;
        return f;
    } else {
        returnResult = EOK;
    }

    r = f_open(f.get(), fullPath.get(), FA_READ);

    if (r != FR_OK) {
        returnResult = EIO;
        return std::shared_ptr<FIL>(nullptr);
    }

    return f;
}

std::shared_ptr<FIL> fatfs::openFile (const char *const path,
                                    int &returnResult) {
    FRESULT r;
    std::shared_ptr<FIL> f(new FIL);

    if (f.get() == nullptr) {
        returnResult = ENOMEM;
        return f;
    } else {
        returnResult = EOK;
    }

    r = f_open(f.get(), path, FA_READ);

    if (r != FR_OK) {
        returnResult = EIO;
        return std::shared_ptr<FIL>(nullptr);
    }

    return f;
}

std::shared_ptr<FIL> fatfs::openFile (std::shared_ptr<char> path,
                                    const char *const name,
                                    int &returnResult) {
    std::shared_ptr<char> fullPath(this->getFullPath(path, name, returnResult));
    if (returnResult != EOK) {
        returnResult = ENOMEM;
        return std::shared_ptr<FIL>(nullptr);
    }

    return this->openFile(fullPath, returnResult);
}

uint32_t fatfs::size (std::shared_ptr<FIL> file) const {
    return f_size(file);
}

int fatfs::read (std::shared_ptr<FIL> file,
                 uint8_t *returnDataArray,
                 uint32_t countReadByte) {
    UINT l;
    FRESULT r;

    r = f_read(file.get(), returnDataArray, static_cast< UINT >( countReadByte ), &l);

    if (r != FRESULT::FR_OK) {
        return EIO;
    }

    if (l != countReadByte) {
        return EINVAL;
    }

    return EOK;
}


int fatfs::setOffsetByteInFile (std::shared_ptr<FIL> file,
                              uint32_t offset) {
    if (f_lseek(file.get(), offset) == FR_OK) {
        return EOK;
    } else {
        return EIO;
    }
}





/// 1
/*
int Fat::checkingFileOrDir ( const char* path, const char* nameFile, FILINFO* fi, FRESULT& fatReturn ) {
	FRESULT		r;

	char* fullPath;
	fullPath = Fat::getFullPath( path, nameFile );
	r = f_stat( fullPath, fi );
	vPortFree( fullPath );

	switch( ( uint32_t )r ) {
	case ( uint32_t )FR_OK:			return 1;
	case ( uint32_t )FR_NO_FILE:	return 0;
	default:
		fatReturn = r;
		return -1;
	}
}*/


int fatfs::removeDir (std::shared_ptr<char> path, FRESULT &fatReturn) {
    FRESULT r;
    do {
        r = f_chmod(path.get(), 0, AM_RDO | AM_ARC | AM_SYS | AM_HID);        /// Снимаем блокировки.
        if (r != FR_OK) break;
        r = f_unlink(path.get());
    } while (false);

    switch ((uint32_t)r) {
        case (uint32_t)FR_OK:
            return 0;
        case (uint32_t)FR_NO_FILE:
            return 1;
        default:
            fatReturn = r;
            return -1;
    }
}

int fatfs::removeDirRecurisve (std::shared_ptr<char> fullPath, FRESULT &fatReturn) {
    (void)fullPath;
    (void)fatReturn;
    static FILINFO f;
    static int r = 0;
    int mem_r;

    std::shared_ptr<DIR> d(fatfs::openDir(fullPath, mem_r));

    /// Что-то не так с драйверами или директории не существует.
    if (d == nullptr)
        return -1;

    /// Рекурсивно обходим все папки.
    while (1) {
        fatReturn = f_readdir(d.get(), &f);

        /// Если проблемы на нижнем уравне.
        if (fatReturn != FR_OK) {
            r = -1;
            break;
        }

        /// Закончились элементы в текущей директории.
        if (f.fname[0] == 0)
            break;

        /// Найдена новая директория.
        if (f.fattrib & AM_DIR) {
            uint32_t i = strlen(fullPath.get());
            sprintf(&fullPath.get()[i], "/%s", f.fname);

            r = fatfs::removeDirRecurisve(fullPath, fatReturn);

            if (fatReturn != FRESULT::FR_OK) {                                /// Аварийная ситуация.
                r = -1;
                break;
            }

            fullPath.get()[i] = 0;
        } else {
            r = fatfs::removeFile(fullPath, f.fname, fatReturn);
            if (r != 0)
                break;
        }
    }

    /// Фиксируем FRESULT::FR_DISK_ERR как приоритет над всем.
    r = (fatfs::closeDir(d) == -1)?-1:r;

    /// Удалили все в директории можно и ее саму.
    if (r == 0)
        r = fatfs::removeDir(fullPath, fatReturn);

    return r;
}

int fatfs::removeFile (std::shared_ptr<char> path, const char *nameFile, FRESULT &fatReturn) {
    int int_r;
    std::shared_ptr<char> fullPath(this->getFullPath(path, nameFile, int_r));
    if (int_r != EOK) {
        return ENOMEM;
    }

    do {
        fatReturn = f_chmod(fullPath.get(), 0, AM_RDO | AM_ARC | AM_SYS | AM_HID);        /// Снимаем блокировки.
        if (fatReturn != FR_OK) break;
        fatReturn = f_unlink(fullPath.get());
    } while (false);

    switch ((uint32_t)fatReturn) {
        case (uint32_t)FR_OK:
            return 0;
        case (uint32_t)FR_NO_FILE:
            return 1;
        default:
            return -1;
    }
}