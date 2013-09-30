/** \file
 *
 * \brief Prototypes for CSV parser classes
 *
 * This file contains the prototypes for all classes related to parsing CSV
 * files.
 */
#ifndef DSNUTIL_CSV_HH
#define DSNUTIL_CSV_HH 1

#include <iterator>
#include <iostream>
#include <vector>
#include <string>

namespace DSN {
  /** \brief CSV file parsing classes
   *
   * This namespace contains classes and utility functions to parse CSV
   * (comma separated values) files.
   */
  namespace CSV {
    /** \brief Row in a CSV file
     *
     * This class represents one row in a CSV file. It can be used read a
     * line of CSV data from an \a std::istream and then access the cell
     * contents via its overloaded subscript operator.
     *
     * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
     *
     * \version 1.0 Initial release
     */
    class Row {
    public:
      std::string const& operator[](std::size_t index) const;
      std::size_t size() const;
      void readNext(std::istream& s);

    private:
      /** \brief CSV cell data
       *
       * This vector contains the content of the cells after parsing a line
       * of CSV data in \a readNext()
       */
      std::vector<std::string> m_data;
    };

    std::istream& operator >>(std::istream& s, Row& data);

    /** \brief Iterator for CSV data streams
     *
     * This class implements an iterator over streams of CSV data.
     *
     * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
     *
     * \version 1.0 Initial release
     */
    class Iterator {
    public:
      typedef std::input_iterator_tag iterator_category;
      typedef Row value_type;
      typedef std::size_t difference_type;
      typedef Row* pointer;
      typedef Row& reference;

      Iterator(std::istream& str);
      Iterator();

      Iterator& operator ++();
      Iterator operator ++(int);

      Row const& operator *() const;
      Row const* operator ->() const;

      bool operator ==(Iterator const& rhs);
      bool operator !=(Iterator const& rhs);

    private:
      /** \brief Input stream for CSV data
       *
       * This input stream is used to read the CSV data that gets parsed
       * by this iterator.
       */
      std::istream* m_str;

      /** \brief CSV row data
       *
       * This holds the data of the last row of CSV data parsed by this
       * iterator.
       */
      Row m_row;
    };
  }
}

#endif // !DSNUTIL_CSVROW_HH
