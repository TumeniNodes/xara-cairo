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

//
// Declaration of Bitmap Effects operations.
//

#ifndef INC_OPLIVEFX
#define INC_OPLIVEFX

#define OPTOKEN_APPLY_LIVEEFFECT		_T("ApplyLiveEffect")
#define OPTOKEN_EDIT_LIVEEFFECT			_T("EditLiveEffect")
#define OPTOKEN_DELETE_LIVEEFFECT		_T("DeleteLiveEffect")
#define OPTOKEN_DELETEALL_LIVEEFFECT	_T("DeleteAllLiveEffect")
#define OPTOKEN_APPLY_FEATHEREFFECT		_T("ApplyFeatherEffect")
#define OPTOKEN_CHANGE_EFFECT_RES		_T("ChangeEffectRes")
#define OPTOKEN_CHANGE_EFFECT_LOCK		_T("ChangeEffectLock")
#define OPTOKEN_CHANGE_EFFECT_LOCKALL	_T("ChangeEffectLockAll")
#define OPTOKEN_EDITEFFECTS				_T("EditEffectsRootMenuItem")
#define OPTOKEN_EDIT_LEGACYEFFECT		_T("EditLegacyEffect")

PORTNOTE("other","Define dummy IXMLDOMDocumentPtr")
#ifndef EXCLUDE_FROM_XARALX
#else
typedef void		   *IXMLDOMDocumentPtr;
#endif

//#include "selop.h"
#include "menuitem.h"
//#include "xpehost.h"

class Range;
class ListRange;
class NodeEffect;
class EffectsStack;
class NodeLiveEffect;
class NodeFeatherEffect;
class NodeBitmapEffect;

// Define this if you want to pick up the development of the Feather Effect
#define FEATHER_EFFECT

/********************************************************************************************
>   class OpLiveEffectParam

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	09/09/2004

	Purpose:	Contain all the info transferred from the UI to the OpApplyLiveEffect
				operation

	SeeAlso:	Operation::DoWithParam

********************************************************************************************/

class OpLiveEffectParam : public OpMenuParam
{
CC_DECLARE_DYNAMIC(OpLiveEffectParam)

public:
	OpLiveEffectParam() : OpMenuParam()
	{
		bIsDestructive = FALSE;
		StackPosition = -1;
		pPPStack = NULL;
		bReUseEditor = FALSE;
		pEditsList = NULL;
		bSilent = FALSE;
		dResolution = 0;
		bForceLock = FALSE;
	}

// Properties
public:
	BOOL				bIsDestructive;
	INT32					StackPosition;
	EffectsStack*		pPPStack;
	BOOL				bReUseEditor;
	IXMLDOMDocumentPtr	pEditsList;
	BOOL				bSilent;
	double				dResolution;
	BOOL				bForceLock;
};




/********************************************************************************************
>   class OpFeatherEffectParam

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/11/2004

	Purpose:	Contain all the info transferred from the UI to the OpApplyLiveEffect
				operation

	SeeAlso:	Operation::DoWithParam

********************************************************************************************/

#ifdef FEATHER_EFFECT
class OpFeatherEffectParam : public OpLiveEffectParam
{
CC_DECLARE_DYNAMIC(OpFeatherEffectParam)

public:
	OpFeatherEffectParam() : OpLiveEffectParam()
	{
		FeatherSize = 0;
		//Profile = CProfileBiasGain default constructor
	}

// Properties
public:
	MILLIPOINT FeatherSize;
	CProfileBiasGain Profile;
};
#endif




/********************************************************************************************

>	class OpLiveEffect : public SelOperation

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	13/09/2004
	Purpose:	Base class for all LiveEffect ops
	See also:	

********************************************************************************************/

class OpLiveEffect : public SelOperation
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpLiveEffect);

	static BOOL				Init();
	static BOOL				UpdateCurrentEditor();
	static BOOL				DoCopyPPStackToSelection(UndoableOperation* pOp, EffectsStack* pPPStack, Document* pSrcDoc, Document* pDestDoc);
	static BOOL				DoLocaliseLiveEffects(UndoableOperation* pOp, Node* pNode);
	static BOOL				DoLocaliseEffectAttrs(UndoableOperation* pOp, Node* pNode);
	static BOOL				DoCopyEffectAttr(UndoableOperation* pOp, NodeAttribute* pAttr, Node* pNode);
	static Node*			DoDeleteAllPostProcessors(UndoableOperation* pOp, NodeRenderableInk* pNode, BOOL bDeleteOldControllers = FALSE, BOOL bDeleteBelowLocked = FALSE);
	static BOOL				DoDeletePostProcessor(UndoableOperation* pOp, Range* pRange);
	static BOOL				DoDeletePostProcessor(UndoableOperation* pOp, NodeEffect* pLE, BOOL bMoveEffectAttrs = FALSE);
