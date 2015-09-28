/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medDatabaseDatasource.h"

#include <medDataManager.h>

#include <medDatabaseSearchPanel.h>
#include <medDatabaseView.h>

#include <medDatabaseProxyModel.h>
#include <medDatabaseModel.h>
#include <medDatabaseExporter.h>
#include <medDatabasePreview.h>
#include <medDatabaseCompactWidget.h>

#include <medActionsToolBox.h>

class medDatabaseDatasourcePrivate
{
public:
    QPointer<QWidget> mainWidget;
    QPointer<medDatabaseCompactWidget> compactWidget;

    medDatabaseModel *model;
    QPointer<medDatabaseView> largeView;
    medDatabaseView *compactView;

    medDatabasePreview *compactPreview;

    medDatabaseProxyModel *proxy;
    medDatabaseProxyModel *compactProxy;

    QList<medToolBox*> toolBoxes;
    medDatabaseSearchPanel *searchPanel;
    medActionsToolBox* actionsToolBox;

};

medDatabaseDatasource::medDatabaseDatasource( QWidget* parent ): medAbstractDatasource(parent),
    d(new medDatabaseDatasourcePrivate)
{
    d->model = new medDatabaseModel (this);
    d->proxy = new medDatabaseProxyModel(this);
    d->proxy->setSourceModel(d->model);

    d->compactProxy = new medDatabaseProxyModel(this);
    d->compactProxy->setSourceModel(d->model);
}

medDatabaseDatasource::~medDatabaseDatasource()
{
    delete d;
    d = NULL;
}

