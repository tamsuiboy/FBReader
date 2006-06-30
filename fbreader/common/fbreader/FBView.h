/*
 * FBReader -- electronic book reader
 * Copyright (C) 2004-2006 Nikolay Pultsin <geometer@mawhrin.net>
 * Copyright (C) 2005 Mikhail Sobolev <mss@mawhrin.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __FBVIEW_H__
#define __FBVIEW_H__

#include "../textview/TextView.h"

class FBReader;

class FBView : public TextView {

public:
	FBView(FBReader &reader, ZLPaintContext &context);

	bool onFingerTap(int x, int y);

protected:
	FBReader &myReader;
};

inline FBView::FBView(FBReader &reader, ZLPaintContext &context) : TextView(context), myReader(reader) {}

#endif /* __FBVIEW_H__ */
