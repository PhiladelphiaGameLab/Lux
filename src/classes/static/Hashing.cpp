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

#include "Hashing.h"

int Hashes::LBHM2Input(MapLocationDS data){
	return data.val1%data.maxC + (data.val2%data.maxR)*data.maxC;
}


int Hashes::LBHMFindBGT(MapLocationDS data){
	return 1;  //TODO: FIGURE OUT HOW TO DO THIS HASHING
}