/*
 * xara-cairo, a vector drawing program
 *
 * Based on Xara XL, Copyright (C) 1993-2006 Xara Group Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */


// Implementation of the colour record handler class of the v2 file format

#include "camtypes.h"

#include "rechunit.h"	// UnitsRecordHandler for importing units from v2 native/web files
#include "unitcomp.h"	// Document units component, handles import of units
#include "cxftags.h"	// TAG_DEFINERGBCOLOUR TAG_DEFINECOMPLEXCOLOUR
#include "cxfunits.h"	// default export unit types


// Declare smart memory handling in Debug builds
#define new CAM_DEBUG_NEW

// An implement to match the Declare in the .h file.
CC_IMPLEMENT_DYNAMIC(UnitsRecordHandler,CamelotRecordHandler);

// #if NEW_NATIVE_FILTER	// New native filters, only available to those who need them at present

/********************************************************************************************

>	virtual BOOL UnitsRecordHandler::BeginImport()

 	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/96
	Inputs:		0
	Returns:	TRUE if ok
				FALSE otherwise
	Purpose:	Initialises the units record handler.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

BOOL UnitsRecordHandler::BeginImport()
{
	pUnitsComponent = GetUnitDocComponent();

	return (pUnitsComponent != NULL);
}

/********************************************************************************************

>	virtual UINT32* UnitsRecordHandler::GetTagList()

 	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/96
	Inputs:		-
	Returns:	Ptr to a list of tag values, terminated by CXFRH_TAG_LIST_END
	Purpose:	Provides the record handler system with a list of records handled by this
				handler
	SeeAlso:	-

********************************************************************************************/

UINT32* UnitsRecordHandler::GetTagList()
{
	static UINT32 TagList[] = {TAG_DEFINE_PREFIXUSERUNIT, TAG_DEFINE_SUFFIXUSERUNIT,
							  TAG_DEFINE_DEFAULTUNITS,
							  CXFRH_TAG_LIST_END};

	return (UINT32*)&TagList;
}

/********************************************************************************************

>	virtual BOOL UnitsRecordHandler::HandleRecord(CXaraFileRecord* pCXaraFileRecord)

 	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/96
	Inputs:		pCXaraFileRecord = ptr to record to handle
	Returns:	TRUE if handled successfuly
				FALSE otherwise
	Purpose:	Handles the given record.
	SeeAlso:	-

********************************************************************************************/

BOOL UnitsRecordHandler::HandleRecord(CXaraFileRecord* pCXaraFileRecord)
{
// WEBSTER - markn 17/12/96
// No unit record handling needed
#ifdef WEBSTER
	return TRUE;
#else
	ERROR2IF(pCXaraFileRecord == NULL,FALSE,"UnitsRecordHandler::HandleRecord pCXaraFileRecord is NULL");

	BOOL ok = TRUE;
	Document * pDoc = GetDocument();
	INT32 Tag = pCXaraFileRecord->GetTag();
	switch (Tag)
	{
		 case TAG_DEFINE_DEFAULTUNITS:
			// Ask the units document component class to import those default display units for us
			// If we are just importing data from this document into an existing one,
			// then don't import this data.
			if (IsImporting())
				break;
			if (pUnitsComponent)
				pUnitsComponent->ImportDefaultDisplayUnits(pCXaraFileRecord, pDoc);
			else
				ERROR3("UnitsRecordHandler::HandleRecord no pUnitsComponent");
			break;
		 case TAG_DEFINE_PREFIXUSERUNIT:
		 case TAG_DEFINE_SUFFIXUSERUNIT:
			// Ask the units document component class to import this user unit definition for us
			if (pUnitsComponent)
				pUnitsComponent->ImportUserUnitDefinition(pCXaraFileRecord, pDoc, Tag);
			else
				ERROR3("UnitsRecordHandler::HandleRecord no pUnitsComponent");
			break;

		default:
			ok = FALSE;
			ERROR3_PF(("UnitsRecordHandler::HandleRecord I don't handle records with the tag (%d)\n",pCXaraFileRecord->GetTag()));
			break;
	}

	return ok;
#endif // WEBSTER
}

