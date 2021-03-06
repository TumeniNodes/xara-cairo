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

// Implements the ClickModifiers class

#ifndef INC_CLICKMODS
#define INC_CLICKMODS

#include "binds.h"

/*
 * Key state Masks for Mouse messages
 */
#if !defined(MK_LBUTTON)
#define MK_LBUTTON		0x0001
#define MK_RBUTTON		0x0002
#define MK_SHIFT		0x0004
#define MK_CONTROL		0x0008
#define MK_MBUTTON		0x0010
#define MK_ALT			0x1000	// Added during LX port...
#endif

/*******************************************************************************************
> 	Class ClickModifiers : public CC_CLASS_MEMDUMP
	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	07/03/95 (from original ClickModifiers struct)
	Purpose:	To provide platform-indy click modifiers to the kernel and to construct
				such objects from the state of the keyboard, mouse and preferences on
				request.
	Comment:	When a click event occurs various other input devices (Shift keys, other
				mouse buttons, etc.) are tested and their states are used to modify the
				meaning of the click.  The input devices which modify the meaning of a click
				are not named but their meaning inside the kernel is given so that the OIL
				layer can best decide which devices to test to perform which modifications.

				Notice that combinations of these modifiers may be TRUE at any time, although
				some combinations may be illegal.

				Note that the "Menu" modifier is just added for consistency with dialogue
				handling - click events on the document views will NEVER receive clicks with
				the "Menu" modifier.

public:
	BOOL Adjust 		: 1;	// Shift key (Also configurable to a mouse button)
	BOOL Menu 			: 1;	// User wants a context sensitive menu (Configurable)
	BOOL Constrain 		: 1;	// Control key (Also configurable to a mouse button)
	BOOL Alternative1	: 1;	// Left-Alt key (Also configurable to a mouse button)
	BOOL Alternative2	: 1;	// Right-Alt key ???

	BOOL EditObject		: 1;	// User wants the best editor for this object (Configurable)
	BOOL FullScreen		: 1;	// User wants to toggle full-screen mode (Configurable)
	BOOL ZoomIn			: 1;	// User wants to zoom in one stage (Configurable)

	UINT32 Pressure;				// Pen Pressure


	SeeAlso:	ClickType; DocView::OnClick; Tool_v1::OnClick

********************************************************************************************/ 

class ClickModifiers : public CC_CLASS_MEMDUMP
{
	CC_DECLARE_MEMDUMP(ClickModifiers);

public:
// Member variables...
	//	 Modifier name			   Default meaning on Windows (RISC OS and the Mac may differ)
	BOOL Adjust 		: 1;	// Shift key (Also configurable to a mouse button)
	BOOL Menu 			: 1;	// User wants a context sensitive menu (Configurable)
	BOOL Constrain 		: 1;	// Control key (Also configurable to a mouse button)
	BOOL Alternative1	: 1;	// Left-Alt key (Also configurable to a mouse button)
	BOOL Alternative2	: 1;	// Right-Alt key ???

	BOOL EditObject		: 1;	// User wants the best editor for this object (Configurable)
	BOOL FullScreen		: 1;	// User wants to toggle full-screen mode (Configurable)
	BOOL ZoomIn			: 1;	// User wants to zoom in one stage (Configurable)

	//Graham 18/6/96: New click modifier ClickWhileDrag
	//Set to TRUE if the present click was made while a drag was in progress
	//e.g. right mouse button clicks while left button is dragging
	BOOL ClickWhileDrag : 1;

	// Matt 16/11/00 - New ClickModifier added - 
	// Push tool activation / Auto-centre screen - when true the user can move the mouse to pan the view
	// or if just a single click, will cause view to centre about the mouse cursor
	BOOL PushCentreTool : 1;

	UINT32 Pressure;				// Pen Pressure
	// More of these may be added when they are needed...

// Member functions...
	ClickModifiers();				// Constructor
	ClickModifiers(const ClickModifiers& CopyMods);
	ClickModifiers& operator=(const ClickModifiers& CopyMods);

	BOOL IsHandledByTool();			// Determines whether modifier should be handled by tool

	static ClickModifiers GetClickModifiers();
	static ClickModifiers GetClickModifiers(wxMouseEvent &event);
	static ClickModifiers GetClickModifiers(UINT32 nFlags);
									// Creates a ClickMod based on keys, mouse buttons and user
									// preferences.
	static UINT32 SynthesizeMouseEventFlags();
	static UINT32 SynthesizeMouseEventFlags(wxMouseEvent &event);
									// Create an nFlags bitfield by reading key/button states
									// from the OS.

private:
	void GetModsFromPrefs(ButtonFunction buttfunc);
									// Set modifier flags according to the button function

// Preferences
public:
	static BOOL DeclarePrefs();								// Declare prefs to the system
	static void GetButtonFuncName(ButtonFunction bf, String_32* pString);	
															// Get the name of a button function
	static void RestoreDefaults();							// Restore to default preferences
	static ButtonFunction GetButtonFunc(UINT32 ButtonID);		// Read the preference for a given button
	static void SetButtonFunc(UINT32 ButtonID, ButtonFunction bf);
															// Read the preference for a given button

private:
	static ButtonFunction LeftButtonFunction;				// Functions assigned to left,mid,right buttons
	static ButtonFunction MiddleButtonFunction;
	static ButtonFunction RightButtonFunction;

	static UINT32 ButtFuncNames[BUTTFUNC_LAST];				// Array of user-level function names

};

#endif // INC_CLICKMODS


