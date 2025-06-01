#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QTableWidget>
#include "xlsxdocument.h"
#include <QStandardItemModel>
#include <QElapsedTimer>
#include <QDesktopServices>
#include <QFutureWatcher>
#include <QtConcurrent>
class Window : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filepath_old READ filepath_old WRITE setfilepath_old NOTIFY filepath_oldChanged)
    Q_PROPERTY(QString filepath_new READ filepath_new WRITE setfilepath_new NOTIFY filepath_newChanged)
    Q_PROPERTY(QString exportStatus READ exportStatus WRITE setExportStatus NOTIFY exportStatusChanged)
    Q_PROPERTY(QAbstractItemModel* file1Model READ file1Model NOTIFY file1ModelChanged)
    Q_PROPERTY(QAbstractItemModel* file2Model READ file2Model NOTIFY file2ModelChanged)
    Q_PROPERTY(QAbstractItemModel* compareModel READ compareModel NOTIFY compareModelChanged)
    Q_PROPERTY(QAbstractItemModel* diffModel READ diffModel NOTIFY diffModelChanged)
    Q_PROPERTY(double totalCells READ totalCells NOTIFY totalCellsChanged)
    Q_PROPERTY(double diffCount READ diffCount NOTIFY diffCountChanged)
    Q_PROPERTY(qint64 elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)

public:
    explicit Window(QObject *parent = nullptr);
    ~Window();
    QString filepath_old() const;
    void setfilepath_old(const QString &filepath_old);
    Q_INVOKABLE void openWeb();
    QString filepath_new() const;
    void setfilepath_new(const QString &filepath_new);

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
    qint64 elapsedTime() const{
        return m_elapsedTime;
    }
    QFutureWatcher<void> *m_compareWatcher = nullptr;
    QAbstractItemModel* diffModel() const { return m_diffModel; }
    double totalCells() const { return m_totalCells; }
    double diffCount() const { return m_diffCount; }
    void saveLastSession();
    void loadLastSession();
    Q_INVOKABLE void resetWindow();

    QVector<QVector<QVariant> > diff_model(const QVector<QVector<QVariant> > &m_1, const QVector<QVector<QVariant> > &m_2, int maxRow, int maxCol);
signals:
    void filepath_oldChanged();
    void filepath_newChanged();
    void exportStatusChanged();
    void compareModelChanged();
    void diffModelChanged();
    void totalCellsChanged();
    void diffCountChanged();
    void file1ModelChanged();
    void file2ModelChanged();
    void elapsedTimeChanged();

private:
    QString m_filepath_old;
    QString m_filepath_new;
    QString m_exportStatus;
    QWidget* m_compareContainer;
    QTableWidget *mc_tableWidget;
    QStandardItemModel *m_file1Model;
    QStandardItemModel *m_file2Model;
    QStandardItemModel *m_compareModel;
    QStandardItemModel *m_diffModel;
    double m_totalCells;
    double m_diffCount;
    qint64 m_elapsedTime;
};

#endif // WINDOW_H
