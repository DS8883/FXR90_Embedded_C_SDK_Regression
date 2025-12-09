#include "ManualTests.h"


// Initialize the Region List with Info such as Region Name, Standard Name, Number of Freq Channels availablee, 
// 
BOOLEAN initRegionList (RegionInfo *pInfo )
{
	for( int RegionCount = 0;RegionCount < MAX_COUNTRY_LIST;RegionCount++ )
	{
		switch( RegionCount )
		{
			case 0:
				wcscpy(pInfo[0].RegionName,L"Argentina");
				wcscpy(pInfo[0].StandardName, L"Argentina");
				pInfo[0].bEURegion = FALSE;
				break;
			case 1:
				wcscpy(pInfo[1].RegionName, L"Australia");
				wcscpy(pInfo[1].StandardName, L"Australia");
				pInfo[1].bEURegion = FALSE;
				break;
			case 2:
				wcscpy(pInfo[2].RegionName, L"Brazil");
				wcscpy(pInfo[2].StandardName, L"Brazil");
				pInfo[2].bEURegion = FALSE;
				break;
			case 3:
				wcscpy(pInfo[3].RegionName, L"Canada");
				wcscpy(pInfo[3].StandardName, L"Canada");
				pInfo[3].bEURegion = FALSE;
				break;
			case 4:
				wcscpy(pInfo[4].RegionName, L"Chile");
				wcscpy(pInfo[4].StandardName, L"Chile");
				pInfo[4].bEURegion = FALSE;
				break;
			case 5:
				wcscpy(pInfo[6].RegionName, L"Colombia");
				wcscpy(pInfo[6].StandardName, L"Colombia");
				pInfo[6].bEURegion = FALSE;
				break;
			case 6:
				wcscpy(pInfo[7].RegionName, L"European Union");
				wcscpy(pInfo[7].StandardName, L"EU 302.208");
				pInfo[7].bEURegion = TRUE;
				break;
			case 7:
				wcscpy(pInfo[8].RegionName, L"Hong Kong");
				wcscpy(pInfo[8].StandardName, L"Hong Kong-A");
				pInfo[8].bEURegion = FALSE;
				break;
			case 8:
				wcscpy(pInfo[9].RegionName, L"Hong Kong");
				wcscpy(pInfo[9].StandardName, L"Hong Kong-B");
				pInfo[9].bEURegion = FALSE;
				break;
			case 9:
				wcscpy(pInfo[10].RegionName, L"India");
				wcscpy(pInfo[10].StandardName, L"India");
				pInfo[10].bEURegion = TRUE;
				break;
			case 10:
				wcscpy(pInfo[11].RegionName, L"Korea");
				wcscpy(pInfo[11].StandardName, L"Korea");
				pInfo[11].bEURegion = FALSE;
				break;
			case 11:
				wcscpy(pInfo[12].RegionName, L"Malaysia");
				wcscpy(pInfo[12].StandardName, L"Malaysia");
				pInfo[12].bEURegion = FALSE;
				break;
			case 12:
				wcscpy(pInfo[13].RegionName, L"Mexico");
				wcscpy(pInfo[13].StandardName, L"Mexico");
				pInfo[13].bEURegion = FALSE;
				break;
			case 13:
				wcscpy(pInfo[14].RegionName, L"New Zealand");
				wcscpy(pInfo[14].StandardName, L"New Zealand");
				pInfo[14].bEURegion = TRUE;
				break;
			case 14:
				wcscpy(pInfo[15].RegionName, L"Peru");
				wcscpy(pInfo[15].StandardName, L"Peru");
				pInfo[15].bEURegion = FALSE;
				break;
			case 15:
				wcscpy(pInfo[16].RegionName, L"Russia");
				wcscpy(pInfo[16].StandardName, L"Russia");
				pInfo[16].bEURegion = TRUE;
				break;
			case 16:
				wcscpy(pInfo[17].RegionName, L"Saudi Arabia");
				wcscpy(pInfo[17].StandardName, L"Saudi Arabia");
				pInfo[17].bEURegion = TRUE;
				break;
			case 17:
				wcscpy(pInfo[18].RegionName, L"Singapore");
				wcscpy(pInfo[18].StandardName, L"Singapore");
				pInfo[18].bEURegion = FALSE;
				break;
			case 18:
				wcscpy(pInfo[19].RegionName, L"South Africa");
				wcscpy(pInfo[19].StandardName, L"South Africa");
				pInfo[19].bEURegion = TRUE;
				break;
			case 19:
				wcscpy(pInfo[20].RegionName, L"Taiwan");
				wcscpy(pInfo[20].StandardName, L"Taiwan");
				pInfo[20].bEURegion = FALSE;
				break;
			case 20:
				wcscpy(pInfo[21].RegionName, L"Thailand");
				wcscpy(pInfo[21].StandardName, L"Thailand");
				pInfo[21].bEURegion = FALSE;
				break;
			case 21:
				wcscpy(pInfo[22].RegionName, L"Turkey");
				wcscpy(pInfo[22].StandardName, L"Turkey");
				pInfo[22].bEURegion = TRUE;
				break;
			case 22:
				wcscpy(pInfo[23].RegionName, L"UAE");
				wcscpy(pInfo[23].StandardName, L"UAE");
				pInfo[23].bEURegion = TRUE;
				break;
			case 23:
				wcscpy(pInfo[23].RegionName, L"Venezuela");
				wcscpy(pInfo[23].StandardName, L"Venezuela");
				pInfo[23].bEURegion = FALSE;
				break;
			default:
					break;
		}
	}

	return TRUE;
}
