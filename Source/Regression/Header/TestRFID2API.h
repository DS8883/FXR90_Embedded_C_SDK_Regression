
#ifndef TESTRFID2API_H
#define TESTRFID2API_H
#endif
#ifndef linux
// iNIT API2 AND DEINIT API2 ROUTINES
void initRFID2API( void );
void DeinitRFID2API( void );

// FUNTIONALITY TESTING API2 ROUTINES 
void test_SL_RFID2API( void );
void test_Session_RFID2API( void );
void test_Taget_RFID2API( void );
void test_Multiple_SelectRecords_RFID2API( void);
void test_SelectRecordIndexAPI2( void );
void test_RFID2WriteTags( void );
void testRfidAPI2GPIO( void );
void testRFID2LockMask( void );

// Test Capabilities API2 ROUTINES
void test_RFID2MemoryBankOFFSet( void );
void test_RFID2MemoryBank( void );
void DisplayRFID2Capabilities( void );
void GetRFID2Stats( void );

// Validate Boundary conditions API2 ROUTINES
void ValidateRFID2LockAction( void );
void ValidateRFID2AccessPassword( void );
void ValidateRFID2KillPassword( void );
void ValidateRFID2SelectAction( void );
void ValidateRFID2ReadMode( void );	
void testRFID2Capabilities( void );
#endif