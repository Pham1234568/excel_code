#include "Cuaso.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "header/xlsxdocument.h"
#include <QHeaderView>
#include <QSpacerItem>
#include <algorithm>
#include <QApplication>
#include <QSqlQuery>
#include "Translation.h"
#include <QSignalBlocker>
#include<QScrollBar>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QSettings>
#include <QTimer>
#include<QStandardItem>
#include<QStandardItemModel>
#include<QListView>
void Cuaso::clearLayout(QLayout *layout) {
    if (!layout)
        return;
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget())
            delete item->widget();
        delete item;
    }
}

Cuaso::Cuaso(QWidget *parent) : QWidget(parent) {
    setMinimumSize(800, 600);

    first = new QWidget();
    second = new QWidget();
    third = new QWidget();
    QString buttonStyle =
        "QPushButton {"
        "  border: 2px solid #8f8f91;"
        "  border-radius: 12px;"
        "  background-color: cyan;"
        "  min-width: 80px;"
        "}"
        "QPushButton:pressed {"
        "  background-color: white;"
        "}";

    m_nutbam  = new QPushButton(tr("Hãy nạp file excel 1"), this);
    m_nutbam1 = new QPushButton(tr("Hãy nạp file excel 2"), this);
    m_nutbamss= new QPushButton(tr("Hãy nạp file ngôn ngữ"),this);
    sosanh    = new QPushButton(tr("Ấn để so sánh"), this);
    m_exit    = new QPushButton(tr("Ấn để thoát"), this);
    searchline = new QLineEdit(this);
    load        =new QProgressBar(this);
    load->setTextVisible(true);
    load->setMaximum(100);
    load->setMinimum(0);
    load->setValue(0);
    load->setVisible(false);
    QString currentlang=Translate::instance().currentLanguage();
    searchline->setPlaceholderText((Translate::instance().translate("Nhập từ khóa để tìm kiếm",currentlang)));
    m_nutbam->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_nutbam1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_nutbamss->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sosanh->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_exit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_nutbam->setStyleSheet(buttonStyle);
    m_nutbam1->setStyleSheet(buttonStyle);
    m_nutbamss->setStyleSheet(buttonStyle);
    sosanh->setStyleSheet(buttonStyle);
    m_exit->setStyleSheet(buttonStyle);

    m_tableWidget = new QTableWidget();
    m_tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_tab = new QTabWidget(this);
    languageCombo = new QComboBox(this);
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int){
        QString lang = languageCombo->currentData().toString();
        Translate::instance().setCurrentLanguage(lang);
        updateUiTranslations();
    });
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(languageCombo);
    buttonLayout->addWidget(m_nutbam);
    buttonLayout->addWidget(m_nutbam1);
    buttonLayout->addWidget(m_nutbamss);
    buttonLayout->addWidget(sosanh);
    buttonLayout->addWidget(m_exit);
    buttonLayout->addStretch();

    QVBoxLayout *beside =new QVBoxLayout;
    beside->addWidget(searchline);
    beside->addWidget(m_tab);
    beside->addWidget(load);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(beside, 1);


    setLayout(mainLayout);

    connect(m_nutbam,  &QPushButton::clicked, this, &Cuaso::loadFile);
    connect(m_nutbam1, &QPushButton::clicked, this, &Cuaso::loadFile3);
    connect(m_nutbamss,&QPushButton::clicked,this,  &Cuaso::loadFile2);
    connect(sosanh,    &QPushButton::clicked, this, &Cuaso::compare);
    connect(m_exit,    &QPushButton::clicked, this, &Cuaso::close);
    connect(searchline,&QLineEdit::textChanged,this,&Cuaso::filterTable);
    setAcceptDrops(true);


    loadLastSession();

}
void Cuaso::filterTable(const QString &text) {
    for (int row = 0; row < m_tableWidget->rowCount(); ++row) {
        bool rowVisible = false;
        for (int col = 0; col < m_tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = m_tableWidget->item(row, col);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                rowVisible = true;
                break;
            }
        }
        m_tableWidget->setRowHidden(row, !rowVisible);
    }
}

void Cuaso::dragEnterEvent(QDragEnterEvent *event) {

    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();
        if (!urlList.isEmpty()) {
            QString fileName = urlList.first().toLocalFile();
            if (fileName.endsWith(".xlsx", Qt::CaseInsensitive)) {
                event->acceptProposedAction();
            }
        }
    }
}

