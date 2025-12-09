#ifndef APITEST_H
#define APITEST_H


// thread safety and stress tests 
void TestThreadSafeSingleReader(  );
void StressConnectDisConnectRead( DWORD CycleCount );
void StressRMAPI( DWORD CycleCount );
void StressConfigAPI( DWORD CycleCount );
void StressInventory(  );

// interface definitions..for Automation.
void TestRFIDStateUnAware( );
void StateunAwareWithTagevents( );
void TestRFIDStateAware( );
void TestOpSeqNeg( );
void TestingEvents( );
void TestingTriggers( );
void TestRFID3ConfigParameters( );
void VerifyRMProfileTimeZoneAPI( );
void ValidaterRFID3rmapi( );
void TestRFID2API( );
void TestG2Truncate( );
void TestWriteSpecificAPI();
void TestSimpleReadWriteErase( );
void TestOpSequenceReadWriteErase( );
void Test_PostFiltersRead( void );
void Test_PostFiltersReadAuto( void );
void TestRFIndex( );
void TestRSSIFilter( );
void TestReadAllMemoryBanks();
void VerifyDutyCycle( );
void VerifyLocationingSanity( );
void Fujitsu64KTest( );
void ImpinjTest( );
void nxpTagTest( );
void TestAccessFilterAPI();
void Monza5TagFocus();
void Monza5FastID();
void Fx7500PhaseInfo();
void Fx7500AntennaRFConfig();
void Fx7500PeriodicReportTrigger();
void Fx7500RMAPI();
void Fx7500AbFlipSLAll( );
void VerifyRegionConfigAPI();
void TestNXPBrandID();
void TestAntennaConfig();
void TestG2V2Features();
void TestLICManager();
void rfid3UnSupportedFeature();
//void TestStateOWareSingulation( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo,TRIGGER_INFO* );

// Manual Cases..
int TestRFID3EventsManual( RFID_HANDLE32 hReader ,ANTENNA_INFO* pInfo );
void TestLockOperations( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo );
void TestLockOperationsOnResMemory( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo );
void TestPermaLockANDPermaUNLock( RFID_HANDLE32 hReader,UINT8* pattern,ANTENNA_INFO* pInfo );
void TestRFID3KillTag( RFID_HANDLE32 hReader,UINT8* pattern,ANTENNA_INFO* pInfo );
void TestThreadSafe( void );
void TestThreadSafeRMAPI( void );
int TestRFID3AcceptConnection( UINT32 Port  );
void SetEAS_Functional(RFID_HANDLE32 hreader);
void SetEAS_Negative(RFID_HANDLE32 hreader);
void SetQuiet(RFID_HANDLE32 hreader);
void SetQuiet_Negative(RFID_HANDLE32 hreader);
void GetReaderStats(RFID_HANDLE32 hreader);

// Mislaneous definitions
void TestRFID3EventsAutoMode( RFID_HANDLE32 hReader );
int TestInventroyWithTriggers( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo );
BOOLEAN TestInventroyWithTriggersAutoMode( RFID_HANDLE32 hReader,ANTENNA_INFO* pInfo );
void TestValidateParameters( RFID_HANDLE32 hReader );
void addanWareSelectRecord( RFID_HANDLE32 hReader, UINT8* pattern,MEMORY_BANK mBank,UINT16 bitoffset,UINT8 patternbitCount,FILTER_ACTION fAction,STATE_UNAWARE_ACTION selectParam );

#endif