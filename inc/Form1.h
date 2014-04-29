#ifndef _FORM1_H_
#define _FORM1_H_

#include <FBase.h>
#include <FUi.h>
#include "DBManager.h"
#include "InfoPopup.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

class Form1 :
	public Tizen::Ui::Controls::Form,
	public Tizen::Ui::IActionEventListener,
 	public Tizen::Ui::IItemEventListener
{

// Construction
public:
	Form1(void);
	virtual ~Form1(void);
	bool Initialize(void);

// Implementation
protected:
	static const int ID_BUTTON_OK = 101;
	Tizen::Ui::Controls::Button *__pButtonOk;

	Tizen::Ui::Controls::EditField* __pEditID;
	Tizen::Ui::Controls::EditField* __pEditName;
	Tizen::Ui::Controls::EditField* __pEditAge;

	Tizen::Ui::Controls::List* __pList;

	InfoPopup* __pInfoPopup;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

public:
	void ShowAllMember();

public:
	virtual void OnItemStateChanged(const Tizen::Ui::Control &source, int index, int itemId, Tizen::Ui::ItemStatus status);
	virtual void OnUserEventReceivedN (RequestId requestId, Tizen::Base::Collection::IList *pArgs) ;
};

#endif	//_FORM1_H_
