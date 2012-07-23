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
    /// \brief Singleton template
    ///
    /// The Singleton template needs to be added as a friend so it can
    /// call the protected constructor.
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
    ///
    /// Writes a short summary string about this message passing
    /// instance to an ostream.
    ///
    /// \param o ostream to which the output shall get written
    /// \param m reference to an instance of this class
    ///
    /// \return o after writing the info string
    friend std::ostream& operator<<(std::ostream& o, const MPI& m) {
      return o << "[MPI:" << m.rank() << "/" << m.poolSize() << "@"
	       << m.processorName() << "]";
    }

  private:
    /// \brief Log category
    ///
    /// This is the log4cpp::Category instance used for log messages
    /// originating from this class.
    log4cpp::Category& log;

    /// \brief MPI processor name
    ///
    /// After a successful MPI initialization this contains the processor
    /// name (usually hostname) of the node that runs this instance of the
    /// class.
    std::string m_processorName;

    /// \brief MPI process rank
    ///
    /// After a successful MPI initialization this contains the rank of
    /// the current instance in the message passing pool.
    int m_rank;

    /// \brief MPI pool size
    ///
    /// After a successful MPI initialization this contains the total number
    /// of parallel jobs in the message passing pool.
    int m_poolSize;
  };
};

#endif // !DSNUTIL_MPI_HH
