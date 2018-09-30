#include "muxer.h"
#include <errno.h>

namespace AyPlayer {

Muxer::Muxer	(	const MuxerCfg*		const cfg	) : cfg( cfg ) {
}

int	Muxer::init	(	void	) {
	this->cfg->dp->shdnReset();

	if (	this->cfg->dp->setValue(	this->cfg->left.chipNumber,
										this->cfg->left.channelNumber,
										this->cfg->left.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->right.chipNumber,
										this->cfg->right.channelNumber,
										this->cfg->right.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->A1.chipNumber,
										this->cfg->A1.channelNumber,
										this->cfg->A1.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->B1.chipNumber,
										this->cfg->B1.channelNumber,
										this->cfg->B1.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->C1.chipNumber,
										this->cfg->C1.channelNumber,
										this->cfg->C1.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->A2.chipNumber,
										this->cfg->A2.channelNumber,
										this->cfg->A2.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->B2.chipNumber,
										this->cfg->B2.channelNumber,
										this->cfg->B2.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->C2.chipNumber,
										this->cfg->C2.channelNumber,
										this->cfg->C2.valueDefault )
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;


	return EOK;
}

int	Muxer::setVolume	(	uint8_t		volume	) {
	if (	this->cfg->dp->setValue(	this->cfg->left.chipNumber,
										this->cfg->left.channelNumber,
										volume	)
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	if (	this->cfg->dp->setValue(	this->cfg->right.chipNumber,
										this->cfg->right.channelNumber,
										volume	)
			!= DigitalPotentiometer::EC_AD5204_ANSWER::OK )
		return EIO;

	return EOK;
}

}
