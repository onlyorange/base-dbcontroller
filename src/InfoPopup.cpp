
#include "InfoPopup.h"

/**
 * InfoPopup shows information about each item and provides an interface to view, modify and delete the item.
 */

InfoPopup::InfoPopup()
:__pPopup(null),__selecteditem(-1)
{
	// TODO Auto-generated constructor stub


}

InfoPopup::~InfoPopup() {
	// TODO Auto-generated destructor stub

	Close();
}

// Init popup
void
InfoPopup::Init(Form* pForm)
{
	__pForm = pForm;

	__pPopup = new Popup();

	if(__pPopup)
	{
		__pPopup->Construct(L"IDP_POPUP1");
		////IDC_EDITFIELD1
		__pPopupEditID = static_cast<EditField*>(__pPopup->GetControl(L"IDC_EDITFIELD1"));
		__pPopupEditName = static_cast<EditField*>(__pPopup->GetControl(L"IDC_EDITFIELD2"));
		__pPopupEditAge = static_cast<EditField*>(__pPopup->GetControl(L"IDC_EDITFIELD3"));

		__pBtnPopupOk = static_cast<Button *>(__pPopup->GetControl(L"IDC_BTN_OK"));
		__pBtnPopupOk->SetActionId(ID_POPUP_OK);
		__pBtnPopupOk->AddActionEventListener(*this);

		__pBtnPopupMod = static_cast<Button *>(__pPopup->GetControl(L"IDC_BTN_MOD"));
		__pBtnPopupMod->SetActionId(ID_POPUP_MOD);
		__pBtnPopupMod->AddActionEventListener(*this);

		__pBtnPopupDel =  static_cast<Button *>(__pPopup->GetControl(L"IDC_BTN_DEL"));
		__pBtnPopupDel->SetActionId(ID_POPUP_DEL);
		__pBtnPopupDel->AddActionEventListener(*this);

		__pPopup->SetShowState(false);
	}
}

// Delete popup
void
InfoPopup::Close()
{
	if(__pPopup)
	{
		delete __pPopup;
		__pPopup = null;
	}
}

// Clear all values
void
InfoPopup::ClearValue()
{
	__pPopupEditID->SetText(L"");
	__pPopupEditName->SetText(L"");
	__pPopupEditAge->SetText(L"");
}

// Show Infomation about a member by seq
void
InfoPopup::ShowInfo(int seq)
{
	__selecteditem = seq;
	MEMBERINFO memInfo;
	GetDB()->GetMemberBySeq(seq,memInfo);

	AppLog("info : (%d,%ls %ls %d)",
			memInfo.seq,memInfo.id.GetPointer(),memInfo.name.GetPointer(),memInfo.age);

	__pPopupEditID->SetText(memInfo.id);
	__pPopupEditName->SetText(memInfo.name);
	Integer age(memInfo.age);
	__pPopupEditAge->SetText(age.ToString());

	__pPopup->SetShowState(true);
	__pPopup->Show();
}

void
InfoPopup::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_POPUP_OK:  //hide popup
		{
			if(__pPopup)
			{
				__pPopup->SetShowState(false);
				__pForm->Draw();__pForm->Show();
				__selecteditem = -1;

				__pPopupEditID->SetText(L"");
				__pPopupEditName->SetText(L"");
				__pPopupEditAge->SetText(L"");

			}
		}
		break;
	case ID_POPUP_MOD: //change data and hide popup
		{
			if(__pPopup)
			{
				String id = __pPopupEditID->GetText();
				String name = __pPopupEditName->GetText();
				int age ; Integer::Parse(__pPopupEditAge->GetText(),age);

				AppLog("%ls,%ls,%d",id.GetPointer(),name.GetPointer(), age);
				GetDB()->UpdateMember(__selecteditem,id,name,age);

				__selecteditem = -1;
				__pPopupEditID->SetText(L"");
				__pPopupEditName->SetText(L"");
				__pPopupEditAge->SetText(L"");

				__pPopup->SetShowState(false);
				__pForm->Draw();__pForm->Show();

				__pForm->SendUserEvent(100,null);

			}
		}
		break;
	case ID_POPUP_DEL: //delete data and hide popup
		{
			GetDB()->DeleteMember(__selecteditem);
			__selecteditem = -1;

			__pPopupEditID->SetText(L"");
			__pPopupEditName->SetText(L"");
			__pPopupEditAge->SetText(L"");

			__pPopup->SetShowState(false);
			__pForm->Draw();__pForm->Show();

			__pForm->SendUserEvent(100,null);
		}
		break;
	default:
		break;
	}
}