void Cuaso::dropEvent(QDropEvent *event) {
    QList<QUrl> urlList = event->mimeData()->urls();
    if (!urlList.isEmpty()) {
        QString filePath = urlList.first().toLocalFile();

        filepath1 = filePath;
        m_nutbam->setText(Translate::instance().translate("Đã nạp file 1", Translate::instance().currentLanguage()));
        QMessageBox::information(this, tr("NẠP FILE"), tr("Bạn đã nạp thành công file 1 thông qua Drag & Drop"));
        qDebug() << "File 1 nạp qua Drag & Drop:" << filepath1;
    }
}
void Cuaso::loadFile() {
    QString currentLang = Translate::instance().currentLanguage();
    QString fileTitle = Translate::instance().translate("Chọn file excel", currentLang);
    QString fileFilter = Translate::instance().translate("Excel Files (*.xlsx)", currentLang);
    QString newFilePath = QFileDialog::getOpenFileName(this, fileTitle, "", fileFilter);
    if (newFilePath.isEmpty()) {
        qDebug() << Translate::instance().translate("Hãy chọn lại file excel 1!", currentLang);
    } else {
        filepath1 = newFilePath;
        m_nutbam->setText(Translate::instance().translate("Đã nạp file 1", currentLang));
        QMessageBox::information(this,
                                 Translate::instance().translate("NẠP FILE", currentLang),
                                 Translate::instance().translate("Bạn đã nạp thành công file 1", currentLang));
        qDebug() << Translate::instance().translate("File 1 được nạp:", currentLang) << filepath1;
    }
}
void Cuaso::loadFile3(){
    QString currentLang = Translate::instance().currentLanguage();
    QString fileTitle = Translate::instance().translate("Chọn file excel", currentLang);
    QString fileFilter = Translate::instance().translate("Excel Files (*.xlsx)", currentLang);
    QString newFilePath = QFileDialog::getOpenFileName(this, fileTitle, "", fileFilter);
    if (newFilePath.isEmpty()) {
        qDebug() << Translate::instance().translate("Hãy chọn lại file excel 2!", currentLang);
    } else {
        filepath3 = newFilePath;
        m_nutbam1->setText(Translate::instance().translate("Đã nạp file 2", currentLang));
        QMessageBox::information(this,
                                    Translate::instance().translate("NẠP FILE", currentLang),
                                 Translate::instance().translate("Bạn đã nạp thành công file 2", currentLang));
        qDebug() << Translate::instance().translate("File 1 được nạp:", currentLang) << filepath3;
    }
}
void Cuaso::loadFile2() {
    QString currentLang = Translate::instance().currentLanguage();
    if (!Translate::instance().currentFilePath().isEmpty()) {
        qDebug() << Translate::instance().translate("File excel 2 đã được nạp, không cần cập nhật lại.", currentLang);
        return;
    }
    QString fileTitle = Translate::instance().translate("Chọn file ngôn ngữ", currentLang);
    QString fileFilter = Translate::instance().translate("Excel Files (*.xlsx)", currentLang);
    QString newFilePath = QFileDialog::getOpenFileName(this, fileTitle, "", fileFilter);
    if (newFilePath.isEmpty()) {
        qDebug() << Translate::instance().translate("Hãy chọn lại file excel 2!", currentLang);
    } else {
        filepath2 = newFilePath;
        m_nutbamss->setText(Translate::instance().translate("Đã nạp xong file ngôn ngữ", currentLang));
        QMessageBox::information(this,
                                 Translate::instance().translate("NẠP FILE", currentLang),
                                 Translate::instance().translate("Bạn đã nạp thành công file ngôn ngữ", currentLang));
        qDebug() << Translate::instance().translate("File 2 được nạp:", currentLang) << filepath2;
        Cuaso::loadFromTranslate(filepath2);
    }
}




void Cuaso::compare() {
    if (filepath1.isEmpty() || filepath3.isEmpty()) {
        QMessageBox::critical(this, tr("Nhập lại"), tr("Chưa có đủ file"));
        qDebug() << tr("Chưa nạp đủ file để so sánh!");
        return;
    }

    QXlsx::Document doc1(filepath1);
    QXlsx::Document doc2(filepath3);
    if (!doc1.load() || !doc2.load()) {
        QMessageBox::information(this, tr("Nhập lại"), tr("Không thể mở file excel"));
        qDebug() << tr("Lỗi khi mở file!");
        return;
    }

    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    if (range1.lastRow() == 0 || range1.lastColumn() == 0 ||
        range2.lastRow() == 0 || range2.lastColumn() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("File excel không hợp lệ cho việc so sánh"));
        return;
    }

    compareSheets(doc1, doc2);
}


