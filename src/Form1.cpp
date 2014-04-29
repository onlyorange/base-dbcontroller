#include "Form1.h"


Form1::Form1(void)
{
}

Form1::~Form1(void)
{
}

bool
Form1::Initialize()
{
	// Construct an XML form
	Construct(L"IDF_FORM1");

	return true;
}

result
Form1::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	// Get a button via resource ID
	__pButtonOk = static_cast<Button *>(GetControl(L"IDC_BUTTON_OK"));
	if (__pButtonOk != null)
	{
		__pButtonOk->SetActionId(ID_BUTTON_OK);
		__pButtonOk->AddActionEventListener(*this);
	}
	__pEditID = static_cast<EditField*>(GetControl(L"IDC_EDITFIELD1"));
	__pEditName = static_cast<EditField*>(GetControl(L"IDC_EDITFIELD2"));
	__pEditAge = static_cast<EditField*>(GetControl(L"IDC_EDITFIELD3"));

	__pList = static_cast<List*>(GetControl(L"IDC_LIST1"));

	__pList->AddItemEventListener(*this);
	ShowAllMember();

	__pInfoPopup = new InfoPopup;

	__pInfoPopup->Init(this);

	return r;
}

result
Form1::OnTerminating(void)
{
	result r = E_SUCCESS;

	if(__pInfoPopup)
	{
		delete __pInfoPopup;
		__pInfoPopup = null;
	}
	ReleaseDB();

	// TODO: Add your termination code here

	return r;
}


void
Form1::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_BUTTON_OK:  //Insert member
		{
			AppLog("OK Button is clicked! \n");
			__pButtonOk = null;

			String id = __pEditID->GetText();
			String name = __pEditName->GetText();
			int age ; Integer::Parse(__pEditAge->GetText(),age);

			//check if data is empty
			if(id.IsEmpty() || name.IsEmpty() || __pEditAge->GetText().IsEmpty() )
			{
				MessageBox msg;
				msg.Construct(L"notice","please fill all field", MSGBOX_STYLE_OK, 3000);
				int modalResult = 0;
				msg.ShowAndWait(modalResult);

				return;
			}

			//Insert member
			bool success = GetDB()->InsertMember(id,name,age);
			if(success)
			{
				__pEditID->SetText(L"");
				__pEditName->SetText(L"");
				__pEditAge->SetText(L"");

				ShowAllMember();
			}
			else
			{
				MessageBox msg;
				msg.Construct(L"notice"," duplicate id", MSGBOX_STYLE_OK, 3000);
				int modalResult = 0;
				msg.ShowAndWait(modalResult);
			}
		}
		break;

	default:
		break;
	}
}

//display member information on the list
void
Form1::ShowAllMember()
{
	if(__pList) __pList->RemoveAllItems();

	//get all the member list
	ArrayList list;
	GetDB()->GetAllMember(list);

	//get detailed member information
	MEMBERINFO* pInfo = null;
	IEnumerator* pEnum = list.GetEnumeratorN();

	String str;
	while(pEnum->MoveNext() == E_SUCCESS)
	{
		pInfo = (MEMBERINFO*)pEnum->GetCurrent();
		AppLog("datainfo : (%d,%ls %ls %d)",
				pInfo->seq,pInfo->id.GetPointer(),pInfo->name.GetPointer(),
				pInfo->age);

		str.Clear();
		str.Format(150,L"%ls / %ls / %d ",
				pInfo->id.GetPointer(),pInfo->name.GetPointer(),
				pInfo->age);

		AppLog("str : %ls",str.GetPointer());

		//add item at list
		if(__pList)
		{
			__pList->AddItem(&str,null,null,null,pInfo->seq);
		}
	}

	RequestRedraw(true);

	if(pEnum ) delete pEnum;


	list.RemoveAll(true);
}


//called when list item is changed
void
Form1::OnItemStateChanged(const Tizen::Ui::Control &source, int index, int itemId, Tizen::Ui::ItemStatus status)
{
	// TODO: Add your implementation codes here
	AppLog("OnItemStateChanged");

	__pInfoPopup->ShowInfo(itemId);
}

//called when user event message is received
void
Form1::OnUserEventReceivedN (RequestId requestId, Tizen::Base::Collection::IList *pArgs)
{
	ShowAllMember();
}
