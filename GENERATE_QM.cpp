#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "xlsxdocument.h"
#include <QStringConverter>

// Hàm sinh file QM từ file Excel theo định dạng mới:
// Excel format mới:
//   String       English    China    VietNam    Japan
//   STR_YES      YES        HAO      DUNG       HAI
//   STR_HELLO    1HELLO     NIHAO    XIN CHAO   XXXX
//   STR_LOGIN             (rỗng)   DUNG       (rỗng)
void generateQM(const QString &excelPath, const QString &outputDir) {
    QXlsx::Document xlsx(excelPath);
    int colCount = xlsx.dimension().columnCount();
    int rowCount = xlsx.dimension().rowCount();

    if (colCount < 2 || rowCount < 2) {
        QMessageBox::warning(nullptr, "Error", "Excel file format is incorrect");
        return;
    }

    // Sử dụng một context chung. (Bắt buộc phải có context, bạn có thể đặt tên theo ý muốn)
    QString contextName = "Default";

    // Duyệt từng cột (bắt đầu từ cột 2 là các ngôn ngữ)
    for (int col = 2; col <= colCount; ++col) {
        QString language = xlsx.read(1, col).toString(); // dòng 1 chứa header: English, China, VietNam, Japan,...
        if (language.isEmpty()) continue;

        // Tạo file TS cho từng ngôn ngữ
        QString tsFile = outputDir + QString("/translations_%1.ts").arg(language);
        QFile file(tsFile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(nullptr, "Error", "Cannot create TS file");
            return;
        }

        QTextStream out(&file);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 2, 0))
        out.setEncoding(QStringConverter::Utf8);
#else
        out.setCodec("UTF-8");
#endif

        out << "<?xml version='1.0' encoding='utf-8'?>\n";
        out << QString("<TS version='2.1' language='%1'>\n<context>\n<name>%2</name>\n")
                   .arg(language).arg(contextName);

        // Duyệt từng dòng (bắt đầu từ dòng 2 chứa các cặp key – translation)
        for (int row = 2; row <= rowCount; ++row) {
            QString key = xlsx.read(row, 1).toString().trimmed(); // cột 1 chứa String (vd: STR_YES)
            if (key.startsWith("STR_")) {
                key = key.mid(4).replace("_"," "); // loại bỏ "STR_" để lấy ra key thật, ví dụ "STR_YES" -> "YES"
            }
            QString translation = xlsx.read(row, col).toString();
            out << "<message>\n";
            out << "<source>" << key << "</source>\n";
            out << "<translation>" << translation << "</translation>\n";
            out << "</message>\n";
        }

        out << "</context>\n</TS>";
        file.close();

        // Sinh file QM bằng lệnh lrelease
        QString qmFile = outputDir + QString("/translations_%1.qm").arg(language);
        QProcess process;
        process.start("lrelease", QStringList() << tsFile << "-qm" << qmFile);
        process.waitForFinished();

        if (process.exitStatus() != QProcess::NormalExit) {
            QMessageBox::warning(nullptr, "Error", "Failed to generate QM file");
            return;
        }
        qDebug() << "Generated QM file:" << qmFile;
    }

    QMessageBox::information(nullptr, "Success", "QM files generated successfully in the default directory!");
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QPushButton *btnSelectExcel = new QPushButton("Select Excel File");
    QPushButton *btnGenerate = new QPushButton("Generate QM");

    layout->addWidget(btnSelectExcel);
    layout->addWidget(btnGenerate);

    QString excelFilePath;
    QString outputDirPath = "D:/Code_Qt/Code_Qt";

    QObject::connect(btnSelectExcel, &QPushButton::clicked, [&]() {
        excelFilePath = QFileDialog::getOpenFileName(nullptr, "Open Excel File", "", "Excel Files (*.xlsx)");
        if (!excelFilePath.isEmpty()) {
            qDebug() << "Excel file selected:" << excelFilePath;
        }
    });

    QObject::connect(btnGenerate, &QPushButton::clicked, [&]() {
        if (excelFilePath.isEmpty()) {
            QMessageBox::warning(nullptr, "Error", "Please select an Excel file first");
            return;
        }
        generateQM(excelFilePath, outputDirPath);
    });

    window.show();
    return app.exec();
}
