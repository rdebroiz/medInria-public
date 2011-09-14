// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include "itkDataDiffusionGradientList.h"
#include "itkDataDiffusionGradientListPlugin.h"

#include <dtkCore/dtkLog.h>

#include <dtkItkLogForwarder/itkLogForwarder.h>

// /////////////////////////////////////////////////////////////////
// itkDataDiffusionGradientListPluginPrivate
// /////////////////////////////////////////////////////////////////

class itkDataDiffusionGradientListPluginPrivate
{
public:
  itkDataDiffusionGradientListPluginPrivate()
  {
      forwarder = itk::LogForwarder::New();
  }
  ~itkDataDiffusionGradientListPluginPrivate()
  {

  }
  // Class variables go here.
  itk::LogForwarder::Pointer forwarder;
};

// /////////////////////////////////////////////////////////////////
// itkDataDiffusionGradientListPlugin
// /////////////////////////////////////////////////////////////////

itkDataDiffusionGradientListPlugin::itkDataDiffusionGradientListPlugin(QObject *parent) : dtkPlugin(parent), d(new itkDataDiffusionGradientListPluginPrivate)
{

}

itkDataDiffusionGradientListPlugin::~itkDataDiffusionGradientListPlugin(void)
{
    delete d;

    d = NULL;
}

bool itkDataDiffusionGradientListPlugin::initialize(void)
{
    if(!itkDataDiffusionGradientList::registered())
	dtkWarning() << "Unable to register itkDataDiffusionGradientList type";

    return true;
}

bool itkDataDiffusionGradientListPlugin::uninitialize(void)
{
    return true;
}

QString itkDataDiffusionGradientListPlugin::name(void) const
{
    return "itkDataDiffusionGradientListPlugin";
}

QString itkDataDiffusionGradientListPlugin::description(void) const
{
    return tr("List of gradients for Diffusion Weighted Images");
}

QString itkDataDiffusionGradientListPlugin::identifier(void) const
{
    return "itkDataDiffusionGradientListPlugin";
}

QStringList itkDataDiffusionGradientListPlugin::tags(void) const
{
    return QStringList() << "itk" << "data" << "diffusion" << "gradients";
}

QStringList itkDataDiffusionGradientListPlugin::types(void) const
{
    return QStringList() << "itkDataDiffusionGradientList";
}

Q_EXPORT_PLUGIN2(itkDataDiffusionGradientListPlugin, itkDataDiffusionGradientListPlugin)
