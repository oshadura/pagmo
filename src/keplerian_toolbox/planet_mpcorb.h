/*****************************************************************************
 *   Copyright (C) 2004-2009 The PaGMO development team,                     *
 *   Advanced Concepts Team (ACT), European Space Agency (ESA)               *
 *   http://apps.sourceforge.net/mediawiki/pagmo                             *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers  *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits     *
 *   act@esa.int                                                             *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

#ifndef PLANET_MPCORB_H
#define PLANET_MPCORB_H

#include"planet.h"
#

namespace kep_toolbox{

/// Minor Planet (keplerian)
/**
 * This class derives from the planet class and allow to instantiate planets of
 * from the MPCORB database using their names or row id. The file MPCORB.DAT is searched
 * in the current directory.
 *
 * @author Dario Izzo (dario.izzo _AT_ googlemail.com)
 */

class planet_mpcorb : public planet
{
public:
	/**
	 * Construct a minor planet from its common name (e.g. EROS). Requires the file MPCORB.DAT
	 * is present in the current directory
	 * \param[in] name a string naming a minor planet (e.g. "eros" or "tu126" or "Apohis"). Case is ignored.
	 *		WARNING: partial names are matched to the first occurrence. So care needs to be taken that
			the string used actually uniquely defines the desired minor planet.
	 * \throws value_error if MPCORB.DAT is not found or name is not found in the file.
	 */
	planet_mpcorb(const std::string& name);

	/**
	 * Construct a minor planet from the row number in the MPCORB.DAT file counted from the
	 * first line containing an asetroid (i.e. line 0 is Ceres)
	 * \param[in] row integer indicating the row.
	 * \throws value_error if MPCORB.DAT is not found or row exceeds the asteroids in the file
	 */
	planet_mpcorb(int row);
private:
	static inline int packed_date2number(char c);
};


} /// End of namespace kep_toolbox
#endif // PLANET_MPCORB_H