#ifdef FEATHER_EFFECT
	static ListRange*		DoApplyFeatherEffect(UndoableOperation* pOp,
												 Range* pSel,
												 String_256 strEffectID,
												 String_64 strDisplayName,
												 MILLIPOINT FeatherSize,
												 CProfileBiasGain Profile);
	static NodeFeatherEffect* DoApplyFeatherEffectNode(UndoableOperation* pOp,
												 Node* pSel,
												 String_256 strEffectID,
												 String_64 strDisplayName,
												 MILLIPOINT FeatherSize,
												 CProfileBiasGain Profile);
#endif
	static BOOL					DoCopyEffectsStack(UndoableOperation* pOp, Node* pDestNode, ListRange* pEffectsStack, Document* pSrcDoc, Document* pDestDoc);
	static NodeBitmapEffect*	DoApplyLiveEffect(UndoableOperation* pOp,
												  Node* pNode,
												  String_256 strEffectID,
												  String_64 strDisplayName,
												  double dResolution,
												  IXMLDOMDocumentPtr pEditsList = NULL,
												  BOOL bMakeLocked = FALSE);
	static BOOL					DoCopyAttributes(UndoableOperation* pOp, Node* pSrcNode, Node* pDestNode, BOOL bEffectsOnly, Document* pSrcDoc, Document* pDestDoc);

public:
	OpLiveEffect();
	~OpLiveEffect();

	// the Op's Do() function.
	virtual void Do(OpDescriptor* pOpDesc);
	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);

	ListRange*				GetLiveEffectList() {return m_pLERange;}
	virtual void			DoEndEdit(BOOL bChangingSelection = FALSE,
									  BOOL bBroadcast = TRUE,
									  BOOL bCancelled = FALSE,
									  BOOL bEmpty = FALSE);
	BOOL					GetPreviewQuality() const {return m_bPreviewQuality;}
	void					SetPreviewQuality(BOOL bNew) {m_bPreviewQuality = bNew;}
							
protected:					
	ListRange*				DoApplyLiveEffect(Range* pSel,
											  String_256 strEffectID,
											  String_64 strDisplayName,
											  double dResolution,
											  IXMLDOMDocumentPtr pEditsList = NULL,
											  BOOL bMakeLocked = FALSE,
											  BOOL bDetectDirectBitmaps = FALSE);
	virtual ListRange*		DoApplyLockedEffect(Range* pSel,
												String_256 strEffectID,
												String_64 strDisplayName,
												double dResolution,
												IXMLDOMDocumentPtr pEditsList = NULL,
												BOOL bDetectDirectBitmaps = FALSE);
	BOOL					EnsureLiveEffectOriginalBitmaps();
	BOOL					GenerateBitmap(NodeBitmapEffect* pBitmapEffect,
										   double dResolution,
										   LPBITMAPINFO* ppInfo,
										   LPBYTE* ppBits,
										   DocRect* prectBounds);
	BOOL					SetChanged(BOOL bNewState);
	BOOL					HasChanged();
	BOOL					IsTopOfStack(EffectsStack* pPPStack, INT32 iStackPos);
	virtual BOOL			DoEffectOp(ListRange* pLevelRange, OpLiveEffectParam* pLEOpParam) { return TRUE; }
	virtual BOOL			AllowEmptySelection() const { return FALSE; }
	virtual BOOL			CheckEffectOpParam(OpLiveEffectParam* pLEOpParam);
	virtual BOOL			DoEffectRangeOp(UINT32 idsProgress, Range* pLevel, OpLiveEffectParam* pParam);
	virtual BOOL			DoEffectNodeOp(NodeEffect* pEffect, OpLiveEffectParam* pParam, NodeBitmapEffect** ppNewEffect) {*ppNewEffect = NULL; return FALSE;}
							
// Member variables			
	ListRange*				m_pLERange;					// List of LiveEffects/nodes we are working on - we DO own this list
	BOOL					m_bAppliedNewEffects;
	String_256				m_strEffectID;				// Record the effect name for display if the Op works
	INT32						m_iStackPos;				// Record the effect stack position for display if the Op works
	BOOL					m_bPreviewQuality;			// True if this Op only needs preview quality rendering at the moment
};

/********************************************************************************************

>	class OpApplyLiveEffect : public OpLiveEffect

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	07/09/2004
	Purpose:	Operation to apply Bitmap Effect to the selection.
	See also:	

********************************************************************************************/
class OpApplyLiveEffect : public OpLiveEffect
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpApplyLiveEffect);

public:
	// query the op for information about itself.
	static OpState GetParamState(String_256* pstrDescription, OpDescriptor* pOpDesc, OpParam* pOpParam);
	virtual void GetOpName(String_256* pstrOpName);

	// the Op's Do() function.
