/*****************************************************************************
 *   Copyright (C) 2008, 2009 Advanced Concepts Team (European Space Agency) *
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

// 04/01/2009: Initial version by Francesco Biscani.

#ifndef PAGMO_ARCHIPELAGO_H
#define PAGMO_ARCHIPELAGO_H

#include <list>
#include <boost/thread/barrier.hpp>

#include "../../config.h"
#include "../problems/GOproblem.h"
#include "../algorithms/go_algorithm.h"
#include "island.h"
#include "py_container_utils.h"
#include "MigrationScheme.h"
#include "Migration.h"

/// The Archipelago class.
/** \todo rename. */
class __PAGMO_VISIBLE archipelago: public py_container_utils<archipelago> {
		
		typedef std::list<island> container_type; ///< Island container type abbreviation.
		typedef container_type::iterator iterator; ///< Island container iterator type abbreviation.
		typedef container_type::const_iterator const_iterator; ///< Island container const iterator type abbreviation.
		
		friend __PAGMO_VISIBLE_FUNC std::ostream &operator<<(std::ostream &, const archipelago &); ///< Stream output operator.
		template <class T> friend class py_container_utils; ///< \todo Document me!
		friend class island; ///< Island as a friend class. Dirty!
		
// Work around behaviour of GCC < 4.1, which does not recognize
// friendship with classes defined inside friend classes.
// TODO: remove support for gcc 3.4?
#if defined( __GNUC__ ) && GCC_VERSION < 401000
		friend struct island::int_evolver;
		friend struct island::t_evolver;
#endif

		/// \todo Document me!
		const_iterator begin() const {return m_container.begin();} 
		/// \todo Document me!
		const_iterator end() const {return m_container.end();}
		/// \todo Document me!
		iterator begin() {return m_container.begin();}
		/// \todo Document me!
		iterator end() {return m_container.end();}
		
	public:
		/// Default constructor.
		/**
		 * Creates an empty archipelago associated with the given problem.
		 * No migration between islands is assumed.
		 * \param[in] p problem to be associated with the archipelago.
		 */
		archipelago(const GOProblem& p);
		
		/// Constructor.
		/**
		 * Creates an empty archipelago associated with the given problem and having the migration scheme.
		 * \param[in] p problem to be associated with the archipelago.
		 * \param[in] _migrationScheme migration scheme of the new archipelago.
		 */
		archipelago(const GOProblem &p, const MigrationScheme& _migrationScheme);
		
		/// Constructor.
		/**
		 * Creates an archipelago with the given number of islands associated with the given problem and
		 * using the specified algorithm.
		 * No migration is assumed.
		 * \param[in] p problem to be associated with the archipelago.
		 * \param[in] a algorithm to be used by every island.
		 * \param[in] N number of islands to create.
		 * \param[in] M population size for each created island.
		 */
		archipelago(const GOProblem& p, const go_algorithm& a, int N, int M);
		
		/// Constructor.
		/**
		 * Creates an archipelago with the given number of islands associated with the given problem,
		 * using the specified algorithm and having the specified migration parameters.
		 * \param[in] p problem to be associated with the archipelago.
		 * \param[in] a algorithm to be used by every island.
		 * \param[in] N number of islands to create.
		 * \param[in] M population size for each created island.
		 * \param[in] migration migration parameters for the archipelago.
		 */
		archipelago(const GOProblem &p, const go_algorithm &a, int N, int M, const Migration& migration);
		
		/// Copy constructor.
		archipelago(const archipelago &);
		
		//Getters and setters
		const island &operator[](int) const;
		void set_island(int, const island &);
		
		const GOProblem &problem() const;
		
		/// Archipelago's migration scheme public getter (<b>synchronised</b>).
		/**
		 * Note that the method will throw an exception when there's no scheme associated with the archipelago.
		 */
		const MigrationScheme& getMigrationScheme() const;
		