void Cuaso::compareSheets(const QXlsx::Document &doc1, const QXlsx::Document &doc2) {
    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    int maxRow = std::max(range1.lastRow(), range2.lastRow());
    int maxCol = std::max(range1.lastColumn(), range2.lastColumn());
    int diffCount = 0;
    int totalCells = maxRow * maxCol;

    QTableWidget *compareWidget = new QTableWidget();
    compareWidget->setRowCount(maxRow);
    compareWidget->setColumnCount(maxCol);
    compareWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    compareWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QString currentLang = Translate::instance().currentLanguage();
    load->setVisible(true);
    load->setValue(0);
    load->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid grey;"
        "    border-radius: 5px;"
        "    background-color: #F0F0F0;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: green;"  // Màu xanh dương
        "    width: 10px;"
        "}"
        );
    QVector<QVector<QVariant>> data1(maxRow, QVector<QVariant>(maxCol));
    QVector<QVector<QVariant>> data2(maxRow, QVector<QVariant>(maxCol));
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            data1[i][j] = doc1.read(i + 1, j + 1);
            data2[i][j] = doc2.read(i + 1, j + 1);
        }
    }
    QVector<QTableWidgetItem*> items;
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            if (data1[i][j] != data2[i][j]) {
                diffCount++;
                item->setText(Translate::instance().translate("❌ Khác", currentLang));
                item->setBackground(Qt::darkCyan);
                item->setForeground(Qt::white);
            } else {
                item->setText(data1[i][j].toString());
            }
            items.append(item);
        }

        for (int j = 0; j < maxCol; j++) {
            compareWidget->setItem(i, j, items[i * maxCol + j]);
        }
        int progress = ((i + 1) * 100) / maxRow;
        load->setValue(progress);
        if (i % 10 == 0) qApp->processEvents();
    }
    load->setValue(100);
    QTimer::singleShot(500, [this]() { load->setVisible(false); });
    QTableWidget *table1 = createTableFromDocument(doc1, maxRow, maxCol);
    QTableWidget *table2 = createTableFromDocument(doc2, maxRow, maxCol);


    double similarityPercent = 100.0 * (totalCells - diffCount) / totalCells;
    QString report = tr("Tổng số ô: %1\nSố ô khác nhau: %2\nTỷ lệ tương đồng: %3%")
                         .arg(totalCells)
                         .arg(diffCount)
                         .arg(similarityPercent, 0, 'f', 2);
    QStandardItemModel *report_tab = new QStandardItemModel();
    report_tab->setColumnCount(2);
    report_tab->setHorizontalHeaderLabels(QStringList() << "Thành phần" << "Giá trị");

    report_tab->appendRow(QList<QStandardItem*>{
        new QStandardItem("Tổng số ô"), new QStandardItem(QString::number(totalCells))
    });
    report_tab->appendRow(QList<QStandardItem*>{
        new QStandardItem("Số ô khác nhau"), new QStandardItem(QString::number(diffCount))
    });
    report_tab->appendRow(QList<QStandardItem*>{
        new QStandardItem("Tỷ lệ tương đồng"), new QStandardItem(QString("%1%").arg(similarityPercent, 0, 'f', 2))
    });

    QTableView *table_Report = new QTableView();
    table_Report->setModel(report_tab);
    table_Report->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_Report->verticalHeader()->setVisible(false);
    table_Report->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_Report->setSelectionMode(QAbstractItemView::NoSelection);

    QMessageBox::information(this, tr("Báo cáo so sánh"), report);

    if (!first->layout())
        first->setLayout(new QVBoxLayout());
    else
        clearLayout(first->layout());
    first->layout()->addWidget(table1);


    if (!second->layout())
        second->setLayout(new QVBoxLayout());
    else
        clearLayout(second->layout());
    second->layout()->addWidget(table2);

    if (!third->layout())
        third->setLayout(new QVBoxLayout());
    else
        clearLayout(third->layout());
    third->layout()->addWidget(compareWidget);

    m_tab->clear();
    m_tab->addTab(first,Translate::instance().translate("Trang 1", currentLang));
    m_tab->addTab(second,Translate::instance().translate("Trang 2", currentLang));
    m_tab->addTab(third,Translate::instance().translate("So sánh", currentLang));
    m_tab->addTab(table_Report,"Báo cáo");
    // adjustSize();

    m_tableWidget = compareWidget;
}

