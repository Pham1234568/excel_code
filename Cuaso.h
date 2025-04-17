#ifndef CUASO_H
#define CUASO_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QTableWidget>
#include "xlsxdocument.h"
#include <QStandardItemModel>
class Cuaso : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filepath1 READ filepath1 WRITE setFilepath1 NOTIFY filepath1Changed)
    Q_PROPERTY(QString filepath2 READ filepath2 WRITE setFilepath2 NOTIFY filepath2Changed)
    Q_PROPERTY(QString exportStatus READ exportStatus WRITE setExportStatus NOTIFY exportStatusChanged)
    Q_PROPERTY(QAbstractItemModel* file1Model READ file1Model NOTIFY file1ModelChanged)
    Q_PROPERTY(QAbstractItemModel* file2Model READ file2Model NOTIFY file2ModelChanged)
    Q_PROPERTY(QAbstractItemModel* compareModel READ compareModel NOTIFY compareModelChanged)
    Q_PROPERTY(QAbstractItemModel* diffModel READ diffModel NOTIFY diffModelChanged)
    Q_PROPERTY(double totalCells READ totalCells NOTIFY totalCellsChanged)
    Q_PROPERTY(double diffCount READ diffCount NOTIFY diffCountChanged)
public:
    explicit Cuaso(QObject *parent = nullptr);
    ~Cuaso();
    QString filepath1() const;
    void setFilepath1(const QString &filepath1);

    QString filepath2() const;
    void setFilepath2(const QString &filepath2);

    QString exportStatus() const;
    void setExportStatus(const QString &exportStatus);
    Q_INVOKABLE void exportDifferences();
    Q_INVOKABLE void loadFile1();
    Q_INVOKABLE void loadFile2();
    Q_INVOKABLE void compareSheets();
    Q_INVOKABLE void exportQm(const QString &directory);
    QAbstractItemModel* file1Model() const { return m_file1Model; }
    QAbstractItemModel* file2Model() const { return m_file2Model; }
    QAbstractItemModel* compareModel() const { return m_compareModel; }
    QAbstractItemModel* diffModel() const { return m_diffModel; }
    double totalCells() const { return m_totalCells; }
    double diffCount() const { return m_diffCount; }
    void saveLastSession();
    void loadLastSession();
    Q_INVOKABLE void resetWindow();


signals:
    void filepath1Changed();
    void filepath2Changed();
    void exportStatusChanged();
    void compareModelChanged();
    void diffModelChanged();
    void totalCellsChanged();
    void diffCountChanged();
    void file1ModelChanged();
    void file2ModelChanged();

private:
    QString m_filepath1;
    QString m_filepath2;
    QString m_exportStatus;
    QWidget* m_compareContainer;
    QTableWidget *mc_tableWidget;
    QStandardItemModel *m_file1Model;
    QStandardItemModel *m_file2Model;
    QStandardItemModel *m_compareModel;
    QStandardItemModel *m_diffModel;
    double m_totalCells;
    double m_diffCount;
};

#endif // CUASO_H
