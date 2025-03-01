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
// Hàm xóa sạch layout
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
        "  background-color: maroon;"
        "}";

    m_nutbam  = new QPushButton(tr("Hãy nạp file excel 1"), this);
    m_nutbam1 = new QPushButton(tr("Hãy nạp file excel 2"), this);
    m_nutbamss= new QPushButton(tr("Hãy nạp file ngôn ngữ"),this);
    sosanh    = new QPushButton(tr("Ấn để so sánh"), this);
    m_exit    = new QPushButton(tr("Ấn để thoát"), this);
    searchline = new QLineEdit(this);
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
        if (Translate::instance().loadTranslations(filepath2)) {
            qDebug() << "Translation data loaded from file 2.";
            {
                QSignalBlocker blocker(languageCombo);
                languageCombo->clear();
                QStringList langs = Translate::instance().availableLanguages();
                for (const QString &lang : langs) {
                    QString iconPath = QDir::currentPath() + "/1.png";  // Tạo đường dẫn tuyệt đối
                    qDebug() << "Icon path:" << iconPath;
                    QIcon icon(iconPath);
                    int index = languageCombo->count(); // Lấy chỉ số mục tiếp theo
                    languageCombo->addItem(lang, lang);
                    languageCombo->setItemIcon(index, QIcon("1.png")); // Đặt icon đúng mục
                }

                if (!currentLang.isEmpty() && langs.contains(currentLang)) {
                    int index = languageCombo->findData(currentLang);
                    if (index >= 0)
                        languageCombo->setCurrentIndex(index);
                }
            }
        } else {
            qDebug() << "Failed to load translation data from file 2.";
        }
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

    QTableWidget *compareWidget = new QTableWidget();
    compareWidget->setRowCount(maxRow);
    compareWidget->setColumnCount(maxCol);
    compareWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    compareWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QString currentLang = Translate::instance().currentLanguage();
    for (int i = 1; i <= maxRow; i++) {
        for (int j = 1; j <= maxCol; j++) {
            QVariant val1 = doc1.read(i, j);
            QVariant val2 = doc2.read(i, j);
            QTableWidgetItem *item = new QTableWidgetItem();
            if (val1 != val2) {
                item->setText(Translate::instance().translate("❌ Khác", currentLang));
                item->setBackground(Qt::darkCyan);
                item->setForeground(Qt::white);
            } else {
                item->setText(val1.toString());
            }
            compareWidget->setItem(i - 1, j - 1, item);
        }
    }

    QTableWidget *table1 = createTableFromDocument(doc1, maxRow, maxCol);
    QTableWidget *table2 = createTableFromDocument(doc2, maxRow, maxCol);

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
    m_nutbam->setText(Translate::instance().translate("Hãy nạp file excel 1", lang));
    m_nutbam1->setText(Translate::instance().translate("Hãy nạp file excel 2", lang));
    m_nutbamss->setText(Translate::instance().translate("Hãy nạp file ngôn ngữ",lang));
    sosanh->setText(Translate::instance().translate("Ấn để so sánh", lang));
    m_exit->setText(Translate::instance().translate("Ấn để thoát", lang));
    searchline->setPlaceholderText((Translate::instance().translate("Nhập từ khóa để tìm kiếm",lang)));
    if (m_tab->count() >= 3) {
        m_tab->setTabText(0, Translate::instance().translate("Trang 1", lang));
        m_tab->setTabText(1, Translate::instance().translate("Trang 2", lang));
        m_tab->setTabText(2, Translate::instance().translate("So sánh", lang));
    }
}
