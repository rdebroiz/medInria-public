#include <medAbstractLayeredView.h>

#include <dtkCore/dtkSmartPointer.h>

#include <medAbstractData.h>
#include <medAbstractLayeredViewInteractor.h>
#include <medLayeredViewFactory.h>


class medAbstractLayeredViewPrivate
{
public:
    QList < dtkSmartPointer <medAbstractData> > layersDataList;
    QHash<dtkSmartPointer<medAbstractData>,  medAbstractLayeredViewInteractor*> primaryIntercatorsHash;
    QHash<dtkSmartPointer<medAbstractData>,  QList<medAbstractInteractor*> > extraIntercatorsHash;

    medAbstractLayeredViewNavigator* primaryNavigator;
    QList<medAbstractNavigator*> extraNavigators;

};

medAbstractLayeredView::medAbstractLayeredView(QObject *parent) : medAbstractView(parent), d (new medAbstractLayeredViewPrivate)
{
    d->primaryNavigator = NULL;
}

medAbstractLayeredView::~medAbstractLayeredView()
{
    delete d;
}

void medAbstractLayeredView::removeInteractors(medAbstractData *data)
{
    d->primaryIntercatorsHash.remove(data);
    d->extraIntercatorsHash.remove(data);
}

void medAbstractLayeredView::initialiseInteractors(medAbstractData *data)
{
    // primary

    medLayeredViewFactory* factory = medLayeredViewFactory::instance();
    QStringList primaryInt = factory->interactorsAbleToHandle(this->identifier(), data->identifier());
    if(primaryInt.isEmpty())
    {
        qWarning() << "Unable to find any primary interactor for: " << this->identifier() << "and" << data->identifier();
         d->primaryIntercatorsHash.insert(data, NULL);
    }
    else
    {
        medAbstractLayeredViewInteractor* interactor = factory->createInteractor(primaryInt.first(), this);
        interactor->setData(data);
        d->primaryIntercatorsHash.insert(data, interactor);
    }

    // extra
    QStringList extraInt = factory->additionalInteractorsAbleToHandle(this->identifier(), data->identifier());
    if(!extraInt.isEmpty())
    {
        QList<medAbstractInteractor*> extraIntList;
        foreach (QString i, extraInt)
        {
            medAbstractInteractor* interactor = factory->createAdditionalInteractor(i, this);
            interactor->setData(data);
            extraIntList << interactor;
        }
        d->extraIntercatorsHash.insert(data, extraIntList);
    }
}

void medAbstractLayeredView::initialiseNavigators()
{
    // primary
    medLayeredViewFactory* factory = medLayeredViewFactory::instance();
    QStringList primaryNav = factory->navigatorsAbleToHandle(this->identifier());
    if(primaryNav.isEmpty())
    {
        qWarning() << "Unable to find any primary navigator for: " << this->identifier();
        d->primaryNavigator = NULL;

    }
    else
        d->primaryNavigator = factory->createNavigator(primaryNav.first(), this);

    // extra
    QStringList extraNav = factory->additionalNavigatorsAbleToHandle(this->identifier());
    if(!extraNav.isEmpty())
    {
        foreach (QString n, extraNav)
        {
               d->extraNavigators << factory->createAdditionalNavigator(n, this);
        }
    }
}

medAbstractLayeredViewInteractor* medAbstractLayeredView::primaryInteractor(medAbstractData* data)
{
    if(d->primaryIntercatorsHash.isEmpty())
        return NULL;

    return d->primaryIntercatorsHash.value(data);
}

QList<medAbstractInteractor*> medAbstractLayeredView::extraInteractors(medAbstractData* data)
{
    return d->extraIntercatorsHash.value(data);
}

medAbstractLayeredViewInteractor* medAbstractLayeredView::primaryInteractor(unsigned int layer)
{
    return d->primaryIntercatorsHash.value(this->data(layer));
}

QList<medAbstractInteractor*> medAbstractLayeredView::extraInteractors(unsigned int layer)
{
    return d->extraIntercatorsHash.value(this->data(layer));
}

medAbstractLayeredViewNavigator* medAbstractLayeredView::primaryNavigator()
{
    return d->primaryNavigator;
}

QList<medAbstractNavigator*> medAbstractLayeredView::extraNavigators()
{
    return d->extraNavigators;
}

void medAbstractLayeredView::addLayer(medAbstractData *data)
{
    if(!data)
    {
        qWarning() << "Attempt to add a NULL data to the view: " << this;
        return;
    }

    if ( this->contains(data))
    {
        qDebug() << "Attempt to add twice the same data to the view: " << this;
        return;
    }

    d->layersDataList << data;
    initialiseInteractors(data);
}

QList<dtkSmartPointer<medAbstractData> > medAbstractLayeredView::data() const
{
    return d->layersDataList;
}

unsigned int medAbstractLayeredView::layer(medAbstractData * data)
{
    return d->layersDataList.indexOf(data);
}

bool medAbstractLayeredView::removeData(medAbstractData *data)
{
    int res = d->layersDataList.removeAll(data);
    this->removeInteractors(data);
    return res > 0;
}

void medAbstractLayeredView::removeLayer(unsigned int layer)
{
    d->layersDataList.removeAt(layer);
    this->removeInteractors(this->data(layer));
}

void medAbstractLayeredView::insertLayer(unsigned int layer, medAbstractData *data)
{
    d->layersDataList.insert(layer, data);
    this->initialiseInteractors(data);
}

void medAbstractLayeredView::moveLayer(unsigned int fromLayer, unsigned int toLayer)
{
    d->layersDataList.move(fromLayer, toLayer);
}

medAbstractData * medAbstractLayeredView::data(unsigned int layer) const
{
    if (layer > (unsigned int)d->layersDataList.size())
    {
        qWarning() << "Unable to retreive data at layer:" <<layer << "from: "<< this->description();
        return NULL;
    }
    return d->layersDataList[layer];
}

bool medAbstractLayeredView::contains(medAbstractData * data) const
{
    return d->layersDataList.contains(data);
}

unsigned int medAbstractLayeredView::layersCount() const
{
    return d->layersDataList.count();
}


void medAbstractLayeredView::setVisibility(bool visibility, unsigned int layer)
{
    medAbstractLayeredViewInteractor* inter = this->primaryInteractor(layer);
    if(!inter)
        return;

    inter->setVisibility(visibility);
    emit visibilityChanged(visibility, layer);
}

bool medAbstractLayeredView::visibility(unsigned int layer)
{
    medAbstractLayeredViewInteractor* inter = this->primaryInteractor(layer);
    if(!inter)
        return true;

    return inter->visibility();
}
