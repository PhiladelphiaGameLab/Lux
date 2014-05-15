#ifndef LBHM_H_INCLUDED
#define LBHM_H_INCLUDED

/*
 * This code was orginally written and developed by the Lux Backend Team
 * at Philadelphia Game Lab:
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * - Mike Oak
 */

#include <stdlib.h>
#include <list>
#include "socket.h"

template<class T>
class LocData{
	public:
		int expire;
		T data;
		LocData(int exp, T data);
		LocData(T data);
		~LocData();
};


template <class T>
class HMBL{
	private:
		int mapwidth;
		int mapheight;
		int columns;
		int rows;
		std::list<LocData<T> >* hashTable;

		void copyIntoList(std::list<LocData<T> >* into, const std::list<LocData<T> > from);

	public:
		HMBL(int mapw, int maph, int col, int row);
		HMBL();
		~HMBL();

		std::list<LocData<T> >* getSocketLists(int loc);
		std::list<LocData<T> > getIndex(int idx);
		std::list<LocData<T> >* getSockets();
		
		void add(int loc, T value);
		void removeExpiredObjects();

		std::list<LocData<T> >& operator[](int idx);
  		const std::list<LocData<T> >& operator[](int idx) const;
};

#endif // LBHM_H_INCLUDED
