import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCharts

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1000
    height: 800
    title: "MULTILANGUAGE SUPPORTING APP"

    Image {
        id: backgroundImage
        source: ""
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: cuaso.totalCells === 0
    }

    property color primaryColor: "#3498db"
    property color secondaryColor: "#2ecc71"
    property color backgroundColor: "#F4F6F7"
    property color buttonColor: "#2980b9"
    property color buttonTextColor: "white"

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 15

            Button {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.05
                highlighted:true

                background: Rectangle {
                    color: rootWindow.buttonColor
                    radius: 5
                }
                contentItem: Text {
                    text: cuaso.filepath1 === "" ? "Load Excel File 1" : "File 1: " + cuaso.filepath1
                    anchors.fill: parent
                    clip: true
                    elide: Text.ElideRight
                    wrapMode: Text.NoWrap
                    maximumLineCount: 1

                    color: rootWindow.buttonTextColor
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: cuaso.loadFile1()
            }
            Button {
                icon.source: "qrc:/icons8-upload-file-48.png"
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.05

                background: Rectangle {
                    color: rootWindow.buttonColor
                    radius: 5
                }
                contentItem: Text {
                    text:cuaso.filepath2 === "" ? "Load Excel File 2" : "File 2: " + cuaso.filepath2
                    anchors.fill: parent
                    clip: true
                    elide: Text.ElideRight
                    wrapMode: Text.NoWrap
                    maximumLineCount: 1
                    color: rootWindow.buttonTextColor
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: cuaso.loadFile2()
            }

            Button {
                icon.source: "qrc:/icons8-upload-file-48.png"

                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.05
                background: Rectangle {
                    color: cuaso.filepath1 !== "" && cuaso.filepath2 !== ""
                           ? rootWindow.secondaryColor : "#BDC3C7"
                    radius: 5
                }
                contentItem: Text {
                    text: "Compare Files"
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    wrapMode: Text.SingleLine
                }
                onClicked: {
                    if (cuaso.filepath1 !== "" && cuaso.filepath2 !== "") {
                        cuaso.compareSheets();
                        infoDialog.text = "📄 ✔ Comparison completed successfully!";
                        infoDialog.open();
                    } else {
                        infoDialog.text="⚠ WARNING: NOT ENOUGH FILE TO COMPARE "
                        infoDialog.open();
                    }
                }
            }
            MessageDialog {
                id: infoDialog
                buttons: MessageDialog.Close
            }

            Button {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.05

                background: Rectangle {
                    color: rootWindow.buttonColor
                    radius: 5
                }
                contentItem: Text {
                     text: "Export QM File"
                    color: rootWindow.buttonTextColor
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    wrapMode: Text.SingleLine
                }
                onClicked: exportDialog.open()
            }
            Button {
                Layout.preferredWidth: rootWindow.width * 0.15
                Layout.preferredHeight: rootWindow.height * 0.05
                highlighted:true

                background: Rectangle {
                    color: rootWindow.buttonColor
                    radius: 5
                }
                contentItem: Text {
                    text: "Clear"
                    anchors.fill: parent
                    clip: true
                    elide: Text.ElideRight
                    wrapMode: Text.NoWrap
                    maximumLineCount: 1

                    color: rootWindow.buttonTextColor
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    cuaso.resetWindow();
                    tabArea.visible=false;

                }
            }
        }

        ColumnLayout {
            id: tabArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            visible: cuaso.totalCells > 0

            TabBar {
                id: bar
                Layout.fillWidth: true

                TabButton {
                    text: qsTr("FILE OLD")
                    font.bold: true
                    onClicked: stackLayout.currentIndex = 0
                }
                TabButton {
                    text: qsTr("FILE NEW")
                    font.bold: true
                    onClicked: stackLayout.currentIndex = 1
                }
                TabButton {
                    text: qsTr("COMPARED FILE")
                    font.bold: true
                    onClicked: stackLayout.currentIndex = 2
                }
                TabButton {
                    text: qsTr("REPORT")
                    font.bold: true
                    onClicked: stackLayout.currentIndex = 3
                }
            }

            StackLayout {
                id: stackLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: bar.currentIndex


                Item {
                    id: tabFileOld
                    ScrollView {
                        anchors.fill: parent
                        TableView {
                            id: tableFileOld
                            anchors.fill: parent
                            model: cuaso.file1Model
                            columnSpacing: 1
                            rowSpacing: 1
                            clip: true


                            columnWidthProvider: function(col) {
                                var maxWidth = 100;
                                for (var row = 0; row < model.rowCount; row++) {
                                    var textWidth = tableFileOld.fontMetrics.boundingRect(
                                        model.data(model.index(row, col), Qt.DisplayRole)
                                    ).width();
                                    maxWidth = Math.max(maxWidth, textWidth + 20);
                                }
                                return maxWidth;
                            }

                            delegate: Rectangle {
                                height: Math.max(40, textItem_3.implicitHeight + 8)
                                width: textItem_3.implicitWidth + 16
                                border.width: 1
                                border.color: "#D5D8DC"
                                color: (model.row === 0 || model.column === 0)
                                       ? "gray"
                                       : "transparent"

                                Text {
                                    id: textItem_3
                                    anchors.fill: parent
                                    anchors.margins: 4
                                    text: model.display
                                    wrapMode: Text.Wrap
                                    elide: Text.ElideNone
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

                Item {
                    id: tabFileNew
                    ScrollView {
                        anchors.fill: parent
                        TableView {
                            id: tableFileNew
                            anchors.fill: parent
                            model: cuaso.file2Model
                            clip: true
                            columnSpacing: 1
                            rowSpacing: 1

                            columnWidthProvider: function(col) {
                                var maxWidth = 100;
                                for (var row = 0; row < model.rowCount; row++) {
                                    var textWidth = tableFileNew.fontMetrics.boundingRect(
                                        model.data(model.index(row, col), Qt.DisplayRole)
                                    ).width();
                                    maxWidth = Math.max(maxWidth, textWidth + 20);
                                }
                                return maxWidth;
                            }

                            delegate: Rectangle {
                                height: Math.max(40, textItem_2.implicitHeight + 10)
                                width: textItem_2.implicitWidth + 20
                                border.width: 1
                                border.color: "#D5D8DC"
                                color: (model.row === 0 || model.column === 0)
                                       ? "gray"
                                       : "transparent"

                                Text {
                                    id: textItem_2
                                    anchors.fill: parent
                                    anchors.margins: 4
                                    text: model.display
                                    wrapMode: Text.WordWrap
                                    elide: Text.ElideRight
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

                Item {
                    id: tabCompare
                    ScrollView {
                        anchors.fill: parent
                        TableView {
                            id: tableCompare
                            anchors.fill: parent
                            model: cuaso.compareModel
                            clip: true
                            columnSpacing: 1
                            rowSpacing: 1

                            columnWidthProvider: function(col) {
                                var maxWidth = 100;
                                for (var row = 0; row < model.rowCount; row++) {
                                    var textWidth = tableCompare.fontMetrics.boundingRect(
                                        model.data(model.index(row, col), Qt.DisplayRole)
                                    ).width();
                                    maxWidth = Math.max(maxWidth, textWidth + 20);
                                }
                                return maxWidth;
                            }

                            delegate: Rectangle {
                                height: Math.max(40, textItem_1.implicitHeight + 8)
                                width: textItem_1.implicitWidth + 16
                                border.width: 1
                                border.color: "#D5D8DC"
                                color: (model.row === 0 || model.column === 0)
                                       ? "gray"
                                       : "transparent"

                                Text {
                                    id: textItem_1
                                    anchors.fill: parent
                                    anchors.margins: 4
                                    text: model.display
                                    wrapMode: Text.WordWrap
                                    elide: Text.ElideRight
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

                // Tab 3: Report
                Item {
                    id: tabActivity
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            ChartView {
                                id: chartView
                                title: qsTr("RATIO OF MATCHING AND DIFFERENT COMPONENTS ")
                                titleFont.family: "Nunito"
                                titleFont.bold: true
                                antialiasing: true
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                PieSeries {
                                    PieSlice {
                                        label: qsTr("Matching components: %1")
                                               .arg(cuaso.totalCells - cuaso.diffCount)
                                        value: cuaso.totalCells - cuaso.diffCount
                                        labelVisible: true
                                    }
                                    PieSlice {
                                        label: qsTr("Different components: %1")
                                               .arg(cuaso.diffCount)
                                        value: cuaso.diffCount
                                        labelVisible: true
                                    }
                                }
                            }
                        }

                        Button {
                            Layout.alignment: Qt.AlignHCenter
                            onClicked: cuaso.exportDifferences()
                            background: Rectangle { color: "#adff2f"; radius: 5 }
                            contentItem: Text {
                                text: qsTr("EXPORT DIFFERENCE FILE")
                                color: "#000000"
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                anchors.fill: parent
                                anchors.margins: 4
                            }
                        }
                    }
                }
            }
        }
    }

    FolderDialog {
        id: exportDialog
        title: "Select Export Directory"
        onAccepted: {
            var folderPath = exportDialog.currentFolder.toString().replace("file:///", "");
            cuaso.exportQm(folderPath)
        }
    }
}
