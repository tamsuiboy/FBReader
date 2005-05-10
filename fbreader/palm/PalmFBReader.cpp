#include <abstract/ZLOptions.h>
#include <palm/ZLPalmFSManager.h>
#include <abstract/ZLInputStream.h>
#include <abstract/ZLScreenSize.h>

#include "PalmFBReader.h"
#include "PalmViewWidget.h"
#include "PalmPaintContext.h"
#include "PalmFBReader-resources.h"

PalmFBReader::PalmFBReader() : FBReader(new PalmPaintContext()) {
	myViewWidget = new PalmViewWidget();
	setMode(BOOK_TEXT_MODE);
}

PalmFBReader::~PalmFBReader() {
	delete (PalmViewWidget*)myViewWidget;
}

void PalmFBReader::setWindowCaption(const std::string &caption) {}
void PalmFBReader::addButton(ActionCode id, const std::string &name) {}
void PalmFBReader::setButtonVisible(ActionCode id, bool visible) {}
void PalmFBReader::setButtonEnabled(ActionCode id, bool enable) {}
void PalmFBReader::searchSlot() {}
void PalmFBReader::cancelSlot() {}
void PalmFBReader::fullscreenSlot() {}
bool PalmFBReader::isRotationSupported() const { return false; }
void PalmFBReader::stylusPressEvent(int x, int y) {
	myViewWidget->view()->onStylusPress(x, y);
}

static Boolean MainFBReaderFormHandleEvent(EventPtr event) {
	switch (event->eType) {
  	case menuCmdBarOpenEvent:	
			MenuCmdBarAddButton(menuCmdBarOnRight, OpenBook, menuCmdBarResultMenuItem, OpenBook, "");
			MenuCmdBarAddButton(menuCmdBarOnRight, AddBook, menuCmdBarResultMenuItem, AddBook, "");
			return false;

   	case penDownEvent:
			{
				int x = event->screenX;
				int y = event->screenY;
				if (ZLScreenSize::getSize() == ZLScreenSize::SIZE_320x320) {
					x *= 2;
					y *= 2;
				}
				DO_PAINT = true;
				READER->stylusPressEvent(x, y);
				DO_PAINT = false;
				/*
				char txt[4];
				//std::string s = "$$TEST";
				//std::string t = s;
				//int x = (s == t) ? 1 : 0;
				//int x = (s == "$$TEST") ? 1 : 0;
				txt[0] = '0' + x / 100 % 10;
				txt[1] = '0' + x / 10 % 10;
				txt[2] = '0' + x % 10;
				txt[3] = '\0';
				FrmCustomAlert(GoodnightMoonAlert, txt, 0, 0);
				*/
			}
			return true;
  	
  	case frmUpdateEvent:	
  	case frmOpenEvent:	
			{
				DO_PAINT = true;
				READER->repaintView();
				DO_PAINT = false;
			}
			return true;

		case menuEvent:
			switch (event->data.menu.itemID) {
				case OpenBook:
				case AddBook:
					FrmGotoForm(OptionsDialogForm);
					break;
			}
			return false;

		default:
			return false;
	}
}

static char *TEXT = "item";

static Boolean OptionsDialogFormHandleEvent(EventPtr event) {
	switch (event->eType) {
  	
  	case frmOpenEvent:	
			{
				FormPtr form = FrmGetActiveForm();
				TablePtr table = (TablePtr)FrmGetObjectPtr(form, FrmGetObjectIndex(form, OptionsTable));
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						TblSetItemStyle(table, i, j, labelTableItem);
						TblSetItemPtr(table, i, j, TEXT);
					}
					TblSetColumnUsable(table, i, true);
				}
				FrmDrawForm(form);
			}
			return true;

		case penDownEvent:
			FrmGotoForm(MainFBReaderForm);
			return true;

		default:
			return false;
	}
}

static Boolean ApplicationHandleEvent(EventPtr event) {
	FormPtr	frm;
	Int16 formId;

	if (event->eType == frmLoadEvent) {
		// Load the form resource specified in the event then activate the form.
		formId = event->data.frmLoad.formID;
		frm = FrmInitForm(formId);
		FrmSetActiveForm(frm);

		// Set the event handler for the form.  The handler of the currently 
		// active form is called by FrmDispatchEvent each time it receives an event.
		switch (formId) {
			case MainFBReaderForm:
				FrmSetEventHandler(frm, MainFBReaderFormHandleEvent);
				break;
			case OptionsDialogForm:
				FrmSetEventHandler(frm, OptionsDialogFormHandleEvent);
				break;
		}
		return true;
	}
	
	return false;
}

void EventLoop(void) {
	EventType event;
	UInt16 error;
	
	do {
		EvtGetEvent(&event, evtWaitForever);
		if (!SysHandleEvent(&event) && !MenuHandleEvent(0, &event, &error) && !ApplicationHandleEvent(&event)) {
			FrmDispatchEvent(&event);
		}
	} while (event.eType != appStopEvent);
}
