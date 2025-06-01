#include "Window.h"
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <QTimer>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QStandardItemModel>
#include <QTableView>
#include <QWindow>
#include <QPushButton>
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include <QProcess>
#include <QSettings>

Window::Window(QObject *parent)
    : QObject(parent), m_exportStatus(""),
    m_file1Model(new QStandardItemModel(this)),
    m_file2Model(new QStandardItemModel(this)),
    m_compareModel(new QStandardItemModel(this)),
    m_diffModel(new QStandardItemModel(this)),
    m_totalCells(0),
    m_diffCount(0)
{
    loadLastSession();
}
void Window::openWeb(){
    QUrl url("https://huggingface.co/spaces/Naruto123321/languages");
    QDesktopServices::openUrl(url);
}
QString Window::filepath_old() const {
    return m_filepath_old;
}
void Window::setfilepath_old(const QString &filepath_old) {
    if (m_filepath_old != filepath_old) {
        m_filepath_old = filepath_old;
        emit filepath_oldChanged();
    }
}

QString Window::filepath_new() const {
    return m_filepath_new;
}

void Window::setfilepath_new(const QString &filepath_new) {
    if (m_filepath_new != filepath_new) {
        m_filepath_new = filepath_new;
        emit filepath_newChanged();
    }
}

QString Window::exportStatus() const {
    return m_exportStatus;
}

void Window::setExportStatus(const QString &exportStatus) {
    if (m_exportStatus != exportStatus) {
        m_exportStatus = exportStatus;
        emit exportStatusChanged();
    }
}
void Window::resetWindow(){
    m_filepath_old.clear();
    m_filepath_new.clear();

    if (m_file1Model) {
        m_file1Model->clear();
        emit file1ModelChanged();
    }
    if (m_file2Model) {
        m_file2Model->clear();
        emit file2ModelChanged();
    }
    if (m_compareModel) {
        m_compareModel->clear();
        emit compareModelChanged();
    }
    if (m_diffModel) {
        m_diffModel->clear();
        emit diffModelChanged();
    }

    m_totalCells = 0;
    m_diffCount = 0;
    emit filepath_oldChanged();
    emit filepath_newChanged();
    emit totalCellsChanged();
    emit diffCountChanged();
}

void Window::loadFile1() {
    QString file = QFileDialog::getOpenFileName(nullptr, "Select Excel File 1", "", "Excel Files (*.xlsx )");
    if (!file.isEmpty()) {
        setfilepath_old(file);
        qDebug() << "Loaded File1:" << file;
    }
}
QString columnNumberToLetter(int columnNumber) {
    QString columnName;
    while (columnNumber > 0) {
        int remainder = (columnNumber - 1) % 26;
        columnName.prepend(QChar('A' + remainder));
        columnNumber = (columnNumber - 1) / 26;
    }
    return columnName;
}

void Window::loadFile2() {
    QString file = QFileDialog::getOpenFileName(nullptr, "Select Excel File 2", "", "Excel Files (*.xlsx )");
    if (!file.isEmpty()) {
        setfilepath_new(file);
        qDebug() << "Loaded File2:" << file;
    }
}
QVector<QVector<QVariant>> cacheSheet(QXlsx::Document& doc, int maxRow, int maxCol) {
    QVector<QVector<QVariant>> data(maxRow, QVector<QVariant>(maxCol));
    for (int i = 0; i < maxRow; ++i)
        for (int j = 0; j < maxCol; ++j)
            data[i][j] = doc.read(i + 1, j + 1);
    return data;
}
QVector<QVector<QVariant>> compare(QVector<QVector<QVariant>>m_1,QVector<QVector<QVariant>> m_2,int maxRow, int maxCol) {
    QVector<QVector<QVariant>> data(maxRow, QVector<QVariant>(maxCol));
    for (int i=0;i<maxRow;i++){
        for(int j=0;j<maxCol;j++){
            if (m_1[i][j]!=m_2[i][j]){
                data[i][j]="x Diffetent";
            }
            else{
                data[i][j]=m_1[i][j].toString();
            }
        }
    }
    return data;
}
QVector<QVector<QVariant>> Window::diff_model(const QVector<QVector<QVariant>>& m_1,
                                      const QVector<QVector<QVariant>>& m_2,
                                      int maxRow, int maxCol) {
    QVector<QVector<QVariant>> result(maxRow, QVector<QVariant>(maxCol + 1)); // +1 để thêm cột "Difference"
    for (int i = 0; i < maxRow; ++i) {
        bool rowHasDiff = false;
        for (int j = 0; j < maxCol; ++j) {
            if (m_1[i][j] != m_2[i][j]) {
                rowHasDiff = true;
                if (m_1[i][j].isNull() && !m_2[i][j].isNull()) {
                    result[i][j] = QString("File_old:{EMPTY} | File_new:{%1}").arg(m_2[i][j].toString());
                } else if (!m_1[i][j].isNull() && m_2[i][j].isNull()) {
                    result[i][j] = QString("File_old:{%1} | File_new:{EMPTY}").arg(m_1[i][j].toString());
                } else {
                    result[i][j] = QString("File_old:{%1} | File_new:{%2}")
                    .arg(m_1[i][j].toString())
                        .arg(m_2[i][j].toString());
                }
            } else {
                result[i][j] = m_1[i][j].toString();
            }
        }
        result[i][maxCol] = rowHasDiff ? "Yes" : "No";
    }
    return result;
}

