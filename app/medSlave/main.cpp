/* main.cpp ---
 *
 * Author: Nicolas Niclausse
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */


#include <dtkLog/dtkLog.h>

#include <medComposer/medComposerFactory.h>

#include <dtkCore>

#include <dtkComposer/dtkComposerEvaluatorSlave.h>

#include <dtkDistributed/dtkDistributedCommunicator.h>

#if defined(DTK_HAVE_MPI) && defined(DTK_BUILD_MPI)
#include <dtkDistributed/dtkDistributedCommunicatorMpi.h>
#endif

#include <dtkDistributed/dtkDistributedCommunicatorTcp.h>

#include <QtCore>

int main(int argc, char **argv)
{

    if(argc != 2) {
        qDebug() << "Usage: " << argv[0] << " <server>";
        return 0;
    }

    QUrl url = QUrl(argv[1]);
    bool useGUI = false;

    QSettings settings("inria", "medinria");
    settings.beginGroup("slave");

    if (settings.contains("log_level"))
        dtkLogger::instance().setLevel(settings.value("log_level").toString());
    else
        dtkLogger::instance().setLevel(dtkLog::Trace);

    if (settings.contains("use_gui") && settings.value("use_gui").toString() == "true")
        useGUI = true;

    QApplication application(argc, argv, useGUI);
    application.setApplicationName("medSlave");
    application.setApplicationVersion("0.0.1");
    application.setOrganizationName("inria");
    application.setOrganizationDomain("fr");

    dtkLogger::instance().attachFile(dtkLogPath(&application));

    dtkPluginManager::instance()->initialize();

#if defined(DTK_HAVE_MPI) && defined(DTK_BUILD_MPI)
    dtkDistributedCommunicatorMpi *communicator =  new dtkDistributedCommunicatorMpi;
#else
    dtkDistributedCommunicatorTcp *communicator =  new dtkDistributedCommunicatorTcp;
#endif
    communicator->initialize();

    dtkComposerEvaluatorSlave *slave = new dtkComposerEvaluatorSlave;
    medComposerFactory *factory = new medComposerFactory;
    slave->setInternalCommunicator(communicator);
    slave->setServer(url);
    slave->setFactory(factory);

    int value;
    do  { value = slave->exec(); } while (value  == 0);

    dtkPluginManager::instance()->uninitialize();

    if (communicator->initialized())
        communicator->uninitialize();

    return value;
}
