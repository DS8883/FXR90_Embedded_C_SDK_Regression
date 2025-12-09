#ifndef CONFIG_H
#define CONFIG_H


int TestRFID_GetAntennaConfigAPI( RFID_HANDLE32 hReader );
int TestRFID_ConnectAPI(  void );
int TestRFID_DisConnectAPI( RFID_HANDLE32 hReader );
void Test_RFID_GetDllVersionInfo( RFID_HANDLE32 hReader );
void TestPhysicalAntennaProperties( RFID_HANDLE32 hReader );
void TestResetConfigToDefaults( RFID_HANDLE32 hReader );

void TestAccessAPIWithNegValues( RFID_HANDLE32 hReader );
void TestErrorInfo( RFID_HANDLE32 hReader );
void TestErrorInfoA(RFID_HANDLE32 hReader);
void TestGetLastErrorInfo( RFID_HANDLE32 hReader );
void TestSetTraceLevel( RFID_HANDLE32 hReader );

#endif