void Window::compareSheets() {
    qDebug() << "Compare called";
    QElapsedTimer timer;
    timer.start();

    if (m_filepath_old.isEmpty() || m_filepath_new.isEmpty()) {
        qDebug() << "Chưa chọn đủ file!";
        return;
    }

    QXlsx::Document doc1(m_filepath_old), doc2(m_filepath_new);
    if (!doc1.load() || !doc2.load()) {
        qDebug() << "Lỗi mở file Excel!";
        return;
    }

    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    int maxRow = std::max(range1.lastRow(), range2.lastRow());
    int maxCol = std::max(range1.lastColumn(), range2.lastColumn());

    m_totalCells = maxRow * maxCol;
    m_diffCount = 0;

    QFuture<QVector<QVector<QVariant>>> futureData1 = QtConcurrent::run(cacheSheet, std::ref(doc1), maxRow, maxCol);
    QFuture<QVector<QVector<QVariant>>> futureData2 = QtConcurrent::run(cacheSheet, std::ref(doc2), maxRow, maxCol);
    futureData1.waitForFinished();
    futureData2.waitForFinished();

    QVector<QVector<QVariant>> data1 = futureData1.result();
    QVector<QVector<QVariant>> data2 = futureData2.result();

    QFuture<QVector<QVector<QVariant>>> futureCompare = QtConcurrent::run(compare, data1, data2, maxRow, maxCol);
    auto futureDiff = QtConcurrent::run([=]() {
        return this->diff_model(data1, data2, maxRow, maxCol);
    });

    futureCompare.waitForFinished();
    futureDiff.waitForFinished();

    QVector<QVector<QVariant>> compareData = futureCompare.result();
    QVector<QVector<QVariant>> diffData = futureDiff.result();

    m_file1Model->clear();
    m_file2Model->clear();
    m_compareModel->clear();
    m_diffModel->clear();

    for (int j = 0; j <= maxCol; ++j) {
        QString colMark = (j == 0) ? "" : columnNumberToLetter(j);
        QStandardItem *header1 = new QStandardItem(colMark);
        QStandardItem *header2 = new QStandardItem(colMark);
        QStandardItem *headerCompare = new QStandardItem(colMark);
        QStandardItem *headerDiff = new QStandardItem(colMark);

        header1->setBackground(Qt::gray);
        header2->setBackground(Qt::gray);
        headerCompare->setBackground(Qt::gray);
        headerDiff->setBackground(Qt::gray);

        m_file1Model->setItem(0, j, header1);
        m_file2Model->setItem(0, j, header2);
        m_compareModel->setItem(0, j, headerCompare);
        m_diffModel->setItem(0, j, headerDiff);
    }
    m_diffModel->setItem(0, maxCol, new QStandardItem("Difference"));

    for (int i = 0; i < maxRow; ++i) {
        QStandardItem *rowHeader1 = new QStandardItem(QString::number(i + 1));
        QStandardItem *rowHeader2 = new QStandardItem(QString::number(i + 1));
        QStandardItem *rowHeaderCompare = new QStandardItem(QString::number(i + 1));
        QStandardItem *rowHeaderDiff = new QStandardItem(QString::number(i + 1));

        rowHeader1->setBackground(Qt::gray);
        rowHeader2->setBackground(Qt::gray);
        rowHeaderCompare->setBackground(Qt::darkYellow);
        rowHeaderDiff->setBackground(Qt::gray);

        m_file1Model->setItem(i + 1, 0, rowHeader1);
        m_file2Model->setItem(i + 1, 0, rowHeader2);
        m_compareModel->setItem(i + 1, 0, rowHeaderCompare);
        m_diffModel->setItem(i + 1, 0, rowHeaderDiff);

        for (int j = 0; j < maxCol; ++j) {
            m_file1Model->setItem(i + 1, j + 1, new QStandardItem(data1[i][j].toString()));
            m_file2Model->setItem(i + 1, j + 1, new QStandardItem(data2[i][j].toString()));

            QStandardItem *compareItem = new QStandardItem(compareData[i][j].toString());
            if (compareData[i][j].toString().contains("Diffetent", Qt::CaseInsensitive)) {
                compareItem->setBackground(QColor("#FFC7CE"));
            }
            m_compareModel->setItem(i + 1, j + 1, compareItem);

            QStandardItem *diffItem = new QStandardItem(diffData[i][j].toString());
            if (data1[i][j] != data2[i][j]) {
                ++m_diffCount;
                diffItem->setBackground(QBrush(QColor("#D9E1F2")));
            }
            m_diffModel->setItem(i + 1, j + 1, diffItem);
        }
        m_diffModel->setItem(i + 1, maxCol, new QStandardItem(diffData[i][maxCol].toString()));
    }

    m_elapsedTime = timer.elapsed();

    emit file1ModelChanged();
    emit file2ModelChanged();
    emit compareModelChanged();
    emit diffModelChanged();
    emit totalCellsChanged();
    emit diffCountChanged();
    emit elapsedTimeChanged();

    double similarityPercent = 100.0 * (m_totalCells - m_diffCount) / m_totalCells;
    qDebug() << "Total cells:" << m_totalCells << "Different:" << m_diffCount << "Similarity:" << similarityPercent;
}

