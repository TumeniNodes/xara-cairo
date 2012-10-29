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


/********************************************************************************************

	menucmds.cpp - This module provides a set of menu command functions which are called from
	OIL Menu Module when an appropriate menu item is selected. 

********************************************************************************************/


#include "camtypes.h" 
//#include <afxext.h>

#include <wx/mediactrl.h>

#include "menucmds.h"
#include "camelot.h"
#include "camframe.h"
#include "camdoc.h"
//#include "camvw.h"
#include "printing.h"
#include "helpuser.h"
#include "menuops.h"
#include "camview.h"
//#ifdef WEBSTER
//#include "ollie.h"
//#include "resource.h"
//#include "simon.h"
//#include "registry.h"
#include "product.h"
//#include "resimmap.h"
#include "fileutil.h"
#include "sgliboil.h"
//#endif //webster
#include "rechblnd.h"		// so that can reset blend pointers
//#include "urls.h"
#include "filedlgs.h"
#include "tmplmngr.h"

#define THIRTY_TWO 32

/********************************************************************************************

>	void ExitAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Exit application menu function.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()
	SeeAlso:	State()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object.This
	is used cast to a Camelot Application object so that camelot the camelot menu functions 
	can be used.
********************************************************************************************/

void ExitAction() 
{               
	AfxGetApp().OnAppExit();
}
 
 
/********************************************************************************************

>	void FileNewAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs New File menu function on the File menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object.This
	is used cast to a Camelot Application object so that camelot the camelot menu functions 
	can be used.
********************************************************************************************/

void FileNewAction() 
{               
	//First tell the document system that the next template to use is the
	//default animation template
	CCamDoc::SetNextTemplateToUse( CTemplateManager::GetDefaultDrawingTemplate() );

	// OnFileOpen needs an event, but doesn't use it
	wxCommandEvent event;
	AfxGetApp().GetDocumentManager()->OnFileNew( event );

	CCamDoc::EnableRemoveUntouchedDocs();		// Next idle event will try ro get rid of 
												// the auto-created startup document
}

/********************************************************************************************

>	void FileNewAnimationAction(); 	

	Author:		Graham_Walmsley (Xara Group Ltd) <camelotdev@xara.com>
	Created:	21/10/97
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	Opens the animation template
	Errors:		-
	SeeAlso:	MenuCmds

********************************************************************************************/

