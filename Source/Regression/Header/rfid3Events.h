#ifndef RFID3EVENTS_H
#define RFID3EVENTS_H
void TestAccessStartEventAuto ( RFID_HANDLE32 hReader,BOOLEAN Mode );
void TestAccessStopEventAuto ( RFID_HANDLE32 hReader,BOOLEAN Mode );
void TestBufferFullWarningEventAuto ( RFID_HANDLE32 hReader,BOOLEAN Mode );
void TestBufferFullEventAuto ( RFID_HANDLE32 hReader,BOOLEAN Mode );
#endif