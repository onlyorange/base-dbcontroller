#ifndef INFOPOPUP_H_
#define INFOPOPUP_H_
#include <FBase.h>
#include <FUi.h>
#include "DBManager.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;


class InfoPopup
:public Tizen::Ui::IActionEventListener
{
public:
	InfoPopup();
	virtual ~InfoPopup();

private:
	Form* __pForm;

	//about Popup
	Tizen::Ui::Controls::Popup* __pPopup;
	Tizen::Ui::Controls::EditField* __pPopupEditID;
	Tizen::Ui::Controls::EditField* __pPopupEditName;
	Tizen::Ui::Controls::EditField* __pPopupEditAge;

	static const int ID_POPUP_OK = 102;
	Tizen::Ui::Controls::Button *__pBtnPopupOk;

	static const int ID_POPUP_MOD = 103;
	Tizen::Ui::Controls::Button *__pBtnPopupMod;

	static const int ID_POPUP_DEL = 104;
	Tizen::Ui::Controls::Button *__pBtnPopupDel;

	int __selecteditem;

public:
	void SetSelectedItem(int itemid){__selecteditem = itemid;}
	int GetSelectedItem(){return __selecteditem;}

public:
	void Init(Form* pForm);
	void Close();

public:
	void ClearValue();
	void ShowInfo(int seq);

public:
	void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

};

#endif /* INFOPOPUP_H_ */
