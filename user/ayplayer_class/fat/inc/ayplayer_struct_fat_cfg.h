#pragma once

#include "ff.h"
#include <memory>

enum class AyPlayFileFormat {
	psg				=	0,
};

/// Структура одного элемента в файле <<*.list>>.
struct ItemFileInFat {
	char						fileName[ FF_MAX_LFN + 1 ];
	AyPlayFileFormat			format;

	/// Длина файла в прерываниях для обновления регистров AY/YM.
	uint32_t					lenTick;
};

struct AyPlayerFatFs {


	/// Информация о файле, который в данный момент воспроизводится.
	//ItemFileInFat				currentFileInfo;

	/// Директория, в которой находится файловый менеджер.
	//DIR							playDir;
};