//	virtual void Do(OpDescriptor* pOpDesc);
	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL MayChangeNodeBounds() const { return TRUE; }

};




/********************************************************************************************

>	class OpApplyFeatherEffect : public OpLiveEffect

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	07/09/2004
	Purpose:	Operation to apply Bitmap Effect to the selection.
	See also:	

********************************************************************************************/
#ifdef FEATHER_EFFECT
class OpApplyFeatherEffect : public OpLiveEffect
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpApplyFeatherEffect);

public:
	// query the op for information about itself.
	static OpState GetParamState(String_256* pstrDescription, OpDescriptor* pOpDesc, OpParam* pOpParam);
	virtual void GetOpName(String_256* pstrOpName);

	// the Op's Do() function.
//	virtual void Do(OpDescriptor* pOpDesc);
	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL MayChangeNodeBounds() const { return TRUE; }

};
#endif



/********************************************************************************************

>	class OpEditLiveEffect : public OpLiveEffect

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/09/2004
	Purpose:	Operation to apply Bitmap Effect to the selection.
	See also:	

********************************************************************************************/
class OpEditLiveEffect : public OpLiveEffect
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpEditLiveEffect);

public:
	// query the op for information about itself.
	static OpState GetParamState(String_256* pstrDescription, OpDescriptor* pOpDesc, OpParam* pOpParam);
//	static OpState GetState(String_256* pstrDescription, OpDescriptor* pOpDesc);
	virtual void GetOpName(String_256* pstrOpName);

	// the Op's Do() function.
//	virtual void Do(OpDescriptor* pOpDesc);
//	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);
	virtual BOOL			DoEffectOp(ListRange* pLevelRange, OpLiveEffectParam* pLEOpParam);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL MayChangeNodeBounds() const { return TRUE; }

protected:
	virtual BOOL AllowEmptySelection() const { return TRUE; }

};




/********************************************************************************************

>	class OpDeleteLiveEffect : public OpLiveEffect

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/09/2004
	Purpose:	Operation to apply Bitmap Effect to the selection.
	See also:	

********************************************************************************************/
class OpDeleteLiveEffect : public OpLiveEffect
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpDeleteLiveEffect);

public:
	// query the op for information about itself.
	static OpState GetState(String_256* pstrDescription, OpDescriptor* pOpDesc);
	virtual void GetOpName(String_256* pstrOpName);

	// the Op's Do() function.
//	virtual void Do(OpDescriptor* pOpDesc);
//	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);
	virtual BOOL			DoEffectOp(ListRange* pLevelRange, OpLiveEffectParam* pLEOpParam);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL MayChangeNodeBounds() const { return TRUE; }

protected:
//	BOOL		CanEditSelLiveEffect();

};




/********************************************************************************************

>	class OpDeleteAllLiveEffect : public OpLiveEffect

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/09/2004
	Purpose:	Operation to delete all Bitmap Effects on the selection.
	See also:	

********************************************************************************************/
class OpDeleteAllLiveEffect : public OpLiveEffect
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpDeleteAllLiveEffect);

public:
	// query the op for information about itself.
	static OpState GetState(String_256* pstrDescription, OpDescriptor* pOpDesc);
	virtual void GetOpName(String_256* pstrOpName);

	// the Op's Do() function.
//	virtual void Do(OpDescriptor* pOpDesc);
//	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);
	virtual BOOL			DoEffectOp(ListRange* pLevelRange, OpLiveEffectParam* pLEOpParam);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL			MayChangeNodeBounds() const { return TRUE; }

protected:
	virtual BOOL			CheckEffectOpParam(OpLiveEffectParam* pLEOpParam);

};




/********************************************************************************************

>	class OpEffectRes : public OpLiveEffect

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/06/2005
	Purpose:	Operation to change effect resolution.
	See also:	

********************************************************************************************/
class OpEffectRes : public OpLiveEffect
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpEffectRes);

public:
	// query the op for information about itself.
	static OpState			GetState(String_256* pstrDescription, OpDescriptor* pOpDesc);
	virtual void			GetOpName(String_256* pstrOpName);
	virtual BOOL			DoEffectOp(ListRange* pLevelRange, OpLiveEffectParam* pLEOpParam);
	virtual BOOL			DoEffectNodeOp(NodeEffect* pEffect, OpLiveEffectParam* pParam, NodeBitmapEffect** ppNewEffect);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL			MayChangeNodeBounds() const { return FALSE; }

protected:
//	BOOL		CanEditSelLiveEffect();

};




/********************************************************************************************

>	class OpEffectLock : public OpLiveEffect

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/06/2005
	Purpose:	Operation to change effect locked state.
	See also:	

********************************************************************************************/
class OpEffectLock : public OpLiveEffect
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpEffectLock);

