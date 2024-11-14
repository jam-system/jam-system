#ifndef DBDIALOGS_H
#define DBDIALOGS_H


#include "jamdatabase.h"
#include <QDialog>

namespace Ui {
class dbDialogs;
}

class dbDialogs : public QDialog
{
    Q_OBJECT

public:
    explicit dbDialogs(QWidget *parent = nullptr);
    ~dbDialogs();




signals:
    void setReelFileId(int reelFileId);
    void setSceneFileId(int sceneFileId);
    void setParameters(int, int, int, int, int, int);

private slots:
    void on_reelsTable_clicked(const QModelIndex &index);
    void on_scenesTable_clicked(const QModelIndex &index);


    void on_btNewReel_clicked();

    void on_btNewScene_clicked();

    void on_btClose_clicked();

private:
    JamDb *db;
    Ui::dbDialogs *ui;
    int currentReelId;

    int currentSceneId;
    QString currentSceneDescr;
    int currentStartFrame;
    int currentEndFrame;
    int currentGain;
    int currentExposure;
    void pupulateScenesTable(int reelId);
    void createNewReelRecord();
    void createNewSceneRecord(int reelFileId);
    QSqlTableModel *rlmodel;
    QSqlTableModel *scmodel;
    int findNextIdNumber(const QString &table, const QString &column, int startId);
    int getReelFileId(int id);
};

#endif // DBDIALOGS_H
