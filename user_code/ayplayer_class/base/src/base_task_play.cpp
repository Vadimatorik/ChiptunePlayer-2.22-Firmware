#include "base.h"

namespace AyPlayer {

void Base::startPlayTrack (void) {
    int r;
    this->playItem = this->fat.readItemFileList(this->playList, this->currentFile, r);
    this->gui->updateTreckWindowMain(this->playItem);
    USER_OS_GIVE_BIN_SEMAPHORE(this->cfg->os->sStartPlay);
}

void Base::playTask (void *obj) {
    Base *o = (Base *)obj;
    int r;
    while (true) {
        USER_OS_TAKE_BIN_SEMAPHORE(o->cfg->os->sStartPlay, portMAX_DELAY);

        do {
            o->playState = AYPLAYER_STATUS::PLAY;

            uint32_t nameLen = strlen(o->playItem->fileName) + 1;
            std::shared_ptr<char> fileName(new char[nameLen], std::default_delete<char[]>());
            strcpy(fileName.get(), o->playItem->fileName);

            r = o->cfg->ay->parse_psg(fileName);

            /// Если трек был остановлен или проблемы на аппаратном уровне.
            if (r != 0) {
                o->playState = AYPLAYER_STATUS::STOP;
                break;
            }

            /// Переходим на следующий трек если это был не последний.
            if (o->currentFile < o->countFileInCurrentDir - 1) {
                o->currentFile++;
            } else {
                o->currentFile = 0;
            }

            o->playItem = o->fat.readItemFileList(o->playList, o->currentFile, r);
            if (r != EOK)
                break;

            o->gui->updateTreckWindowMain(o->playItem);

        } while (true);
    }

    while (1);
}

}
