////////////////////////////////////////////////////////////////////////////////
// Keys and buttons scanner
////////////////////////////////////////////////////////////////////////////////

#include "scanner.h"

#define SCANNER_BYTES 16
#define SCANNER_KEYS_START 64

static struct
{
	uint8_t stateBits[SCANNER_BYTES];
} scanner;

void scanner_init(void)
{
	memset(&scanner,0,sizeof(scanner));
}

static inline int scanner_state(uint8_t key)
{
	return (scanner.stateBits[key>>3]&(1<<(key&7)))!=0;
}

int8_t inline scanner_keyState(uint8_t key)
{
	return scanner_state(key+SCANNER_KEYS_START);
}

int8_t inline scanner_buttonState(p600Button_t button)
{
	return scanner_state(button);
}

static inline void scanner_event(uint8_t key, int8_t pressed)
{
	if (key<SCANNER_KEYS_START)
		p600_buttonEvent(key,pressed);
	else
		p600_keyEvent(key-SCANNER_KEYS_START,pressed);
}

#define DO_ONE_SCAN(x) if(pa&1) scanner_event(i*8+(x),ps&1); pa>>=1; ps>>=1;

void scanner_update(void)
{
	uint8_t i;
	uint8_t ps=0,pa;		

	BLOCK_INT
	{
		// prepare first iteration
		io_write(0x08,0);
		CYCLE_WAIT(16);
		
		for(i=0;i<SCANNER_BYTES;++i)
		{
			ps=io_read(0x0a);
			
			// prepare next iteration
			io_write(0x08,i+1);

			pa=ps^scanner.stateBits[i];
			scanner.stateBits[i]=ps;

			if(pa)
			{
				DO_ONE_SCAN(0);
				DO_ONE_SCAN(1);
				DO_ONE_SCAN(2);
				DO_ONE_SCAN(3);
				DO_ONE_SCAN(4);
				DO_ONE_SCAN(5);
				DO_ONE_SCAN(6);
				DO_ONE_SCAN(7);
			}
		}
	}
}
	