		/// Archipelago's migration scheme setter (<b>synchronised</b>).
		/**
		 * A deep copy of the passed migration scheme is stored.
		 * All islands in the archipelago are registred in the new migration scheme.
		 * \param[in] newMigrationScheme migration scheme to be set in the archipelago.
		 */
		void setMigrationScheme(const MigrationScheme* newMigrationScheme);
		
		/// Underlying topology getter (<b>synchronised</b>).
		/**
		 * This method has been provided for the user's convenience so that it is possible to
		 * get the topology of the archipelago without an intermediate reference to the migration scheme.
		 * Note that the method will throw an exception when there's no migration scheme associated with the archipelago
		 * or when the scheme has no topology.
		 */
		const base_topology& getTopology() const;
		
		/// Underlying topology setter (<b>synchronised</b>).
		/**
		 * This method has been provided for the user's convenience so that it is possible to
		 * change the topology of the archipelago without an intermediate reference to the migration scheme.
		 * Note that the method will throw an exception when there's no migration scheme associated with the archipelago.
		 */
		void setTopology(const base_topology* newTopology);

		
		//Evolution functions
		
		/// Wait until all islands complete evolution.
		void join() const;
		/// Check if the evolution is still in progress.
		bool busy() const;
		
		/// Run the evolution for the given number of iterations
		/**
		 * \param[in] n Number of epochs to evolve on each island.
		 */
		void evolve(int n = 1);
		
		/// Run the evolution for the specified amount of time.
		/**
		 * \param[in] t Amount of time to evolve each island (in miliseconds).
		 */
		void evolve_t(const size_t& t);
		
		/// Add an island to the archipelago (<b>synchronised</b>).
		void push_back(const island &);
		
		/// Get the number of islands in the archipelago.
		size_t size() const;
		
		/// Get the best individual from the whole archipelago (<b>synchronised</b>)
		Individual best() const;
		
		/// Get the maximum total evolution time for all islands (<b>synchronised</b>).
		size_t getMaxEvoTime() const;
		
		/// Get the sum of total evolution times for all islands (<b>synchronised</b>).
		size_t getTotalEvoTime() const;

	protected:
		/// To be called by an island before the actual evolution starts.
		/** \see MigrationScheme::preEvolutionCallback */
		void preEvolutionCallback(island& _island) { if(migrationScheme) { migrationScheme->preEvolutionCallback(_island); } }

		/// To be called by an island after the actual evolution finishes.
		/** \see MigrationScheme::postEvolutionCallback */
		void postEvolutionCallback(island& _island) { if(migrationScheme) { migrationScheme->postEvolutionCallback(_island); } }
		
		/// To be called by an islands thread just before starting the evolution.
		/**
		 * This method allows synchronisation of all computational threads. All islands should call
		 * this method, which will block all of them until all threads are ready for computation.
		 */
		 void syncIslandStart() const;
		
	private:
		/// Check if the island is compatible with the archipelago
		/**
		 * Islands in the archipelago must be associated with the same problem as the archipelago.
		 * If the island is not compatible, an exception is thrown. 
		 */
		void check_island(const island &) const;
		
		container_type						m_container; ///< Island container.
		boost::shared_ptr<const GOProblem>	m_gop; ///< Problem associated with the archipelago.
		boost::shared_ptr<MigrationScheme>  migrationScheme; ///< Migration scheme of the archipelago. May be null, what means no migration.
	
		/// A barrier used to synchronise the start time of all islands.
		/**
		 * A pointer is used here because the ultimate number of islands is not known on archipelago creation.
		 * The object is created on each call to archipelago::evolve and archipelago::evolve_t
		 */
		boost::scoped_ptr<boost::barrier> islandsSyncPoint;

		
		/// Dummy assognment operator. Assignment is not a valid operation - throws an exception.
		archipelago &operator=(const archipelago &);
};

/// Stream output operator.
std::ostream __PAGMO_VISIBLE_FUNC &operator<<(std::ostream &, const archipelago &);

#endif
