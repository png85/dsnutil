/** \file
 *
 * \brief Implementation of DSN::CSV::Row
 *
 * This file implements a class to parse a single row of CSV data from an
 * input stream.
 */
#include <stdexcept>
#include <sstream>

#include "csv.hh"

using namespace DSN::CSV;


/** \brief Access parsed CSV cell data
 *
 * This overloaded operator can be used to access the parsed CSV cell data.
 * The index of the first cell is 0, the last one is (size() - 1).
 *
 * \param index Index to the parsed CSV cells
 *
 * \return Cell content as string
 *
 * \throw std::out_of range when the access index is greater than the number
 * of parsed CSV cells
 */
std::string const& Row::operator[](std::size_t index) const {
  if (index >= size()) {
    std::stringstream s;
    s << "Cell index " << index << " is out of range for DSN::CSV::Row";
    throw std::out_of_range(s.str());
  }
  return m_data[index];
}


/** \brief Get number of parsed cells
 *
 * Returns the number of parsed CSV cells in the last row that has been
 * read.
 *
 * \return Number of parsed CSV cells in the last row
 */
std::size_t Row::size() const {
  return m_data.size();
}


/** \brief Read CSV row from input stream
 *
 * Reads one row of CSV data from the given input stream and splits it into
 * cells that can be accessed through the subscript operator.
 *
 * \param s Input stream from which the CSV data shall be read
 */
void Row::readNext(std::istream& s) {
  std::string line;
  std::getline(s, line);

  std::stringstream ls(line);
  std::string cell;

  m_data.clear();
  while(std::getline(ls, cell, ','))
    m_data.push_back(cell);
}


/** \brief Read CSV row from input stream
 *
 * This operator calls Row::readNext(str) on given supplied Row instance.
 *
 * \see Row::readNext()
 *
 * \param str Input stream from which the CSV data shall be read
 * \param data Row instance that shall hold the parsed cell contents
 *
 * \return str after reading one line of CSV data from it
 */
std::istream& DSN::CSV::operator >>(std::istream& str, Row& data) {
  data.readNext(str);
  return str;
}

