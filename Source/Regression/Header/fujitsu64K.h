#ifndef FUJITSU64K_H
#define FUJITSU64K_H

void fujitsu64KBurstWrite( );
void fujitsu64KBurstErase( );
void fujitsu64KChangeWordLock( );
void fujitsu64KChangeBlockLock( );
void fujitsu64KNeg( );
void fujitsu64KReading( ANTENNA_INFO* pInfo,TRIGGER_INFO* ptInfo );

#endif