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

// 16/05/08 Created by Dario Izzo.

#ifndef PAGMO_POPULATION_H
#define PAGMO_POPULATION_H

#include <boost/scoped_ptr.hpp>
#include <iostream>
#include <vector>

#include "../../config.h"
#include "../../exceptions.h"
#include "GOproblem.h"
#include "constants.h"
#include "individual.h"
#include "py_container_utils.h"
#include "rng.h"

/// Population class
/**
 * Population is a collection of co-evolving individuals with a bunch of useful functions.
 */
class __PAGMO_VISIBLE Population: public py_container_utils<Population> {
	
	/// Stream output operator.
	friend std::ostream __PAGMO_VISIBLE_FUNC &operator<<(std::ostream &, const Population &);

public:
	
	/// Constructor.
	/**
	 * Creates an empty population associated with the given problem.
	 * The operation internally creates an own copy of the problem object, so that two populations
	 * associated with identical problem can be safely used in different threads.
	 * \param[in] p Problem to be associated with the population.
	 */
	Population(const GOProblem& p);
	
	/// Constructor.
	/**
	 * Creates a population of a given size associated with the given problem.
	 * Individuals are generated randomly.
	 * The operation internally creates an own copy of the problem object, so that two populations
	 * associated with identical problem can be safely used in different threads.
	 * \param[in] p Problem to be associated with the population.
	 * \param[in] N Size of the population.
	 */
	Population(const GOProblem& p, int N);
	
	/// Copy constructor.
	/**
	 * Creates a deep copy of the population.
	 * The operation internally creates an own copy of the problem object and of the individuals,
	 * so that two populations can be safely used in different threads.
	 * \param[in] p Population to be duplicated.
	 */
	Population(const Population& p);
	
	/// Assignment operator
	/**
	 * Creates a deep copy of a population, dropping current contents of the population.
	 * The operation internally creates an own copy of the problem object and of the individuals,
	 * so that two populations can be safely used in different threads.
	 * Assignment is allowed only if the populations are associated with identical problems
	 * (in terms of the operator==  of the GOProblem class).
	 * \param[in] p Population to be duplicated.
	 */
	Population &operator=(const Population& p);
	
	
	//Collection interface functions
	
	/// Indexed const access operator
	const Individual& operator[](int index) const;
	
	/// Indexed access operator
	/**
	 * Please note that an explicit assignment is different from using the Population::setIndividual method,
	 * because the latter allows the right-hand individual to be out of problem bounds, while the former does not.
	 */
	Individual& operator[](int index);
	
	/// Forced set individual.
	/**
	 * Set an individual at the specified position in the population.
	 * The new individual is allowed to be out of problem bounds, and is fixed if necessary.
	 * Deep copy of the individual is created and stored.
	 * \param[in] idx Target position.
	 * \param[in] ind New individual.
	 */
	void setIndividual(int idx, const Individual &ind);
	
	/// Append an individual to the population.
	/**
	 * The new individual is allowed to be out of problem bounds, and is fixed if necessary.
	 * Deep copy of the individual is created and stored.
	 * \param[in] i New individual.
	 */
	void push_back(const Individual &i);
	
	/// Insert an uindividual at the specified position in the population, while extending the latter.
	/**
	 * The new individual is allowed to be out of problem bounds, and is fixed if necessary.
	 * Deep copy of the individual is created and stored.
	 * \todo The index_error is never thrown by the std::vector::insert. Fix the implementation.
	 * \param[in] i New individual.
	 */		
	void insert(int, const Individual &);
	
	/// Erase an individual at the specified position.
	/**
	 * \todo Here in turn, the index is range is not checked. Congrats.
	 */
	void erase(int n);
	
	/// Get the number of individuals in the population.
	size_t size() const;
	
	/// begin() iterator
	std::vector<Individual>::const_iterator begin() const { return pop.begin(); }
	
	/// end() iterator
	std::vector<Individual>::const_iterator end() const { return pop.end(); }
	
		
	// Getters and setters
	
	/// Get the problem associated with the population.
	const GOProblem &problem() const;
	
	/// Exposes the underlying vector of individuals.
	/**
	 * This is very useful when one wants to view Population just as a vector.
	 */
	const std::vector<Individual>& toVector() const { return pop; }
		
	
	// Utility functions.
	
	/// Calculate mean fitness of the individuals.
	double evaluateMean() const;	

	/// Calculate the standard deviation of the fitness of the individuals.
	double evaluateStd() const;
	