public:
	// query the op for information about itself.
	static OpState			GetState(String_256* pstrDescription, OpDescriptor* pOpDesc);
	virtual void			GetOpName(String_256* pstrOpName);
	virtual BOOL			DoEffectOp(ListRange* pLevelRange, OpLiveEffectParam* pLEOpParam);
	virtual BOOL			DoEffectNodeOp(NodeEffect* pEffect, OpLiveEffectParam* pParam, NodeBitmapEffect** ppNewEffect);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL			MayChangeNodeBounds() const { return FALSE; }

protected:
	virtual BOOL			CopyEffectAttrs(NodeBitmapEffect* pSrcNode, NodeBitmapEffect* pDestNode);

};




/********************************************************************************************

>	class OpEffectLockAll : public OpEffectLock

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/06/2005
	Purpose:	Operation to change effect locked state.
	See also:	

********************************************************************************************/
class OpEffectLockAll : public OpEffectLock
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpEffectLockAll);

public:
	// query the op for information about itself.
	static OpState GetState(String_256* pstrDescription, OpDescriptor* pOpDesc);
	virtual void GetOpName(String_256* pstrOpName);

	// the Op's Do() function.
//	virtual void Do(OpDescriptor* pOpDesc);
//	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);
	virtual BOOL			DoEffectOp(ListRange* pLevelRange, OpLiveEffectParam* pLEOpParam);

	// whether or not the Op may change the bounds of some nodes.
	virtual BOOL			MayChangeNodeBounds() const { return FALSE; }

protected:
	virtual BOOL			CheckEffectOpParam(OpLiveEffectParam* pLEOpParam);

};




/********************************************************************************************

>	class OpEditLegacyEffect : public Operation

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/08/2005
	Purpose:	Hold all class for passing on requests to "edit" a legacy effect
	See also:	

********************************************************************************************/
class OpEditLegacyEffect : public Operation
{
// Give my name out in memory dumps.
CC_DECLARE_DYNCREATE(OpEditLegacyEffect);

public:
	// query the op for information about itself.
	static OpState GetParamState(String_256* pstrDescription, OpDescriptor* pOpDesc, OpParam* pOpParam);
	virtual void GetOpName(String_256* pstrOpName);

	// the Op's Do() function.
	virtual void Do(OpDescriptor* pOpDesc);
	virtual void DoWithParam(OpDescriptor* pOpDesc, OpParam* pOpParam);

};




/********************************************************************************************

>	class ChangeEffectResAction : public Action

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	09/06/2005
	Purpose:	An action to record/restore the current state of the LiveEffect EditList.
	SeeAlso:	

********************************************************************************************/

class ChangeEffectResAction : public Action
{

CC_DECLARE_DYNCREATE(ChangeEffectResAction)

public:
	ChangeEffectResAction();
	~ChangeEffectResAction();
	virtual ActionCode Execute();
	static ActionCode Init( Operation* pOp,
							ActionList* pActionList,
							Action** NewAction);
	static BOOL CreateChangeEffectResAction(Operation* pOp,
									  NodeBitmapEffect* pEffectNode,
									  double dResolution
									 );
	// Data members
	NodeBitmapEffect*		m_pEffectNode;
	double					m_dResolution;
};




/********************************************************************************************

>	class ToggleLockAction : public Action

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	13/06/2005
	Purpose:	An action to record/restore the current state of the LiveEffect EditList.
	SeeAlso:	

********************************************************************************************/

/*class ToggleLockAction : public Action
{

CC_DECLARE_DYNCREATE(ToggleLockAction)

public:
	ToggleLockAction();
	~ToggleLockAction();
	virtual ActionCode Execute();
	static ActionCode Init( Operation* pOp,
							ActionList* pActionList,
							Action** NewAction);
	static BOOL CreateToggleLockAction(Operation* pOp,
									  NodeBitmapEffect* pEffectNode,
									  BOOL bNewLockedState
									 );
	// Data members
	NodeBitmapEffect*		m_pEffectNode;
	double					m_bLockedState;
};*/




/********************************************************************************************

>	class MarkEditListAction : public Action

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	17/09/2004
	Purpose:	An action to record/restore the current state of the LiveEffect EditList.
	SeeAlso:	

********************************************************************************************/

class MarkEditListAction : public Action
{

CC_DECLARE_DYNCREATE(MarkEditListAction)

public:
	MarkEditListAction();
	~MarkEditListAction();
	virtual ActionCode Execute();
	static ActionCode Init( Operation* pOp,
							ActionList* pActionList,
							Action** NewAction);
	static ActionCode DoMarkEditList(Operation* pOp,
									  ActionList* pActionList,
									  EffectsStack* pPPStack,
									  INT32 iStackPos
									 );
	// Data members
	IXMLDOMDocumentPtr		pEditList;
	INT32						iStackPos;
};




#endif	// INC_OPLIVEFX
