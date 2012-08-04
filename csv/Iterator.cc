/** \file
 *
 * \brief Implementation of DSN::CSV::Iterator
 *
 * This file implements an iterator-based interface to parse CSV data from
 * arbitrary input streams.
 */
#include "csv.hh"

using namespace DSN::CSV;

/** \brief Construct iterator for input stream
 *
 * Constructs a new Iterator instance that is ready to parse CSV data from
 * the given input stream.
 *
 * \param str Input stream from which the Iterator shall parse CSV data
 */
Iterator::Iterator(std::istream& str) :
  m_str(str.good() ? &str : NULL) {
  ++(*this);
}


/** \brief Construct empty iterator
 *
 * Cosntructs a new iterator without an underlying input stream which is
 * usually used in comparision operations when looping over another instance
 * of this class.
 */
Iterator::Iterator() : m_str(NULL) {}


/** \brief Prefix increment
 */
Iterator& Iterator::operator ++() {
  if (m_str) {
    (*m_str) >> m_row;
    m_str = m_str->good() ? m_str : NULL;
  }

  return (*this);
}


/** \brief Postfix increment
 */
Iterator Iterator::operator ++(int) {
  Iterator tmp(*this);
  ++(*this);
  return tmp;
}


/** \brief Access to last parsed row via reference
 *
 * Returns a reference to the Row instance containing the last parsed row
 * of CSV data.
 */
Row const& Iterator::operator *() const {
  return m_row;
}


/** \brief Access to last parsed row via pointer
 *
 * Returns a pointer to the Row instance containing the last parsed row
 * of CSV data.
 */
Row const* Iterator::operator ->() const {
  return &m_row;
}


/** \brief Equality operator
 *
 * Checks if this instance is equal to the given right hand side of the
 * comparision.
 *
 * \param rhs Iterator instance on the right hand side of the comparision
 */
bool Iterator::operator ==(Iterator const& rhs) {
  return ((this == &rhs) || 
	  ((this->m_str == NULL) && (rhs.m_str == NULL)));
}


/** \brief Inequality operator
 *
 * Checks if this instance is different to the given right hand side of
 * the comparision.
 *
 * \param rhs Iterator instance on the right hand side of the comparision
 */
bool Iterator::operator !=(Iterator const& rhs) {
  return !((*this) == rhs);
}