	/// Get the best individual from the population.
	/**
	 * \todo Rename 'extract' to 'get' because there's no extraction here.
	 * \todo This method has two hidden drawbacks: it's complexity if linear (i.e. bad) and it silently assumes minimisation problem.
	 */
	const Individual& extractBestIndividual() const;
	
	/// Get the worst individual from the population.
	/**
	 * \todo Same remarks as to extractBestIndividual apply here.
	 */
	const Individual& extractWorstIndividual() const;

	
	// Functions-that-definitely-shouldn't-be-here-but-are-left-alone-as-for-now.
	/// \todo Remove me!!!
	void replace_best(const Individual &);
	/// \todo Remove me!!!
	void replace_worst(const Individual &);
	/// \todo Remove me!!!
	/**
	 * \todo The presence of this function is especially dangerous, because calling it invalidates the indexes of individuals,
	 * which otherwise can be assumed to be individual's identifiers.
	 */
	void sort();
	/// \todo Remove me (replace with a general-purpose function which uses policies)!!!
	Population extractRandomDeme(int, std::vector<size_t> &);
	/// \todo Remove me (replace with a general-purpose function which uses policies)!!!
	void insertDeme(const Population &, const std::vector<size_t> &);
	/// \todo Remove me (replace with a general-purpose function which uses policies)!!!
	void insertBestInDeme(const Population &, const std::vector<size_t> &);
	/// \todo Remove me (replace with a general-purpose function which uses policies)!!!
	void insertDemeForced(const Population &, const std::vector<size_t> &);
	
private:
	
	// Private utility functions
	
	/// Check individual, and fix it if possible.
	/**
	 * \todo <b>This function made my day when I read it's implementation</b>. Jesus, this is a great example of
	 * bad programming. It can be well used as an excersise for students: find as much performance killers as you can.
	 * This is a classical example of an algorithm that is 'mathematically' correct, but extremely non-optimal.
	 * Such things make me vomit, really.
	 *
	 * Original doc:
	 * This function returns an individual based on input individual which has been checked for compatibility with the population.
	 * If there are no size mismatches and no boundaries mismatches (i.e., input's decision vector falls within problem's boundaries),
	 * then a copy of input is returned. If there is a size mismatch, an exception is raised. If there is a boundaries mismatch,
	 * then replace the out-of-boundaries values of the input decision vector with randomly-generated ones.
	 */
	Individual checked_individual(const Individual &) const;

	/// Generates number of random individuals and places them in the population.
	/**
	 * \param N Number of individuals to generate.
	 */
	void createRandomPopulation(int N);
	
	/// Extracts an index that makes fitness extremise a given functor.
	template <class Functor> size_t extract_most_index() const {
		if (pop.empty()) {
			pagmo_throw(index_error,"population is empty");
		}
		const Functor func = Functor();
		double f = pop[0].getFitness();
		size_t index = 0;
		const size_t size = pop.size();
		for (size_t i = 1; i < size; ++i) {
			if(func(pop[i].getFitness(),f)) {
				index = i;
				f = pop[i].getFitness();
			}
		}
		return index;
	}
	
	/// \todo Remove me!!!
	template <bool Forced> void ll_insert_deme(const Population &deme, const std::vector<size_t> &picks) {
		const size_t picks_size = picks.size(), pop_size = size();
		if (picks_size != deme.size()) {
			pagmo_throw(index_error,"mismatch between deme size and picks size while inserting deme");
		}
		for (size_t i = 0; i < picks_size; ++i) {
			if (picks[i] >= pop_size) {
				pagmo_throw(index_error,"pick value exceeds population's size while inserting deme");
			}
			if (Forced || deme.pop[i].getFitness() < pop[picks[i]].getFitness()) {
				pop[picks[i]] = deme.pop[i];
			}
		}
	}
	
	
	//Class fields
	
	std::vector<Individual>				pop; ///< Individuals container.	
	/// Associated problem.
	/**
	 * \todo I'm not sure if this should be a pointer, as we always want to have a copy of a GOProlem, to be thread-safe.
	 * Thus it seems logical to bind the lifetime of this copy of the GOProblem to the population's lifetime.
	 * Err... do we really want to be thread-safe here?
	 */
	boost::shared_ptr<const GOProblem>	m_problem;
};

/// Stream output operator.
std::ostream __PAGMO_VISIBLE_FUNC &operator<<(std::ostream &, const Population &);

#endif