void Window::exportDifferences() {
    QString filePath = QFileDialog::getSaveFileName(nullptr,
                                                    tr("Save File As"),
                                                    QDir::homePath() + "/Different_file.xlsx",
                                                    tr("Excel Files (*.xlsx)"));
    if (filePath.isEmpty())
        return;

    QXlsx::Format format_string;
    format_string.setFontBold(true);
    format_string.setFontColor(QColor(Qt::gray));
    format_string.setPatternBackgroundColor(QColor(Qt::green));
    QXlsx::Document exportDoc;

    int rowCount = m_diffModel->rowCount();
    int colCount = m_diffModel->columnCount();

    for (int r = 1; r < rowCount; ++r) {
        for (int c = 1; c < colCount; ++c) {
            QStandardItem *item = m_diffModel->item(r, c);
            if (item) {
                QString text = item->text();
                if (text.contains("File_old")) {
                    exportDoc.write(r + 1, c + 1, text, format_string);
                } else {
                    exportDoc.write(r + 1, c + 1, text);
                }
            }
        }
    }
    exportDoc.autosizeColumnWidth(1, colCount);
    if (!filePath.endsWith(".xlsx"))
        filePath += ".xlsx";
    exportDoc.saveAs(filePath);
}


void Window::exportQm(const QString &directory) {
    qDebug()<<"Qm called";
    if (m_filepath_new.isEmpty()) {
        m_exportStatus = "Please check file Excel new";
        emit exportStatusChanged();
        return;
    }

    QXlsx::Document xlsx(m_filepath_new);
    int colCount = xlsx.dimension().columnCount();
    int rowCount = xlsx.dimension().rowCount();
    QDir dir(directory);

    if (!dir.exists() && !dir.mkpath(".")) {
        m_exportStatus = "Output directory does not exist and cannot be created";
        emit exportStatusChanged();
        return;
    }

    if (colCount < 2 || rowCount < 2) {
        m_exportStatus = "Excel file format is incorrect";
        emit exportStatusChanged();
        return;
    }

    QString contextName = "main";
    for (int col = 2; col <= colCount; ++col) {
        QString language = xlsx.read(1, col).toString();
        if (language.isEmpty()) continue;

        QString tsFile = directory + QString("/translations_%1.ts").arg(language);
        QFile file(tsFile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            m_exportStatus = "Cannot create TS file";
            emit exportStatusChanged();
            return;
        }

        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << "<?xml version='1.0' encoding='utf-8'?>\n";
        out << QString("<TS version='2.1' language='%1'>\n<context>\n<name>%2</name>\n")
                   .arg(language).arg(contextName);

        for (int row = 2; row <= rowCount; ++row) {
            QString key = xlsx.read(row, 1).toString().trimmed();
            if (key.startsWith("STR_")) {
                key = key.mid(4).replace("_", " ");
            }
            QString translation = xlsx.read(row, col).toString();
            out << "<message>\n";
            out << "<source>" << key << "</source>\n";
            out << "<translation>" << translation << "</translation>\n";
            out << "</message>\n";
        }

        out << "</context>\n</TS>";
        file.close();

        QString qmFile = directory + QString("/translations_%1.qm").arg(language);
        QProcess process;
        process.start("lrelease", QStringList() << tsFile << "-qm" << qmFile);
        process.waitForFinished();

        if (process.exitStatus() != QProcess::NormalExit) {
            m_exportStatus = "Failed to generate QM file";
            emit exportStatusChanged();
            return;
        }
    }
    qDebug() << "Export QM into:" << directory;
    setExportStatus("Export successfully");
}
void Window::loadLastSession() {
    QSettings settings("MyCompany", "MyApp");
    m_filepath_old = settings.value("filepath_old").toString();
    m_filepath_new = settings.value("filepath_new").toString();
}
void Window::saveLastSession() {
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("filepath_old", m_filepath_old);
    settings.setValue("filepath_new", m_filepath_new);

}
Window::~Window() {
    saveLastSession();
}
