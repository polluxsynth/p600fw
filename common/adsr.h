#ifndef ADSR_H
#define	ADSR_H

#include "p600.h"

typedef enum
{
	sWait=0,sAttack=1,sDecay=2,sSustain=3,sRelease=4,sDone=5
} adsrStage_t;

struct adsr_s
{
	uint32_t stageIncrement;	
	uint32_t phase;
	uint32_t attackIncrement,decayIncrement,releaseIncrement; 
	
	uint16_t sustainCV,levelCV;
	uint16_t stageLevel,stageAdd,stageMul;
	uint16_t output;

	int8_t expOutput,gate,nextGate,gateChanged;
	
	adsrStage_t stage;
};

void adsr_setCVs(struct adsr_s * adsr, uint16_t atk, uint16_t dec, uint16_t sus, uint16_t rls, uint16_t lvl);
void adsr_setGate(struct adsr_s * adsr, int8_t gate);
void adsr_setShape(struct adsr_s * adsr, int8_t isExp);

adsrStage_t adsr_getStage(struct adsr_s * adsr);
uint16_t adsr_getOutput(struct adsr_s * adsr);

void adsr_init(struct adsr_s * adsr);
void adsr_update(struct adsr_s * adsr);

#endif	/* ADSR_H */
