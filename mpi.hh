/** \file
 *
 * \brief Prototypes for DSN::MPI related classes
 *
 * This file implements the prototypes and templates within the DSN::MPI
 * namespace which contains utility classes and templates to speed up the
 * development of applications utilizing the message passing interface.
 *
 * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
 *
 * \version 1.1 Added functor-based p2p communication
 *
 * \version 1.0 Initial release
 */
#ifndef DSNUTIL_MPI_HH
#define DSNUTIL_MPI_HH

#include <string>

#include <log4cpp/Category.hh>

#include <mpi.h>

#include "Singleton.hh"

namespace DSN {
/** \brief Message Passing Interface utility classes
 *
 * This namespace contains a bunch of classes and templates to speed up
 * development of applications using the Message Passing Interface (MPI)
 * libraries.
 */
namespace MPI {
/** \brief Message Passing job information
 *
 * This class encapsulates the initialization and shutdown of the message
 * passing interface in a singleton and provides some convenience methods
 * to get information about the executed job.
 *
 * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
 */
class Job : public DSN::Singleton<Job> {
    /** \brief Singleton template
     *
     * The \a Singleton template needs to be added as a friend so it
     * can call the protected constructor.
     */
    friend class DSN::Singleton<Job>;
protected:
    Job();
    ~Job();

public:
    bool initialize(int& argc, char**& argv);
    bool isInitialized();

    std::string processorName() const;
    int rank() const;
    int poolSize() const;

    void copyLogSettings(log4cpp::Category& l);

    /** \brief Output job info on an ostream
     *
     * Writes a short summary string about this message passing instance
     * to the given ostream.
     *
     * \param o ostream to which the output shall get written
     * \param m reference to an instance of this class
     *
     * \return o after writing the informational string
     */
    friend std::ostream& operator<<(std::ostream& o, const Job& m) {
        return o << "[MPI Job #" << m.rank() << "/" << m.poolSize() << "@"
                 << m.processorName() << "]";
    }

    const ::MPI::Intracomm& world() const;

private:
    /** \brief log4cpp logging category
     *
     * This is the log4cpp::Category isntance used for log messages
     * originating from this class.
     */
    log4cpp::Category& log;

    /** \brief Processor name for this job
     *
     * After a successful initialization of the message passing interface
     * this contains the processor name (= usually hostname) of the node
     * that runs this instance of the class.
     */
    std::string m_processorName;

    /** \brief Rank of this job
     *
     * After a successful initialization of the message passing interface
     * this contains the rank (= job #) of this instance of the class.
     */
    int m_rank;

    /** \brief Total number of jobs
     *
     * After a successful initialization of the message passing interface
     * this contains the total number of jobs in the MPI::COMM_WORLD pool.
     */
    int m_poolSize;
};

/** \brief Interface for Transmission functors
 *
 * This abstract base class defines the interface that all functors
 * for the various transmission calls of the message passing interface
 * must implement.
 */
class TransmitFunctor {
public:
    virtual void operator()(const void* data, int count,
                            const ::MPI::Datatype& type,
                            int destination, int tag) const = 0;
};

/// \brief Functor for MPI::BSend
class BSend : public TransmitFunctor {
public:
    virtual void operator()(const void* data, int count,
                            const ::MPI::Datatype& type,
                            int destination, int tag) const;
};

/// \brief Functor for MPI::SSend
class SSend : public TransmitFunctor {
public:
    virtual void operator()(const void* data, int count,
                            const ::MPI::Datatype& type,
                            int destination, int tag) const;
};

/// \brief Functor for MPI::RSend
class RSend : public TransmitFunctor {
public:
    virtual void operator()(const void* data, int count,
                            const ::MPI::Datatype& type,
                            int destination, int tag) const;
};

/// \brief Functor for MPI::Send
class Send : public TransmitFunctor {
public:
    virtual void operator()(const void* data, int count,
                            const ::MPI::Datatype& type,
                            int destination, int tag) const;
};

/** \brief Reception functor
 *
 * This functor is being used to receive data in point-to-point
 * communication between message passing jobs.
 *
 * \note Note that unlike the transmission functors this is not an
 * abstract base class since there is only one recv() function available
 */
class ReceiveFunctor {
public:
    void operator()(void* data, int count, const ::MPI::Datatype& type,
                    int source, int tag, ::MPI::Status& status) const;
};

/** \brief Functor based transceiver for P2P communication
 *
 * This template implements an easy-to-use functor based interface
 * to send/receive data in point-to-point based message passing
 * communication.
 *
 * \param T \a TransmitFunctor class to use for this transceiver
 * \param R \a ReceiveFunctor class to use for this transceiver
 */
template <typename T=Send, typename R=ReceiveFunctor>
class Transceiver {
public:
    /** \brief Send data using functor
     *
     * Sends data using the functor specified when the template was
     * instanced.
     */
    void send(const void* data, int count, const ::MPI::Datatype& type,
              int destination, int tag) {
        T()(data, count, type, destination, tag);
    };

    /** \brief Receive data using functor
     *
     * Receives data using the functor specivied when the template
     * was instanced.
     */
    void recv(void* data, int count, const ::MPI::Datatype& type,
              int source, int tag, ::MPI::Status& status) {
        R()(data, count, type, source, tag, status);
    };
};
}
}

#endif // !DSNUTIL_MPI_HH
