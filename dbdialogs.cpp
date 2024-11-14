#include "dbdialogs.h"
#include "ui_dbdialogs.h"
#include <QDebug>

dbDialogs::dbDialogs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dbDialogs)
{
    ui->setupUi(this);

    db = new JamDb();
    db->initDatabase();

    //db->dropTables();
   // db->createTables();

    scmodel = new QSqlTableModel(this);
    scmodel->setEditStrategy(QSqlTableModel::OnFieldChange);
    rlmodel = new QSqlTableModel(this);
    //scmodel->setEditStrategy(QSqlTableModel::OnFieldChange);
    rlmodel->setTable("reels");
    rlmodel->select();
    ui->reelsTable->setModel(rlmodel);

    //ui->reelsTable->setColumnWidth(0, 50);
    ui->reelsTable->setColumnHidden(0, true);
    ui->reelsTable->setColumnWidth(1, 70);
    ui->reelsTable->setColumnWidth(2, 200);

}

dbDialogs::~dbDialogs()
{
    delete ui;
}


int dbDialogs::getReelFileId(int id)
{
    QSqlQuery query;
    query.prepare("SELECT reelFileId FROM reels WHERE reelId = " + QString::number(id));
    if(query.exec())
    {
        query.first();
        qDebug() << query.value("reelFileId").toString();
        return query.value("reelFileId").toString().toInt();
    }else
    {

        qDebug() << "getReelFileId - ERROR: " << query.lastError().text();
        return -1;
    }
}

void dbDialogs::pupulateScenesTable(int reelId)
{
    // Define the SQL query for specific columns
    scmodel->setQuery("SELECT * FROM scenes where reelId = " + QString::number(reelId));

    if (scmodel->lastError().isValid()) {
        qDebug() << "Query Error:" << scmodel->lastError();
    }
    //scmodel->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->scenesTable->setModel(scmodel);
}



void dbDialogs::on_reelsTable_clicked(const QModelIndex &index)
{
    QModelIndexList selectedRows = ui->reelsTable->selectionModel()->selectedRows();
    if (selectedRows.isEmpty())
        return;
    int row = selectedRows.first().row();

    QVariant datam;
    // Retrieve data from each column in the selected row
    QString rowData;
    for (int col = 0; col < rlmodel->columnCount(); ++col) {
        QString columnName = rlmodel->headerData( col, Qt::Horizontal ).toString();
        datam = rlmodel->data(rlmodel->index(row, col));
        qDebug() << "columnName" << columnName;
        qDebug() << "currentReelId" << datam;

    }
    currentReelId = datam.toInt();
    pupulateScenesTable(currentReelId);
    emit setReelFileId(currentReelId);

    qDebug() << getReelFileId(2);
}


void dbDialogs::on_scenesTable_clicked(const QModelIndex &index)
{
    QModelIndexList selectedRows = ui->scenesTable->selectionModel()->selectedRows();
    if (selectedRows.isEmpty())
        return;
    int row = selectedRows.first().row();
    qDebug() << "row" << row;
    // Retrieve data from each column in the selected row
    int rowArray[6];
    // qDebug() << "scmodel->columnCount()" << scmodel->columnCount();
    // for (int col = 2; col < scmodel->columnCount(); ++col) {
    //     rowArray[col-2] = scmodel->data(scmodel->index(row, col)).toInt();
    //     qDebug() << "data" << rowArray[col-2];
    // }
    // qDebug() << "rowArray" << rowArray;
    // emit setSceneFileId(rowArray[0]);

    currentSceneId    = scmodel->data(scmodel->index(row, 0)).toInt();
    currentSceneDescr = scmodel->data(scmodel->index(row, 1)).toString();
    currentReelId     = scmodel->data(scmodel->index(row, 2)).toInt();
    currentSceneId    = scmodel->data(scmodel->index(row, 3)).toInt();
    currentStartFrame = scmodel->data(scmodel->index(row, 4)).toInt();
    currentEndFrame   = scmodel->data(scmodel->index(row, 5)).toInt();
    currentGain       = scmodel->data(scmodel->index(row, 6)).toInt();
    currentExposure   = scmodel->data(scmodel->index(row, 7)).toInt();
    qDebug() << "Sending: " << currentReelId<< currentSceneId<< currentStartFrame<< currentEndFrame<<
        currentGain<< currentExposure  ;

    emit setParameters(currentReelId, currentSceneId, currentStartFrame, currentEndFrame,
                       currentGain, currentExposure  );
}

int dbDialogs::findNextIdNumber(const QString &table, const QString &column, int startId)
{
    QSqlQuery query;
    int numberOfRows = 0;
    int newFileId;

    query.exec("SELECT * FROM " + table);
    if(query.last())
    {
        numberOfRows =  query.at() + 1;
    }
    qDebug() << "Number of Rows: " << numberOfRows;

    if (numberOfRows > 0){
        if (!query.exec("SELECT MAX(" + column + ") FROM "+ table )) {
            qDebug() << "Error executing query: " << query.lastError().text();
        }
        query.first();
        qDebug() << query.value(0).toInt();
        newFileId = query.value(0).toInt() + 1;
    }else //numberOfRows = 0
    {
        newFileId = startId;
    }
    qDebug() << "newFileId: " << newFileId ;
    return newFileId;
}

void dbDialogs::createNewReelRecord()
{
    int newReelFileId = findNextIdNumber("reels", "reelFileId", 2000);
    QSqlQuery query;
    query.prepare("INSERT INTO reels (reelFileId, reelDescr) VALUES (:reelFileId, :reelDescr)");
    query.bindValue(":reelFileId", newReelFileId);
    query.bindValue(":reelDescr",  "");

    if(!query.exec()){
        qDebug() << "Insert into style_detail - ERROR: " << query.lastError().text();
        return;
    }
    rlmodel->select();
    ui->reelsTable->scrollToBottom();
}

void dbDialogs::createNewSceneRecord(int reelId)
{
    int newSceneFileId = findNextIdNumber("scenes", "sceneFileId", 5000);
    QSqlQuery query;
    query.prepare("INSERT INTO scenes (reelId, sceneFileId, sceneDescr, startFrame, endFrame, gain, exposure) "
                            " VALUES (:reelId, :sceneFileId, :sceneDescr, :startFrame, :endFrame, :gain, :exposure)");

    query.bindValue(":reelId",  reelId);
    query.bindValue(":sceneFileId",  newSceneFileId);
    query.bindValue(":sceneDescr",  "");
    query.bindValue(":startFrame",   0);
    query.bindValue(":endFrame",     0);
    query.bindValue(":gain",         0);
    query.bindValue(":exposure",     0);

    if(!query.exec()){
        qDebug() << "Insert into scenes - ERROR: " << query.lastError().text();
        return;
    }
    pupulateScenesTable(reelId);

}

void dbDialogs::on_btNewReel_clicked()
{
    createNewReelRecord();
}


void dbDialogs::on_btNewScene_clicked()
{
    createNewSceneRecord(currentReelId);
}


void dbDialogs::on_btClose_clicked()
{
    dbDialogs::close();
}