QWidget* medDatabaseDatasource::mainViewWidget()
{
    if(d->mainWidget.isNull())
    {
        d->mainWidget = new QWidget;
        d->largeView = new medDatabaseView(d->mainWidget);
        d->largeView->setModel(d->proxy);

        QVBoxLayout *database_layout = new QVBoxLayout(d->mainWidget);
        database_layout->setContentsMargins(0, 0, 0, 0);
        database_layout->setSpacing(0);
        database_layout->addWidget(d->largeView);

        connect(d->largeView, SIGNAL(open(const medDataIndex&)), this, SIGNAL(open(const medDataIndex&)));
        connect(d->largeView, SIGNAL(exportData(const medDataIndex&)), this, SIGNAL(exportData(const medDataIndex&)));
        connect(d->largeView, SIGNAL(dataRemoved(const medDataIndex&)), this, SIGNAL(dataRemoved(const medDataIndex&)));

        if(!d->toolBoxes.isEmpty())
        {
            connect(d->actionsToolBox, SIGNAL(removeClicked()), d->largeView, SLOT(onRemoveSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(exportClicked()), d->largeView, SLOT(onExportSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(viewClicked()), d->largeView, SLOT(onViewSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(saveClicked()), d->largeView, SLOT(onSaveSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(newPatientClicked()), d->largeView, SLOT(onCreatePatientRequested()));
            connect(d->actionsToolBox, SIGNAL(newStudyClicked()), d->largeView, SLOT(onCreateStudyRequested()));
            connect(d->actionsToolBox, SIGNAL(editClicked()), d->largeView, SLOT(onEditRequested()));

            connect(d->largeView, SIGNAL(patientClicked(const medDataIndex&)), d->actionsToolBox, SLOT(patientSelected(const medDataIndex&)));
            connect(d->largeView, SIGNAL(seriesClicked(const medDataIndex&)), d->actionsToolBox, SLOT(seriesSelected(const medDataIndex&)));
            connect(d->largeView, SIGNAL(noPatientOrSeriesSelected()), d->actionsToolBox, SLOT(noPatientOrSeriesSelected()));
        }

    }

    return d->mainWidget;
}

QWidget* medDatabaseDatasource::compactViewWidget()
{
    if(d->compactWidget.isNull())
    {
        d->compactWidget = new medDatabaseCompactWidget;
        d->compactView = new medDatabaseView(d->compactWidget);
        d->compactView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        d->compactView->setModel(d->compactProxy);
        d->compactPreview = new medDatabasePreview(d->compactWidget);

        d->compactWidget->setViewAndPreview(d->compactView, d->compactPreview);


        for(int i =1; i<12; ++i)
            d->compactView->hideColumn(i);

        connect(d->compactView, SIGNAL(patientClicked(const medDataIndex&)), d->compactPreview, SLOT(showPatientPreview(const medDataIndex&)));
        connect(d->compactView, SIGNAL(studyClicked(const medDataIndex&)), d->compactPreview, SLOT(showStudyPreview(const medDataIndex&)));
        connect(d->compactView, SIGNAL(seriesClicked(const medDataIndex&)), d->compactPreview, SLOT(showSeriesPreview(const medDataIndex&)));

        connect(d->compactPreview, SIGNAL(openRequest(medDataIndex)), d->compactView , SIGNAL(open(medDataIndex)));
        connect(d->compactView, SIGNAL(exportData(const medDataIndex&)), this, SIGNAL(exportData(const medDataIndex&)));
        connect(d->compactView, SIGNAL(dataRemoved(const medDataIndex&)), this, SIGNAL(dataRemoved(const medDataIndex&)));

    }
    return d->compactWidget;
}

QWidget* medDatabaseDatasource::sourceSelectorWidget()
{
    return new QWidget();
}

QString medDatabaseDatasource::tabName()
{
    return tr("Database");
}

QList<medToolBox*> medDatabaseDatasource::getToolBoxes()
{
    if(d->toolBoxes.isEmpty())
    {
        d->actionsToolBox = new medActionsToolBox(0, false);
        d->toolBoxes.push_back(d->actionsToolBox);

        d->searchPanel = new medDatabaseSearchPanel();
        d->searchPanel->setColumnNames(d->model->columnNames());
        d->toolBoxes.push_back(d->searchPanel);

        connect(d->searchPanel, SIGNAL(filter(const QString &, int)),this, SLOT(onFilter(const QString &, int)));

        if( !d->largeView.isNull())
        {
            connect(d->actionsToolBox, SIGNAL(removeClicked()), d->largeView, SLOT(onRemoveSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(exportClicked()), d->largeView, SLOT(onExportSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(viewClicked()), d->largeView, SLOT(onViewSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(saveClicked()), d->largeView, SLOT(onSaveSelectedItemRequested()));
            connect(d->actionsToolBox, SIGNAL(newPatientClicked()), d->largeView, SLOT(onCreatePatientRequested()));
            connect(d->actionsToolBox, SIGNAL(newStudyClicked()), d->largeView, SLOT(onCreateStudyRequested()));
            connect(d->actionsToolBox, SIGNAL(editClicked()), d->largeView, SLOT(onEditRequested()));

            connect(d->largeView, SIGNAL(patientClicked(const medDataIndex&)), d->actionsToolBox, SLOT(patientSelected(const medDataIndex&)));
            connect(d->largeView, SIGNAL(seriesClicked(const medDataIndex&)), d->actionsToolBox, SLOT(seriesSelected(const medDataIndex&)));
            connect(d->largeView, SIGNAL(noPatientOrSeriesSelected()), d->actionsToolBox, SLOT(noPatientOrSeriesSelected()));
        }
    }
    return d->toolBoxes;
}

QString medDatabaseDatasource::description(void) const
{
    return tr("Browse the medInria Database");
}

void medDatabaseDatasource::onFilter( const QString &text, int column )
{
    // adding or overriding filter on column
    d->proxy->setFilterRegExpWithColumn(QRegExp(text, Qt::CaseInsensitive, QRegExp::Wildcard), column);
}

void medDatabaseDatasource::warnOpeningFailed(medDataIndex const& index)
{
    emit openingFaild(index);
}

void medDatabaseDatasource::visualiseItem(medDataIndex const& index)
{
    if(item->dataIndex().isValidForSeries())
        emit open(item->dataIndex ());
}

void medDatabaseDatasource::exportItem(medDataIndex const& index)
{
    if(item)
        emit exportData(item->dataIndex());
}

void medDatabaseDatasource::removeItem(medDataIndex const& index)
{
    medDataIndex index = item->dataIndex();
    medDataManager::instance()->removeData(index);
}

void medDatabaseDatasource::saveItem(medDataIndex const& index)
{
    // Copy the data index, because the data item may cease to be valid.
    medDataIndex index = item->dataIndex();
    medDataManager *dataManager = medDataManager::instance();
    dataManager->makePersistent(dataManager->retrieveData(index));
}

void medDatabaseDatasource::createNewPatient(medDataIndex const& index)
{
    bool isPersistent = medDataManager::instance()->controllerForDatasource(
                item->dataIndex().dataSourceId())->isPersistent();

    QString patientName = "new patient";
    QString birthdate = "";
    QString gender = "";

    QList<QString> ptAttributes;
    ptAttributes << medMetaDataKeys::PatientName.label();
    ptAttributes << medMetaDataKeys::BirthDate.label();
    ptAttributes << medMetaDataKeys::Gender.label();

    QList<QVariant> ptValues;
    ptValues << patientName;
    ptValues << QString("");
    ptValues << QChar();

    medDatabaseEditItemDialog editDialog(ptAttributes, ptValues, this, true, isPersistent);

    int res =  editDialog.exec();
    if(res == QDialog::Accepted)
    {
        patientName = editDialog.value(medMetaDataKeys::PatientName.label()).toString();
        birthdate = editDialog.value(medMetaDataKeys::BirthDate.label()).toString();
        gender = editDialog.value(medMetaDataKeys::Gender.label()).toString();

        // TODO: Hack to be able to create patient using medAbstractData
        // Need to be rethought
        medAbstractData* medData = new medAbstractData();

        QString generatedPatientID = QUuid::createUuid().toString().replace ( "{","" ).replace ( "}","" );

        medData->addMetaData(medMetaDataKeys::PatientName.key(), QStringList() << patientName);
        medData->addMetaData(medMetaDataKeys::PatientID.key(), QStringList() << generatedPatientID);
        medData->addMetaData(medMetaDataKeys::BirthDate.key(), QStringList() << birthdate);
        medData->addMetaData(medMetaDataKeys::Gender.key(), QStringList() << gender);

        medDataManager::instance()->importData(medData, editDialog.isPersistent());
    }
}

void medDatabaseDatasource::createNewStudy(medDataIndex const& index)
{
    bool isPersistent = medDataManager::instance()->controllerForDatasource(
                item->dataIndex().dataSourceId() )->isPersistent();

    int patientNameIndex = item->attributes().indexOf(medMetaDataKeys::PatientName.key());
    int birthdateIndex = item->attributes().indexOf(medMetaDataKeys::BirthDate.key());
    QString patientName = item->data( patientNameIndex ).toString();
    QString birthdate = item->data( birthdateIndex ).toString();

    if(birthdate.isNull())
        birthdate="";

    QList<QString> stAttributes;
    stAttributes << medMetaDataKeys::StudyDescription.label();

    QList<QVariant> stValues;
    stValues << "new study";

    medDatabaseEditItemDialog editDialog(stAttributes, stValues, this, true, isPersistent);

    int res =  editDialog.exec();

    if(res == QDialog::Accepted)
    {
        QString studyName = editDialog.value(medMetaDataKeys::StudyDescription.label()).toString();

        // TODO: Hack to be able to create study using medAbstractData
        // Need to be rethought
        medAbstractData* medData = new medAbstractData();

        medData->addMetaData ( medMetaDataKeys::PatientName.key(), QStringList() << patientName );
        medData->addMetaData ( medMetaDataKeys::BirthDate.key(), QStringList() << birthdate );
        medData->addMetaData ( medMetaDataKeys::StudyDescription.key(), QStringList() << studyName );

        medData->addMetaData ( medMetaDataKeys::StudyID.key(), QStringList() << "0" );
        medData->addMetaData ( medMetaDataKeys::StudyDicomID.key(), QStringList() << "" );

        medDataManager::instance()->importData(medData, editDialog.isPersistent());
    }
}

void medDatabaseDatasource::editDatabaseItem(medDataIndex const& index)
{
    QList<QVariant> attributes = item->attributes();
    QList<QVariant> values = item->values();
    QList<QString> labels;

    for(QVariant const& attrib : attributes)
    {
        const medMetaDataKeys::Key* key =  medMetaDataKeys::Key::fromKeyName(attrib.toString().toStdString().c_str());
        if(key)
            labels << key->label();
        else labels << "";
    }

    medDatabaseEditItemDialog editDialog(labels,values,this);

    int res =  editDialog.exec();
    medDataIndex index = item->dataIndex();

    if(res == QDialog::Accepted)
    {
        int i=0;
        for(QString const& label : labels)
        {
            QVariant data = editDialog.value(label);
            QVariant variant = item->attribute(i);
            medDataManager::instance()->setMetadata(index, variant.toString(), data.toString());
            i++;
        }
    }
}
