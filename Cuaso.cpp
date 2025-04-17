#include "Cuaso.h"
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
#include <QtConcurrent>
Cuaso::Cuaso(QObject *parent)
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

QString Cuaso::filepath1() const {
    return m_filepath1;
}

void Cuaso::setFilepath1(const QString &filepath1) {
    if (m_filepath1 != filepath1) {
        m_filepath1 = filepath1;
        emit filepath1Changed();
    }
}

QString Cuaso::filepath2() const {
    return m_filepath2;
}

void Cuaso::setFilepath2(const QString &filepath2) {
    if (m_filepath2 != filepath2) {
        m_filepath2 = filepath2;
        emit filepath2Changed();
    }
}

QString Cuaso::exportStatus() const {
    return m_exportStatus;
}

void Cuaso::setExportStatus(const QString &exportStatus) {
    if (m_exportStatus != exportStatus) {
        m_exportStatus = exportStatus;
        emit exportStatusChanged();
    }
}
void Cuaso::resetWindow(){
    m_filepath1.clear();
    m_filepath2.clear();
    emit filepath1Changed();
    emit filepath2Changed();
}

void Cuaso::loadFile1() {
    QString file = QFileDialog::getOpenFileName(nullptr, "Select Excel File 1", "", "Excel Files (*.xlsx )");
    if (!file.isEmpty()) {
        setFilepath1(file);
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

void Cuaso::loadFile2() {
    QString file = QFileDialog::getOpenFileName(nullptr, "Select Excel File 2", "", "Excel Files (*.xlsx )");
    if (!file.isEmpty()) {
        setFilepath2(file);
        qDebug() << "Loaded File2:" << file;
    }
}
void Cuaso::compareSheets() {
    if(m_filepath1.isEmpty() || m_filepath2.isEmpty()){
        qDebug() << "Chưa chọn đủ file!";
        return;
    }

    QXlsx::Document doc1(m_filepath1), doc2(m_filepath2);
    if(!doc1.load() || !doc2.load()){
        qDebug() << "Lỗi mở file Excel!";
        return;
    }

    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    int maxRow = std::max(range1.lastRow(), range2.lastRow());
    int maxCol = std::max(range1.lastColumn(), range2.lastColumn());
    m_totalCells = maxRow * maxCol;
    m_diffCount = 0;

    delete m_file1Model;
    m_file1Model = new QStandardItemModel(maxRow + 1, maxCol + 1, this);
    delete m_file2Model;
    m_file2Model = new QStandardItemModel(maxRow + 1, maxCol + 1, this);
    delete m_compareModel;
    m_compareModel = new QStandardItemModel(maxRow +1, maxCol +1, this);
    delete m_diffModel;
    m_diffModel = new QStandardItemModel(maxRow, maxCol + 1, this);

    QVector<QVector<QVariant>> data1(maxRow , QVector<QVariant>(maxCol));
    QVector<QVector<QVariant>> data2(maxRow , QVector<QVariant>(maxCol));

    m_file1Model->setItem(0, 0, new QStandardItem(""));
    m_file2Model->setItem(0, 0, new QStandardItem(""));
    m_compareModel->setItem(0,0,new QStandardItem(""));

    for (int i = 0; i <= maxRow; i++) {
        for (int j = 0; j <= maxCol; j++) {
            if (j == 0 && i > 0) {
                QString rowMark = QString::number(i);
                QStandardItem *itemRow1 = new QStandardItem(rowMark);
                QStandardItem *itemRow2 = new QStandardItem(rowMark);
                itemRow1->setBackground(QBrush(Qt::gray));
                itemRow2->setBackground(QBrush(Qt::gray));
                m_file1Model->setItem(i, 0, itemRow1);
                m_file2Model->setItem(i, 0, itemRow2);
            }
            if (i == 0 && j > 0) {
                QString colMark = columnNumberToLetter(j);
                QStandardItem *itemCol1 = new QStandardItem(colMark);
                QStandardItem *itemCol2 = new QStandardItem(colMark);
                itemCol1->setBackground(QBrush(Qt::gray));
                itemCol2->setBackground(QBrush(Qt::gray));
                m_file1Model->setItem(0, j, itemCol1);
                m_file2Model->setItem(0, j, itemCol2);
            }
            if (i > 0 && j > 0) {
                data1[i-1][j-1] = doc1.read(i, j);
                data2[i-1][j-1] = doc2.read(i, j);
                m_file1Model->setItem(i, j, new QStandardItem(data1[i-1][j-1].toString()));
                m_file2Model->setItem(i, j, new QStandardItem(data2[i-1][j-1].toString()));
            }
        }
    }
//SET COMPARE MODEL
    for (int i = 0; i <= maxRow; i++) {
        for (int j = 0; j <= maxCol; j++){
            QStandardItem *itemCompare = new QStandardItem();
            if (j == 0 && i > 0) {
                QString rowMark = QString::number(i);
                QStandardItem *itemRow1 = new QStandardItem(rowMark);
                itemRow1->setBackground(QBrush(Qt::darkYellow));
                m_compareModel->setItem(i, 0, itemRow1);

            }
            if (i == 0 && j > 0) {
                QString colMark = columnNumberToLetter(j);
                QStandardItem *itemCol1 = new QStandardItem(colMark);
                itemCol1->setBackground(QBrush(Qt::gray));
                m_compareModel->setItem(0, j, itemCol1);

            }
            if (i > 0 && j > 0) {
                if (data1[i-1][j-1] != data2[i-1][j-1]){
                    itemCompare->setText(QStringLiteral("❌ Different"));
                    m_compareModel->setItem(i, j, itemCompare);
                }
                else{
                    m_compareModel->setItem(i, j, new QStandardItem(data1[i-1][j-1].toString()));
                }
            }
        }
    }
 //SET DIFF MODEL
    for (int i = 0; i < maxRow; i++) {
        bool rowHasDiff = false;
        for (int j = 0; j < maxCol; j++) {
            QStandardItem *itemDiff    = new QStandardItem();
            if (data1[i][j] != data2[i][j]) {
                m_diffCount++;
                QString diffText;
                if (data1[i][j].isNull() && !data2[i][j].isNull())
                    diffText = QString("File_old:{EMPTY} | File_new:{%1}").arg(data2[i][j].toString());
                else if (!data1[i][j].isNull() && data2[i][j].isNull())
                    diffText = QString("File_old:{%1} | File_new:{EMPTY}").arg(data1[i][j].toString());
                else
                    diffText = QString("File_old:{%1} | File_new:{%2}")
                                   .arg(data1[i][j].toString())
                                   .arg(data2[i][j].toString());
                itemDiff->setText(diffText);
                itemDiff->setBackground(QBrush(QColor("#D9E1F2")));
                rowHasDiff = true;
            } else {
                itemDiff->setText(data1[i][j].toString());
            }
            m_diffModel->setItem(i, j, itemDiff);
        }
        QStandardItem *indicator = new QStandardItem(i == 0 ? "Difference" : (rowHasDiff ? "Yes" : "No"));
        m_diffModel->setItem(i, maxCol, indicator);
    }

    emit file1ModelChanged();
    emit file2ModelChanged();
    emit compareModelChanged();
    emit diffModelChanged();
    emit totalCellsChanged();
    emit diffCountChanged();

    double similarityPercent = 100.0 * (m_totalCells - m_diffCount) / m_totalCells;
    qDebug() << "Total cells:" << m_totalCells << "Different:" << m_diffCount << "Similarity:" << similarityPercent;
}

void Cuaso::exportDifferences() {
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

    for (int r = 0; r < rowCount; ++r) {
        for (int c = 0; c < colCount; ++c) {
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


void Cuaso::exportQm(const QString &directory) {
    if (m_filepath2.isEmpty()) {
        m_exportStatus = "Please check file Excel new";
        emit exportStatusChanged();
        return;
    }

    QXlsx::Document xlsx(m_filepath2);
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

    QString contextName = "Default";
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
    qDebug() << "Export QM vào thư mục:" << directory;
    setExportStatus("Export thành công!");
}
void Cuaso::loadLastSession() {
    QSettings settings("MyCompany", "MyApp");
    m_filepath1 = settings.value("filepath1").toString();
    m_filepath2 = settings.value("filepath2").toString();
}
void Cuaso::saveLastSession() {
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("filepath1", m_filepath1);
    settings.setValue("filepath2", m_filepath2);

}
Cuaso::~Cuaso() {
    saveLastSession();
}
