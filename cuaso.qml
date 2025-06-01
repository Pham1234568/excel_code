import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCharts
import QtQuick.Controls.Material

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1000; height: 1000
    title: qsTr("MULTILANGUAGE SUPPORTING APP")

    // Material theme
    Material.theme: Material.Light
    Material.accent: Material.Cyan
    // Image {
    //     id: backgroundImage
    //     source: ""
    //     anchors.fill: parent
    //     fillMode: Image.PreserveAspectCrop
    //     visible: window.totalCells === 0
    // }
    property color primaryColor: "#3498db"
    property color secondaryColor: "#2ecc71"
    property color backgroundColor: "#F4F6F7"
    property color buttonColor: "#2980b9"
    property color buttonTextColor: "white"
    property string backgroundImage: "qrc:/assets/images/background.jpg"
    Image {
        source: backgroundImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        opacity: 0.2
    }
    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 15
            Item {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.12

                Column {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    Image {
                        source: "qrc:/assets/images/icons8-google-translate-256"
                        width: 100; height: 100
                        fillMode: Image.PreserveAspectFit
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        text: qsTr("Translate by AI")
                        color: "black"
                        font.bold: true
                        font.pointSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: window.openWeb()
                }
            }



            Item {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.12

                Column {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    Image {
                        source: "qrc:/assets/images/icons8-upload-file-100"
                        width: 100; height: 100
                        fillMode: Image.PreserveAspectFit
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        text: window.filepath1 === "" ? qsTr("Load Excel File old") : qsTr("File old uploaded")
                        font.bold: true
                        font.pointSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        wrapMode: Text.WordWrap
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: window.loadFile1()
                }
            }


            Item {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.12

                Column {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    Image {
                        source: "qrc:/assets/images/icons8-upload-file-100"
                        width: 100; height: 100
                        fillMode: Image.PreserveAspectFit
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        text: window.filepath2 === "" ? qsTr("Load Excel File new") : qsTr("File new uploaded")
                        font.bold: true
                        font.pointSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        wrapMode: Text.WordWrap
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: window.loadFile2()
                }
            }


            Item {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.12

                Column {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    Image {
                        source: "qrc:/assets/images/icons8-compare-60"
                        width: 100; height: 100
                        fillMode: Image.PreserveAspectFit
                        anchors.horizontalCenter: parent.horizontalCenter
                        opacity: (window.filepath1 !== "" && window.filepath2 !== "") ? 1 : 0.4
                    }
                    Text {
                        text: qsTr("Compare Files")
                        font.bold: true
                        font.pointSize: 12
                        color: (window.filepath1 !== "" && window.filepath2 !== "") ? "black" : "#7f8c8d"
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        wrapMode: Text.WordWrap
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: (window.filepath1 !== "" && window.filepath2 !== "") ? Qt.PointingHandCursor : Qt.ArrowCursor
                    enabled: (window.filepath1 !== "" && window.filepath2 !== "")
                    onClicked: {
                        window.compareSheets()
                        infoDialog.text = "📄 ✔ " + qsTr("Comparison completed successfully!")
                        infoDialog.open()
                    }
                }



                MessageDialog {
                    id: infoDialog
                    modality: Qt.ApplicationModal
                    buttons: MessageDialog.Close
                }
            }

            Item {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.12

                Column {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    Image {
                        source: "qrc:/assets/images/export_qm"
                        width: 100; height: 100
                        fillMode: Image.PreserveAspectFit
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        text: qsTr("Export QM File")
                        font.bold: true
                        font.pointSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        wrapMode: Text.WordWrap
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: exportDialog.open()
                }
            }


            Item {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.12

                Column {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    Image {
                        source: "qrc:/assets/images/icons8-clear-100"
                        width: 100; height: 100
                        fillMode: Image.PreserveAspectFit
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Text {
                        text: qsTr("Clear")
                        font.bold: true
                        font.pointSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        wrapMode: Text.WordWrap
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: window.resetWindow()
                }
            }

        }
        Binding {
            target: tabArea; property: "visible"; value: window.totalCells > 0
        }

        ColumnLayout {
            id: tabArea
            Layout.fillWidth: true; Layout.fillHeight: true; spacing: 10

            TabBar {
                id: bar; Layout.fillWidth: true
                TabButton { text: qsTr("FILE OLD"); font.bold: true; onClicked: stackLayout.currentIndex = 0 }
                TabButton { text: qsTr("FILE NEW"); font.bold: true; onClicked: stackLayout.currentIndex = 1 }
                TabButton { text: qsTr("COMPARED FILE"); font.bold: true; onClicked: stackLayout.currentIndex = 2 }
                TabButton { text: qsTr("REPORT"); font.bold: true; onClicked: stackLayout.currentIndex = 3 }
            }
            // Stacked views for each tab
            StackLayout {
                id: stackLayout; Layout.fillWidth: true; Layout.fillHeight: true; currentIndex: bar.currentIndex
                Item {
                    ScrollView {
                        anchors.fill: parent
                        TableView {
                            id: tableFileOld; anchors.fill: parent
                            model: window.file1Model; columnSpacing: 1; rowSpacing: 1
                            property int selectedRow: -1
                            property int selectedColumn: -1
                            columnWidthProvider: function(col) {
                                var max = 100;
                                for (var r=0; r<window.file1Model.rowCount; ++r) {
                                    var w = tableFileOld.fontMetrics.boundingRect(
                                                window.file1Model.data(window.file1Model.index(r, col), Qt.DisplayRole)
                                                ).width();
                                    max = Math.max(max, w + 20);
                                }
                                return max;
                            }
                            TableView.editDelegate: TextField {
                                anchors.fill: parent
                                text: display
                                horizontalAlignment: TextInput.AlignHCenter
                                verticalAlignment: TextInput.AlignVCenter
                                Component.onCompleted: selectAll()

                                TableView.onCommit: {
                                    display = text
                                }
                            }
                            delegate: Rectangle {
                                id: cell_1
                                required property int row
                                required property int column
                                required property string display

                                implicitHeight: Math.max(40, cellText.implicitHeight + 16)
                                implicitWidth: cellText.implicitWidth + 16
                                height: implicitHeight
                                width: implicitWidth
                                border.width: 1
                                border.color: "#D5D8DC"
                                color: (row === 0 || column === 0) ? "gray" :
                                                                     (tableFileOld.selectedRow === row && tableFileOld.selectedColumn === column ? "#D6EAF8" : "transparent")

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        tableFileOld.selectedRow = row
                                        tableFileOld.selectedColumn = column
                                    }
                                }

                                Text {
                                    id: cellText
                                    anchors.fill: parent
                                    anchors.margins: 4
                                    text: cell_1.display
                                    wrapMode: Text.Wrap
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

                // --- New File Tab ---
                Item {
                    ScrollView { anchors.fill: parent
                        TableView {
                            id: tableFileNew; anchors.fill: parent
                            model: window.file2Model; columnSpacing: 1; rowSpacing: 1

                            columnWidthProvider: function(col) {
                                var max = 100;
                                for (var r=0; r<window.file2Model.rowCount; ++r) {
                                    var w = tableFileNew.fontMetrics.boundingRect(
                                                window.file2Model.data(window.file2Model.index(r, col), Qt.DisplayRole)
                                                ).width();
                                    max = Math.max(max, w + 20);
                                }
                                return max;
                            }

                            delegate: Rectangle {
                                implicitHeight: Math.max(40, cellTextNew.implicitHeight + 10)
                                implicitWidth: cellTextNew.implicitWidth + 20
                                height: implicitHeight; width: implicitWidth
                                border.width: 1; border.color: "#D5D8DC"
                                color: (row === 0 || column === 0) ? "gray" : "transparent"

                                Text {
                                    id: cellTextNew; anchors.fill: parent; anchors.margins: 4
                                    text: display
                                    wrapMode: Text.WordWrap; elide: Text.ElideRight
                                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

                // --- Compared File Tab ---
                Item {
                    ScrollView { anchors.fill: parent
                        TableView {
                            id: tableCompare; anchors.fill: parent
                            model: window.compareModel; columnSpacing: 1; rowSpacing: 1

                            columnWidthProvider: function(col) {
                                var max = 100;
                                for (var r=0; r<window.compareModel.rowCount; ++r) {
                                    var w = tableCompare.fontMetrics.boundingRect(
                                                window.compareModel.data(window.compareModel.index(r, col), Qt.DisplayRole)
                                                ).width();
                                    max = Math.max(max, w + 20);
                                }
                                return max;
                            }

                            delegate: Rectangle {
                                implicitHeight: Math.max(40, cellTextComp.implicitHeight + 16)
                                implicitWidth: cellTextComp.implicitWidth + 16
                                height: implicitHeight; width: implicitWidth
                                border.width: 1; border.color: "#D5D8DC"
                                color: (row === 0 || column === 0) ? "gray" : "transparent"

                                Text {
                                    id: cellTextComp; anchors.fill: parent; anchors.margins: 4
                                    text: display
                                    wrapMode: Text.WordWrap; elide: Text.ElideRight
                                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

                // --- Report Tab ---
                Item {
                    ColumnLayout { anchors.fill: parent; spacing: 10
                        RowLayout { Layout.fillWidth: true
                            ChartView {
                                id: chartView
                                title: qsTr("RATIO OF MATCHING AND DIFFERENT COMPONENTS")
                                titleFont.family: "Nunito"; titleFont.bold: true; antialiasing: true
                                Layout.fillWidth: true; Layout.fillHeight: true
                                PieSeries {
                                    PieSlice { label: qsTr("Matching: %1").arg(window.totalCells - window.diffCount); value: window.totalCells - window.diffCount; labelVisible: true }
                                    PieSlice { label: qsTr("Different: %1").arg(window.diffCount); value: window.diffCount; labelVisible: true }
                                }
                            }
                        }
                        Button {
                            text: qsTr("EXPORT DIFFERENCE FILE")
                            font.bold: true; font.pointSize: 12; Layout.alignment: Qt.AlignHCenter
                            onClicked: window.exportDifferences()
                            background: Rectangle { color: "#adff2f"; radius: 5 }
                        }
                    }
                }
            }
        }
    }

    // Folder dialog for exporting QM
    FolderDialog {
        id: exportDialog; title: qsTr("Select Export Directory")
        onAccepted: {
            var folder = exportDialog.currentFolder.toString().replace("file:///", "/");
            window.exportQm(folder);
        }
    }
}