/********************************************************************************************

>	virtual void UnitsRecordHandler::GetRecordDescriptionText(CXaraFileRecord* pRecord,StringBase* pStr)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/96
	Inputs:		pRecord = ptr to a record
				pStr = ptr to string to update
	Returns:	-
	Purpose:	This provides descriptions for the define units records.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

#ifdef XAR_TREE_DIALOG
void UnitsRecordHandler::GetRecordDescriptionText(CXaraFileRecord* pRecord, StringBase* pStr)
{
	if (pStr == NULL || pRecord == NULL)
		return;

	// Call base class first
	// This outputs the tag and size
	CamelotRecordHandler::GetRecordDescriptionText(pRecord,pStr);

	UINT32 Tag = pRecord->GetTag();
	BOOL ok = TRUE;
	TCHAR s[256];
//	INT32 RecordNumber = pRecord->GetRecordNumber();
	switch (Tag)
	{
		case TAG_DEFINE_DEFAULTUNITS:
		{
			// Read in the export unit types
			INT32 ExportPageUnits = 0L;
			INT32 ExportFontUnits = 0L;
			if (ok) ok = pRecord->ReadINT32(&ExportPageUnits);
			(*pStr) += _T("Page units: ");
			DescribeDefaultUnit(ExportPageUnits, pStr);
			(*pStr) += _T("\r\n");
			if (ok) ok = pRecord->ReadINT32(&ExportFontUnits);
			(*pStr) += _T("Font units: ");
			DescribeDefaultUnit(ExportFontUnits, pStr);
			(*pStr) += _T("\r\n");
			break;
		}

		 case TAG_DEFINE_PREFIXUSERUNIT:
		 case TAG_DEFINE_SUFFIXUSERUNIT:
		{
//			BOOL Prefix = TRUE;
			switch (Tag)
			{
				case TAG_DEFINE_PREFIXUSERUNIT:
					(*pStr) += _T("Define prefix unit: \r\n\r\n");
					break;
				case TAG_DEFINE_SUFFIXUSERUNIT:
					(*pStr) += _T("Define suffix unit: \r\n\r\n");
					break;
				default:
					ERROR3("Bad tag in ImportUserUnitDefinition");
			}
					
			// Read in the main full name of the unit
			String_32 Name;
			pRecord->ReadUnicode(&Name);//Name, Name.MaxLength());
			(*pStr) += _T("Name of unit: \t\t= ");
			(*pStr) += Name;
			(*pStr) += _T("\r\n");

			// And the abbreviation
			String_32 Abbrev;
			pRecord->ReadUnicode(&Abbrev);//Abbrev, Abbrev.MaxLength());
			(*pStr) += _T("Abbreviation for unit: \t= ");
			(*pStr) += Abbrev;
			(*pStr) += _T("\r\n\r\n");
			
			// Read in the size of this unit, 0 means based on
			double UnitSize = 0.0;
			pRecord->ReadDOUBLE(&UnitSize);
			camSprintf(s,_T("Size of unit: \t\t= %f\r\n"),UnitSize);
			(*pStr) += s;

			// Read in the exported base unit type
			INT32 ExportBaseUnit = 0L;
			pRecord->ReadINT32(&ExportBaseUnit);
			(*pStr) += _T("Based on unit: \t\t= ");
			DescribeDefaultUnit(ExportBaseUnit, pStr);
			(*pStr) += _T("\r\n");

			// Read in the multipliers for this unit
			double BaseNumerator = 0.0;
			double BaseDenominator = 0.0;
			pRecord->ReadDOUBLE(&BaseNumerator);
			pRecord->ReadDOUBLE(&BaseDenominator);
			camSprintf(s,_T("Numerator: \t\t= %f\r\n"),BaseNumerator);
			(*pStr) += s;
			camSprintf(s,_T("Denominator: \t\t= %f\r\n"),BaseDenominator);
			(*pStr) += s;

			break;
		}
	} 

	return;
}

/********************************************************************************************

>	virtual void UnitsRecordHandler::GetRecordDescriptionText(CXaraFileRecord* pRecord,StringBase* pStr)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	25/7/96
	Inputs:		ExportUnit(*pStr) += default unit to describe
				pStr = ptr to string to update
	Returns:	-
	Purpose:	Converts the ExportUnitType into a textual description.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

void UnitsRecordHandler::DescribeDefaultUnit(INT32 ExportUnitType, StringBase* pStr)
{
	TCHAR s[256];
	if (ExportUnitType < 0)
	{
		switch (ExportUnitType)
		{
			case REF_DEFAULTUNIT_MILLIMETRES:	(*pStr) += _T("MILLIMETRES");	break;
			case REF_DEFAULTUNIT_CENTIMETRES:	(*pStr) += _T("CENTIMETRES");	break;
			case REF_DEFAULTUNIT_METRES:		(*pStr) += _T("METRES");		break;
			case REF_DEFAULTUNIT_KILOMETRES:	(*pStr) += _T("KILOMETRES");	break;
			case REF_DEFAULTUNIT_MILLIPOINTS:	(*pStr) += _T("MILLIPOINTS");	break;
			case REF_DEFAULTUNIT_COMP_POINTS:	(*pStr) += _T("COMP_POINTS");	break;
			case REF_DEFAULTUNIT_PICAS:			(*pStr) += _T("PICAS");			break;
			case REF_DEFAULTUNIT_INCHES:		(*pStr) += _T("INCHES");		break;
			case REF_DEFAULTUNIT_FEET:			(*pStr) += _T("FEET");			break;
			case REF_DEFAULTUNIT_YARDS:			(*pStr) += _T("YARDS");			break;
			case REF_DEFAULTUNIT_MILES:			(*pStr) += _T("MILES");			break;
			case REF_DEFAULTUNIT_PIXELS:		(*pStr) += _T("PIXELS");		break;

			case REF_DEFAULTUNIT_NOTYPE:		(*pStr) += _T("NOTYPE");		break;
			default:
				camSprintf(s,_T("%d"),ExportUnitType);
				(*pStr) += s;
				break;
		}
	}
	else
	{
		camSprintf(s,_T("%d"),ExportUnitType);
		(*pStr) += s;
	}
}

#endif // _DEBUG

// #endif // NEW_NATIVE_FILTER

