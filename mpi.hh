/// \file
/// \brief Prototype for DSN::MPI class
///
/// \author Peter Hille (png!das-system) <peter@das-system-networks.de>
#ifndef DSNUTIL_MPI_HH
#define DSNUTIL_MPI_HH

#include <string>

#include <log4cpp/Category.hh>

#include "Singleton.hh"

namespace DSN {
  /// \brief Utility class for MPI initialization
  ///
  /// This class encapsulates the initialization and shutdown
  /// of the message passing interface (MPI) libraries in a singleton
  /// and provides some convenience methods to get information about
  /// the MPI job.
  ///
  /// \author Peter Hille (png!das-system) <peter@das-system-networks.de>
  class MPI : public Singleton<MPI> {
    friend class Singleton<MPI>;
  protected:
    MPI();
    ~MPI();

  public:
    bool initialize(int& argc, char**& argv);
    bool isInitialized();

    std::string processorName() const;
    int rank() const;
    int poolSize() const;

    void copyLogSettings(log4cpp::Category& l);

    /// \brief Output basic operating info on an ostream
    friend std::ostream& operator<<(std::ostream& o, const MPI& m) {
      return o << "[MPI:" << m.rank() << "/" << m.poolSize() << "@"
	       << m.processorName() << "]";
    }

  private:
    log4cpp::Category& log;
    std::string m_processorName;
    int m_rank;
    int m_poolSize;
  };
};

#endif // !DSNUTIL_MPI_HH