QTableWidget* Cuaso::createTableFromDocument(const QXlsx::Document &doc, int maxRow, int maxCol) {
    QTableWidget *table = new QTableWidget();
    table->setRowCount(maxRow);
    table->setColumnCount(maxCol);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    for (int i = 1; i <= maxRow; i++) {
        for (int j = 1; j <= maxCol; j++) {
            QVariant val = doc.read(i, j);
            QTableWidgetItem *item = new QTableWidgetItem(val.toString());
            table->setItem(i - 1, j - 1, item);
        }
    }
    return table;
}

void Cuaso::updateUiTranslations() {
    QString lang = Translate::instance().currentLanguage();

    if (filepath1.isEmpty())
        m_nutbam->setText(Translate::instance().translate("Hãy nạp file excel 1", lang));
    else
        m_nutbam->setText(Translate::instance().translate("Đã nạp file 1", lang));
    if (filepath3.isEmpty())
        m_nutbam1->setText(Translate::instance().translate("Hãy nạp file excel 2", lang));
    else
        m_nutbam1->setText(Translate::instance().translate("Đã nạp file 2", lang));
    if (filepath2.isEmpty())
        m_nutbamss->setText(Translate::instance().translate("Hãy nạp file ngôn ngữ", lang));
    else
        m_nutbamss->setText(Translate::instance().translate("Đã nạp file ngôn ngữ", lang));

    sosanh->setText(Translate::instance().translate("Ấn để so sánh", lang));
    m_exit->setText(Translate::instance().translate("Ấn để thoát", lang));
    searchline->setPlaceholderText(Translate::instance().translate("Nhập từ khóa để tìm kiếm", lang));

    if (m_tab->count() >= 3) {
        m_tab->setTabText(0, Translate::instance().translate("Trang 1", lang));
        m_tab->setTabText(1, Translate::instance().translate("Trang 2", lang));
        m_tab->setTabText(2, Translate::instance().translate("So sánh", lang));
    }
}

void Cuaso::saveLastSession() {
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("filepath1", filepath1);
    settings.setValue("filepath2", filepath2);
    settings.setValue("filepath3", filepath3);
    settings.setValue("lastLanguage", Translate::instance().currentLanguage());
}
void Cuaso::loadLastSession() {
    QSettings settings("MyCompany", "MyApp");
    filepath1 = settings.value("filepath1").toString();
    filepath2 = settings.value("filepath2").toString();
    filepath3 = settings.value("filepath3").toString();
    QString savedLang = settings.value("lastLanguage").toString();

    if (!filepath1.isEmpty()) {
        m_nutbam->setText(Translate::instance().translate("Đã nạp file 1", savedLang));
        qDebug() << "Loaded file 1:" << filepath1;
    }
    if (!filepath2.isEmpty()) {
         qDebug() << "Loaded file ngon ngu:" << filepath2;
        m_nutbamss->setText(Translate::instance().translate("Đã nạp file ngôn ngữ", savedLang));
        Cuaso::loadFromTranslate(filepath2);
        if (!savedLang.isEmpty() && Translate::instance().availableLanguages().contains(savedLang)) {
            Translate::instance().setCurrentLanguage(savedLang);
            int index = languageCombo->findData(savedLang);
            if (index >= 0) {
                languageCombo->setCurrentIndex(index);
            }
        }
        updateUiTranslations();
    }

    if (!filepath3.isEmpty()) {
        m_nutbam1->setText(Translate::instance().translate("Đã nạp file 2", savedLang));
        qDebug() << "Loaded file 2:" << filepath3;
    }
}

void Cuaso::loadFromTranslate(QString filepath) {
    Translate::instance().loadTranslations(filepath);
    QString currentLang = Translate::instance().currentLanguage();
    QSignalBlocker blocker(languageCombo);
    languageCombo->clear();
    QStringList langs = Translate::instance().availableLanguages();
    for (const QString &lang : langs) {
        int index = languageCombo->count();
        languageCombo->addItem(lang, lang);
        languageCombo->setItemIcon(index, QIcon("1.png"));
    }
    if (!currentLang.isEmpty() && langs.contains(currentLang)) {
        int index = languageCombo->findData(currentLang);
        if (index >= 0)
            languageCombo->setCurrentIndex(index);
    }
}

Cuaso::~Cuaso() {
    saveLastSession();
}
