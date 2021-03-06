#pragma once

#include "tablesub.h"
#include "../../gametbl.h"
#include "../../text.h"

namespace mihajong_graphic {

using std::uint64_t;

class TableSubsceneBeginning : public TableSubscene {
protected:
	HugeTextRenderer* myTextRenderer;
	static const std::wstring WindName, Numeral;
	void ZoomChar(unsigned ID, const std::wstring& str, int xOffset, uint64_t Anfang, uint64_t Ende);
public:
	TableSubsceneBeginning(DevicePtr device);
	TableSubsceneBeginning(const TableSubsceneBeginning&) = delete; // Delete unexpected copy constructor
	TableSubsceneBeginning& operator= (const TableSubsceneBeginning&) = delete; // Delete unexpected assign operator
	~TableSubsceneBeginning();
	void Render();
	virtual void skipEvent();
};

}