void FileNewAnimationAction() 
{               
	//First tell the document system that the next template to use is the
	//default animation template
	CCamDoc::SetNextTemplateToUse( CTemplateManager::GetDefaultAnimationTemplate() );

PORTNOTETRACE("other", "FileNewAnimationAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	AfxGetApp().OnFileNew();
#endif

	CCamDoc::EnableRemoveUntouchedDocs();		// Next idle event will try ro get rid of 
												// the auto-created startup document
}

/********************************************************************************************

>	void FileNewTemplateAction(INT32 iNumberOfTemplate) 

	Author:		Graham_Walmsley (Xara Group Ltd) <camelotdev@xara.com>
	Created:	21/10/97
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	Opens the specified template
	Errors:		-
	SeeAlso:	MenuCmds

********************************************************************************************/

void FileNewTemplateAction(INT32 iNumberOfTemplate) 
{               
	//First find the default template path
	CTemplateManager&	TemplateManager = GetApplication()->GetTemplateManager();
	String_256 			strPathOfFile;
	if( !TemplateManager.GetTemplateFilename( iNumberOfTemplate, &strPathOfFile ) )
		return;

	TRACEUSER( "jlh92", _T("Opening %s\n"), PCTSTR(strPathOfFile) );

	CCamDoc::SetNextTemplateToUse( strPathOfFile );

	// OnFileOpen needs an event, but doesn't use it
	wxCommandEvent event;
	AfxGetApp().GetDocumentManager()->OnFileNew( event );

	CCamDoc::EnableRemoveUntouchedDocs();		// Next idle event will try ro get rid of 
												// the auto-created startup document
}


 
/********************************************************************************************

>	void FileOpenAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs File Open menu function on the File menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object.This
	is used cast to a Camelot Application object so that camelot the camelot menu functions 
	can be used.
********************************************************************************************/

void FileOpenAction() 
{
	// OnFileOpen needs an event, but doesn't use it
	wxCommandEvent		event;
//	AfxGetApp().GetDocumentManager()->SetLastDirectory(BaseFileDialog::DefaultOpenFilePath);
//	AfxGetApp().GetDocumentManager()->OnFileOpen( event );
	AfxGetApp().OnFileOpen();
	CCamDoc::EnableRemoveUntouchedDocs();		// Next idle event will try ro get rid of 
												// the auto-created startup document
}
 
/********************************************************************************************

>	void FileCloseAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs File Close menu function on the File menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The MDIGetActive() function is a member func-
	tion of the CMDIFrameWnd class and is used to get the currently active child window. 
	From the Child window the GetActiveDocument() function will obtain the currently active
	document. The OnFileClose() function is then applied to the currently active document 
	which is incidently cast to a Camelot Document before this call made.
********************************************************************************************/

void FileCloseAction() 
{               
 	// CGS:  we need to clear out all intermediate blend steps ....
	// NOTE:  this clears out all 'built-up' intermediate data
	// it clears out the becomea stuff

	NodeGroup::KillAllBlendBecomeAConsLists ();

	// CGS:  these pointers MUST be reset! ....
		
	BlendRecordHandler::SetLastInsertedNodeBlender (NULL);
	BlendRecordHandler::SetLastInsertedNodeBlend (NULL);
	BlendRecordHandler::SetLastInsertedNodeBlendPath (NULL);
	
	// OnFileClose needs an event, but doesn't use it
	wxCommandEvent		event;
	AfxGetApp().GetDocumentManager()->OnFileClose( event );
}
 
/********************************************************************************************

>	void FileSaveAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs File Save menu function on the File menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The MDIGetActive() function is a member func-
	tion of the CMDIFrameWnd class and is used to get the currently active child window. 
	From the Child window the GetActiveDocument() function will obtain the currently active
	document. The OnFileSave() function is then applied to the currently active document 
	which is incidently cast to a Camelot Document before this call made.
********************************************************************************************/

void FileSaveAction() 
{               
	wxCommandEvent		event;
//	AfxGetApp().GetDocumentManager()->SetLastDirectory(BaseFileDialog::DefaultSaveFilePath);
	AfxGetApp().GetDocumentManager()->OnFileSave( event );
}
 
/********************************************************************************************

>	void FileSaveAsAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs File SaveAs menu function on the File menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The MDIGetActive() function is a member func-
	tion of the CMDIFrameWnd class and is used to get the currently active child window. 
	From the Child window the GetActiveDocument() function will obtain the currently active
	document. The OnFileSaveAs() function is then applied to the currently active document 
	which is incidently cast to a Camelot Document before this call made.
********************************************************************************************/

void FileSaveAsAction() 
{               
//	AfxGetApp().GetDocumentManager()->SetLastDirectory(BaseFileDialog::DefaultSaveFilePath);

	// Don't call wxDocManager::OnFileSaveAs because that calls wxDocument non-virtual functions
	// We need to ensure our pointer is cast to a CCamDoc*
	// Note also that GetCurrentDocument doesn't mean "Current" in the way that the Kernel defines it!!!
	//	(It means "Active"...)
//	wxCommandEvent		event;
//	AfxGetApp().GetDocumentManager()->OnFileSaveAs( event );

	wxDocument* pDoc = AfxGetApp().GetDocumentManager()->GetCurrentDocument();
	if (pDoc && pDoc->IsKindOf(CLASSINFO(CCamDoc)))
		((CCamDoc*)pDoc)->SaveAs();
}


/********************************************************************************************

>	void FileSaveAllAction()

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/7/95
	Purpose:	Performs the File Save All Action in a Kernel / oil compliant way

********************************************************************************************/

void FileSaveAllAction()
{
	// We do not want to call our version of this function as for some reason
	// it also shuts down the dialog manager...
PORTNOTETRACE("other", "FileSaveAllAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	((CCamApp*)AfxGetApp())->RealSaveAll();
#endif
}


/********************************************************************************************

>	void PrintSetupAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Printer Setup menu function on the File menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object.This
	is used cast to a Camelot Application object so that camelot the camelot menu functions 
	can be used.
********************************************************************************************/
//	WEBSTER-ranbirr-12/11/96
#ifndef WEBSTER
void PrintSetupAction() 
{               
	AfxGetApp().OnFilePrintSetup();
}
#endif //webster
/********************************************************************************************

>	void PrintAction()

	Author:		Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	16/7/93
	Inputs:		None
	Outputs:	void for handled or not
	Returns:	None
	Purpose:	Stop MFC menu items from doing things that don't work (e.g. Print). For
				now calls the Print operation (which ENSURE's).
	Errors:		None

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The MDIGetActive() function is a member func-
	tion of the CMDIFrameWnd class and is used to get the currently active child window. 
	From the Child window the GetActiveView() function will obtain the currently active
	view. The OnFilePrint() function is then applied to the currently active view 
	which is incidently cast to a Camelot View before this call made.
********************************************************************************************/
//	WEBSTER-ranbirr-12/11/96
#ifndef WEBSTER
void PrintAction() 
{               
#if !defined(STANDALONE)
	wxView * pView = AfxGetApp().GetDocumentManager()->GetCurrentView();
	if (pView && pView->IsKindOf(CLASSINFO(CCamView)))
		((CCamView*)pView)->OnFilePrint();

#endif
}
#endif //webster
/********************************************************************************************

>	void PrintPreviewAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs File Print Preview menu function on the File menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The MDIGetActive() function is a member func-
	tion of the CMDIFrameWnd class and is used to get the currently active child window. 
	From the Child window the GetActiveView() function will obtain the currently active
	view. The OnFilePrintPreview() function is then applied to the currently active view 
	which is incidently cast to a Camelot View before this call made.
********************************************************************************************/

void PrintPreviewAction() 
{               
	wxView * pView = AfxGetApp().GetDocumentManager()->GetCurrentView();
	if (pView && pView->IsKindOf(CLASSINFO(CCamView)))
		((CCamView*)pView)->OnFilePrintPreview();
}
 
/********************************************************************************************

>	void WindowNewAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs New Window menu function on the Window menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The OnWindowNew() function is then applied to 
	the mainframe window of the currently active application.
********************************************************************************************/

void WindowNewAction() 
{               
PORTNOTETRACE("other", "WindowNewAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
 	((CMainFrame*)((AfxGetApp())->m_pMainWnd))->OnWindowNew();
#endif
}

/********************************************************************************************

>	void WindowArrangeAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Window menu function Arrange.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

void WindowArrangeAction() 
{               
PORTNOTETRACE("other", "WindowArrangeAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
 	((CMainFrame*)((AfxGetApp())->m_pMainWnd))->OnMDIWindowCmd(_R(ID_WINDOW_ARRANGE));
#endif
}

/********************************************************************************************

>	void WindowCascadeAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Window menu function Cascade.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

void WindowCascadeAction() 
{               
PORTNOTETRACE("other", "WindowCascadeAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
 	((CMainFrame*)((AfxGetApp())->m_pMainWnd))->OnMDIWindowCmd(_R(ID_WINDOW_CASCADE));
#endif
}

/********************************************************************************************

>	void WindowTileHorzAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Window menu function Tile Horizontal.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

void WindowTileHorzAction() 
{               
PORTNOTETRACE("other", "WindowTileHorzAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
 	((CMainFrame*)((AfxGetApp())->m_pMainWnd))->OnMDIWindowCmd(_R(ID_WINDOW_TILE_HORZ));
#endif
}

/********************************************************************************************

>	void WindowTileVertAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Window menu function Tile Verticle.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

void WindowTileVertAction() 
{               
PORTNOTETRACE("other", "WindowTileVertAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
 	((CMainFrame*)((AfxGetApp())->m_pMainWnd))->OnMDIWindowCmd(_R(ID_WINDOW_TILE_VERT));
#endif
}


/********************************************************************************************

>	void WindowTileVertAction(); 	

	Author:		Luke_Hart (Xara Group Ltd) <lukehcamelotdev@xara.com>
	Created:	01/08/06
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Window menu function switch next\last document.
	Errors:		None
	SeeAlso:	MenuCmds

********************************************************************************************/

void WindowNextDcocument( bool fForward )
{
// This is only needed for Linux (and maybe Mac, we'll see)
#if defined(__WXGTK__)
	CCamFrame*			pFrame = (CCamFrame*)AfxGetApp().GetTopWindow();
	if( fForward )
		pFrame->ActivateNext();
	else
		pFrame->ActivatePrevious();
#endif
}


/********************************************************************************************

>	OpState WindowCmdState()

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Outputs:	None
	Returns:	None
	Purpose:	Updates the state of the window menu options.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

OpState WindowCmdState()
{
PORTNOTETRACE("other", "WindowCmdState does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
    // Get Active Child Window                                                 
    if (((CMDIFrameWnd *) (AfxGetApp())->m_pMainWnd)->MDIGetActive() != NULL)                                               
#endif
		return OpState( FALSE, FALSE );					// not ticked, not greyed
#if !defined(EXCLUDE_FROM_XARALX)
	else
	 	return OpState( FALSE, TRUE );					// not ticked, greyed
#endif
} 

/********************************************************************************************

>	void HelpIndexAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Help Index menu function on the Help menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object.This
	is used cast to a Camelot Application object so that camelot the camelot menu functions 
	can be used.
********************************************************************************************/

void HelpIndexAction() 
{
	AfxGetApp().OnHelpIndex();
}


 
/********************************************************************************************

>	void HelpUsingAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs Help Using menu function on the Help menu.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object.This
	is used cast to a Camelot Application object so that camelot the camelot menu functions 
	can be used.
********************************************************************************************/

void HelpUsingAction() 
{               
PORTNOTETRACE("other", "HelpUsingAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
#if (_MFC_VER >= 0x250)
	GetMainFrame()->OnHelpUsing();
#else
	((CCamApp*) AfxGetApp())->OnHelpUsing();
#endif
#endif
}



void HelpToolsAction() 
{               
	HelpUsingTools();
}



void HelpGalleriesAction()
{
	HelpUsingGalleries();
}

static void StartMovie( const wxString &strFile )
{
	wxString			strDataPath( (wxChar*)CCamApp::GetResourceDirectory() );
	if( !wxDir::Exists( strDataPath ) )
	{
#if defined(_DEBUG)
		// We'll try default location under debug to make life easier
		strDataPath = _T("/usr/share");
#endif
	}

	wxString			strVideoPath( strDataPath );
	strVideoPath += _("/video");

	wxString			strCommand; // ( _T("mplayer -slave \"") );
	strCommand += strVideoPath + _T("/") + strFile + _T("\"");	

#if true
	CCamApp::LaunchMediaApp( strCommand );
#else
	long /*TYPENOTE: CORRECT*/ lResult = wxExecute( strCommand, wxEXEC_SYNC, NULL );
	if( 255 == lResult )
	{
		strCommand = strDataPath + _T("/xaralx/bin/mplayer -slave \"");
		strCommand += strVideoPath + _T("/") + strFile + _T("\"");

		lResult = wxExecute( strCommand, wxEXEC_SYNC, NULL );
		if( 255 == lResult )
			InformWarning( _R(IDS_MPLAYER_MISSING), _R(IDS_OK) );
	}
#endif
}

void HelpDemosAction()
{
	StartMovie( _T("Part_1_master_inc_audio_smaller_q35_fr15_hi.ogm") );

//	HelpOnlineDemos();
}

void HelpPlayerAction()
{
	CCamApp::SelectMediaApp();
}

void HelpTechSupportAction()
{
	HelpTechnicalSupport();
}


#ifdef STANDALONE
/********************************************************************************************

>	void HelpSpecAction(); 	

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/5/95
	Purpose:	Performs Help Spec menu function on the Help menu.
	SeeAlso:	MenuCmds

********************************************************************************************/

void HelpSpecAction() 
{
	ShowHelpSpec();
}
#endif

 
 
/********************************************************************************************

>	void ViewToolBarAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs View menu function to update ToolBar.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The OnBarCheck() function is then applied to 
	the mainframe window of the currently active application.
********************************************************************************************/

void ViewToolBarAction() 
{
PORTNOTETRACE("other", "ViewToolBarAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	((CMainFrame*)((AfxGetApp())->m_pMainWnd))->OnBarCheck(_R(ID_VIEW_TOOLBAR));
#endif
}

/********************************************************************************************

>	void ViewStatusBarAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs View menu function to update StatusBar.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/
/*
Technical Notes:
	The AfxGetApp() function is used to obtain the currently active application object. The 
	m_pMainWnd object is a member variable of a CWinApp class which contains the main frame
	window of the current application object. The OnBarCheck() function is then applied to 
	the mainframe window of the currently active application.
********************************************************************************************/

void ViewStatusBarAction() 
{
PORTNOTETRACE("other", "ViewStatusBarAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	GetMainFrame()->ShowStatusBar(!GetMainFrame()->IsStatusBarVisible());
#endif
}

/********************************************************************************************

>	void ViewRulersAction(); 	

	Author:		Chris_Parks (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs View menu function to update Rulers.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

void ViewRulersAction() 
{
	// Get the currently selected view pointer. (can be NULL if no documents!)
	DocView *pView = DocView::GetSelected();
	
	if (pView)
	{
		BOOL NewState = ! pView->AreRulersVisible();
		pView->ShowViewRulers(NewState);
		CCamView::SetDefaultRulersState(NewState);
	}
}
/********************************************************************************************

>	OpState ViewToolBarState()

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Outputs:	None
	Returns:	None
	Purpose:	Updates the state of the ToolBar menu option.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

OpState ViewToolBarState()
{
PORTNOTETRACE("other", "ViewToolBarState does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	CWnd* pBar = ((CMainFrame*)((AfxGetApp())->m_pMainWnd)
									)->GetDescendantWindow(_R(AFX_IDW_TOOLBAR));
	
	if ((pBar != NULL) && ((pBar->GetStyle() & WS_VISIBLE) != 0))
#endif
		 return OpState( TRUE, FALSE );					// not ticked, not greyed
#if !defined(EXCLUDE_FROM_XARALX)
	else
	 	return OpState( FALSE, FALSE );					// ticked, not greyed
#endif
} 

/********************************************************************************************

>	OpState ViewStatusBarState(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Outputs:	None
	Returns:	None
	Purpose:	Updates the state of the ToolBar menu option.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

OpState ViewStatusBarState()
{
PORTNOTETRACE("other", "ViewStatusBarState does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	if(GetMainFrame()->IsStatusBarVisible())
#endif
		return OpState( TRUE, FALSE );				// not ticked, not greyed
#if !defined(EXCLUDE_FROM_XARALX)
	else  
		return OpState( FALSE, FALSE );					// ticked, not greyed
#endif
}

/********************************************************************************************

>	OpState ViewColourBarState(); 	

	Author:		Chris_Parks (Xara Group Ltd) <camelotdev@xara.com>
	Created:	7/10/94
	Outputs:	None
	Returns:	None
	Purpose:	Updates the state of the ColourBar menu option.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

OpState ViewColourBarState()
{
PORTNOTETRACE("other", "ViewColourBarState does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	if(GetMainFrame()->IsColourBarVisible())
#endif
		return OpState( TRUE, FALSE );				// not ticked, not greyed
#if !defined(EXCLUDE_FROM_XARALX)
	else
	 	return OpState( FALSE, FALSE );					// ticked, not greyed
#endif	
}

/********************************************************************************************

>	OpState ViewRulersState(); 	

	Author:		Chris_Parks (Xara Group Ltd) <camelotdev@xara.com>
	Created:	7/10/94
	Outputs:	None
	Returns:	None
	Purpose:	Updates the state of the Rulers menu option.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

OpState ViewRulersState()
{
	// Get the currently selected view pointer. (can be NULL if no documents!)
	DocView *pView = DocView::GetSelected();
	if (pView == NULL)
		return OpState( FALSE, TRUE );				// not ticked, greyed;

	if(pView->AreRulersVisible())
		 return OpState( TRUE, FALSE );				// ticked, not greyed
	else
	 	return OpState( FALSE, FALSE );				// not ticked, not greyed
	
}

/********************************************************************************************

>	OpState ViewFullScreenState(); 	

	Author:		Chris_Parks (Xara Group Ltd) <camelotdev@xara.com>
	Created:	7/10/94
	Outputs:	None
	Returns:	None
	Purpose:	Updates the state of the FullScreen menu option.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

OpState ViewFullScreenState()
{			
PORTNOTETRACE("other", "ViewFullScreenState does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	if(GetMainFrame()->IsFullScreenMode())
#endif
		return OpState( TRUE, FALSE );				// not ticked, not greyed
#if !defined(EXCLUDE_FROM_XARALX)
	else
	 	return OpState( FALSE, FALSE );					// ticked, not greyed
#endif	
}

/********************************************************************************************

>	OpState ViewScrollBarsState(); 	

	Author:		Chris_Parks (Xara Group Ltd) <camelotdev@xara.com>
	Created:	7/10/94
	Outputs:	None
	Returns:	None
	Purpose:	Updates the state of the ScrollBars menu option.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

OpState ViewScrollBarsState()
{			
	// Get the currently selected view pointer. (can be NULL if no documents!)
	DocView *pView = DocView::GetSelected();
	if (pView == NULL)
		return OpState( FALSE, TRUE );				// not ticked, greyed;

	if(pView->AreScrollersVisible())
		 return OpState( TRUE, FALSE );				// ticked, not greyed
	else
	 	return OpState( FALSE, FALSE );				// not ticked, not greyed
	
	/* OLD Global version
	if(GetMainFrame()->AreScrollBarsVisible())
		 return OpState( TRUE, FALSE );				// not ticked, not greyed
	else  
	 	return OpState( FALSE, FALSE );					// ticked, not greyed
	*/
}

/********************************************************************************************

>	void ViewColourBarAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs View menu function to update Colour Bar.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

void ViewColourBarAction() 
{
PORTNOTETRACE("other", "ViewColourBarAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	GetMainFrame()->ShowColourBar(!GetMainFrame()->IsColourBarVisible());
#endif	
}

/********************************************************************************************

>	void ViewScrollBarsAction(); 	

	Author:		Mario_Shamtani (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/7/93
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs View menu function to update Scroll Bars.
	Errors:		None
	SeeAlso:	MenuCmds
	SeeAlso:	SetupDefaultMenu()
	SeeAlso:	SetupSharedMenu()

********************************************************************************************/

void ViewScrollBarsAction() 
{
PORTNOTETRACE("other", "ViewScrollBarsAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	// Get the currently selected view pointer. (can be NULL if no documents!)
	DocView *pView = DocView::GetSelected();
	if (pView)
	{
		BOOL NewState = ! pView->AreScrollersVisible();
		pView->ShowViewScrollers(NewState);
		ScreenCamView::SetDefaultScrollersState(NewState);
	}

	// OLD Global version			
	//  GetMainFrame()->ShowScrollBars(!GetMainFrame()->AreScrollBarsVisible());
#endif
}


/********************************************************************************************

>	void ViewFullScreenAction(); 	

	Author:		Chris_Parks (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/10/94
	Inputs:		None
	Outputs:	None
	Returns:	None
	Purpose:	Performs View menu function to Toggle Full Screen mode.
				Full Screen Mode is a bit misleading.. this function actually toggles mode
				by writing/loading either clean.con or normal.con files - all bars except the infobar
				are destroyed and recreated in there new state after the load.The Infobar is a 
				special case this is just moved after the load.  
	Errors:		None
	SeeAlso:	MenuCmds

********************************************************************************************/

void ViewFullScreenAction() 
{
PORTNOTETRACE("other", "ViewFullScreenAction does nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	BOOL NoError = TRUE;
	// lock the window to avoid untidy layout recalcs
	::LockWindowUpdate(GetMainFrame()->GetSafeHwnd());
	
	// tell the mainframe we are changing modes - this prevents
	// some unnecessary work
	GetMainFrame()->SetChangingMode(TRUE);

	// save current bars setup in clean/normal ini file
	if(GetMainFrame()->IsFullScreenMode())
		NoError = DialogBarOp::WriteNamedBars("clean");
	else
		NoError = DialogBarOp::WriteNamedBars("normal");

	if(!NoError)
		InformError();
	// kill all bars ( except infobar at the mo..)
	BROADCAST_TO_CLASS(DialogMsg(NULL, DIM_BAR_DEATH, NULL ),DialogBarOp);
	
	// hide the info bar
	InformationBarOp::SetVisibility(FALSE,TRUE);
	
	// load clean/normal bars ini file
	if(GetMainFrame()->IsFullScreenMode())
		DialogBarOp::LoadNamedBars("normal");
	else
		DialogBarOp::LoadNamedBars("clean");
	
	// force the info bar to the correct visible state
	InformationBarOp::SetVisibility(InformationBarOp::IsVisible(),TRUE);
	
	// All clear to Mainframe
	GetMainFrame()->SetChangingMode(FALSE);
	
	// Make sure everything is positioned correctly
	GetMainFrame()->RecalcLayout();
	GetMainFrame()->RelocateToolbar ();
	
	// and unlock and show the window
	::LockWindowUpdate(NULL);

	// Update all button controls that invoke this Op
	OpDescriptor* pOpDesc = OpDescriptor::FindOpDescriptor(OPTOKEN_VIEWFULLSCREEN);
	if (pOpDesc!=NULL)
	{
		// Found the opdescriptor. Now find all the gadgets associated with it
		List Gadgets;
		if (pOpDesc->BuildGadgetList(&Gadgets))
		{
			// Found some. Set the controls accordingly
			GadgetListItem* pGadgetItem = (GadgetListItem*) Gadgets.GetHead();

			while (pGadgetItem != NULL)
			{
				// Set the gadget
				pGadgetItem->pDialogBarOp->SetBoolGadgetSelected(pGadgetItem->gidGadgetID,
																	GetMainFrame()->IsFullScreenMode());
				// Find the next gadget
				pGadgetItem = (GadgetListItem*) Gadgets.GetNext(pGadgetItem);
			}
	
			// Clean out the list
			Gadgets.DeleteAll();
		}
	}
#endif
}


/*********************************************************************************************************************
/// Website related help items
***********************************************************************************************************************/


//	WEBSTER-ranbirr-12/11/96
//#ifdef WEBSTER


/*********************************************************************************************************************
	void HelpXaraForumAction() 

	Author:		Alex
	Created:	19/02/2004
	Inputs:		-
	Outputs:	-
	Returns		-
	Purpose:	Provides access from a help menu to XaraX's Forum Page (e.g. Talkgraphics)
	Errors		If the browser could not be opened, an Error message box is diplayed.
	See Also	-

***********************************************************************************************************************/

void HelpXaraForumAction()
{
	wxString strURL = CamResource::GetText(_R(IDS_URL_FORUM));
	CCamApp::LaunchWebBrowser(strURL);
}


/*********************************************************************************************************************
	void HelpWebsterHomePage() 

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Inputs:		-
	Outputs:	-
	Returns		-
	Purpose:	Provides access from a help menu to Webster's Home Page.
	Errors		If the browser could not be opened, an Error message box is diplayed.
	See Also	-

***********************************************************************************************************************/

void HelpWebsterHomePage()
{
	wxString strURL = CamResource::GetText(_R(IDS_URL_PRODUCT));
	CCamApp::LaunchWebBrowser(strURL);
}
/*********************************************************************************************************************
	void HelpXaraHomePage()

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Inputs:		-
	Outputs:	-
	Returns		-
	Purpose:	Provides access from a help menu to Xara's HomePage Page.
	Errors		If the browser could not be opened, an Error message box is diplayed.
	See Also	-

***********************************************************************************************************************/
void HelpXaraHomePage()
{
	wxString strURL = CamResource::GetText(_R(IDS_URL_XARASITE));
	CCamApp::LaunchWebBrowser(strURL);
}

//#endif //webster

/*********************************************************************************************************************
	BOOL IsCDROMOn() 

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/01/97
	Inputs:		-
	Outputs:	-
	Returns		True if this is a CD version of Webster, FALSE if it is a download version.
	Purpose:	To discover whether the installed version of Webster is a CD or DownLoad version.
	Errors		-
	See Also	-

***********************************************************************************************************************/
BOOL IsCDROMOn() 
{
PORTNOTETRACE("other","IsCDROMOn - do nothing");
#if !defined(EXCLUDE_FROM_XARALX)
	BOOL bSetting;
	HKEY arhKeys[3];
	DWORD dwBoolSize = sizeof(BOOL);
	DWORD dwDisposition;

	BOOL bResult = 	((RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_ALL_ACCESS, &arhKeys[0]) == ERROR_SUCCESS) &&

	(RegCreateKeyEx(arhKeys[0], PRODUCT_MANUFACTURER,  0, NULL, REG_OPTION_NON_VOLATILE,  KEY_ALL_ACCESS, NULL, &arhKeys[1], &dwDisposition) == ERROR_SUCCESS) &&

	(RegCreateKeyEx(arhKeys[1], PRODUCT_WEBLINKNAME,  0, NULL, REG_OPTION_NON_VOLATILE,  KEY_ALL_ACCESS, NULL, &arhKeys[2], &dwDisposition) == ERROR_SUCCESS) &&

	(RegQueryValueEx(arhKeys[2],	"Search CDROM",	NULL, NULL,  (LPBYTE) &bSetting,	&dwBoolSize) == ERROR_SUCCESS));

//	ERROR2IF(!bResult, "Registry read error");

	// If We failed to read from the registry, Untick the Menu item.
	if(!bResult)
		bSetting = bResult;

	for (INT32 i = 2; i >= 0; i--) 
	      RegCloseKey(arhKeys[i]);

	return bSetting;
#else
	return FALSE;
#endif
}

/*********************************************************************************************************************
	void WebsterDemos()

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Inputs:		-
	Outputs:	-
	Returns		-
	Purpose:	Provides access from a help menu to Webster's Movies Demos Page.
	Errors		If the browser could not be opened, an Error message box is diplayed.
	See Also	-

************************************************************************************************************************/
void WebsterDemos()
{
PORTNOTETRACE("other","Movies index - do nothing");
#ifndef EXCLUDE_FROM_XARALX
	// Opens the default Browser on Xara's Home Page
	const String_256 cmd = TEXT(PRODUCT_WEBLINKEXENAME) TEXT(" movies/index.htm");
	InvokeWeblink(cmd);
#endif
}

/*********************************************************************************************************************
	void WebsterHelpPages()

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	17/02/97
	Inputs:		-
	Outputs:	-
	Returns		-
	Purpose:	Provides access from a help menu to Webster's Help Pages
	Errors		If the browser could not be opened, an Error message box is diplayed.
	See Also	-

************************************************************************************************************************/
void WebsterHelpPages()
{
	wxString strURL = CamResource::GetText(_R(IDS_URL_TUTORIALS));
	CCamApp::LaunchWebBrowser(strURL);
}

/*********************************************************************************************************************
	void HelpXaraPurchasePage()

	Author:		Priestley (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/09/2000
	Inputs:		-
	Outputs:	-
	Returns		-
	Purpose:	Provides access from a help menu to Xara's Purchase Page.
	Errors		If the browser could not be opened, an Error message box is diplayed.
	See Also	-

***********************************************************************************************************************/
void HelpXaraPurchasePage()
{
	wxString strURL = CamResource::GetText(_R(IDS_URL_PURCHASE));
	CCamApp::LaunchWebBrowser(strURL);
}


/*********************************************************************************************************************

>	BOOL InvokeWeblink(const String_256& command)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com> from Ranbir code
	Created:	16/09/97
	Inputs:		-
	Returns		True if worked ok, False otherwise
	Purpose:	Allows a command to be sent to Weblink. This enables the default browser to be fired up
				to display the specified URL or file. If there is a browser open then this will be used
				instead rather than firing up a new instance.
	Errors		If the command had a problem, an Error message box is diplayed.
	See Also	-

************************************************************************************************************************/

BOOL InvokeWeblink(const String_256& command)
{
	CCamApp::LaunchWebBrowser(command);
	return TRUE;
}


/*********************************************************************************************************************
	void SetWebsterReg(BOOL pcData) 

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/01/97
	Inputs:		pcdata - To set the state of the Flag
	Outputs:	-
	Returns		-
	Purpose:	To set the registry flag for Webster.
				i.e. Is the Help to be obtained from the CD or from the Net.
	Errors		
	See Also	-

***********************************************************************************************************************/
//#ifdef WEBSTERCD
void SetWebsterReg(BOOL pcData) 
{
PORTNOTETRACE("other","SetWebsterReg - do nothing");
#ifndef EXCLUDE_FROM_XARALX
	HKEY arhKeys[3];
	DWORD dwBoolSize = sizeof(BOOL);
	DWORD dwDisposition;

		BOOL bResult = ((RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_ALL_ACCESS, &arhKeys[0]) == ERROR_SUCCESS) &&

		(RegCreateKeyEx(arhKeys[0], PRODUCT_MANUFACTURER,  0, NULL, REG_OPTION_NON_VOLATILE,  KEY_ALL_ACCESS, NULL, &arhKeys[1], &dwDisposition) == ERROR_SUCCESS) &&

		(RegCreateKeyEx(arhKeys[1], PRODUCT_WEBLINKNAME,  0, NULL, REG_OPTION_NON_VOLATILE,  KEY_ALL_ACCESS, NULL, &arhKeys[2], &dwDisposition) == ERROR_SUCCESS) &&

		(RegSetValueEx (arhKeys[2],	"Search CDROM",	NULL, REG_DWORD,  (LPBYTE) &pcData,	dwBoolSize) == ERROR_SUCCESS));

	for (INT32 i = 2; i >= 0; i--) 
	      RegCloseKey(arhKeys[i]);
#endif
}
//#endif
