import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCharts

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 900
    height: 600
    title: "MULTILANGUAGE SUPPORTING APP"

    Image {
        id: backgroundImage
        source: "qrc:/language_background.jpg"
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
                    text: cuaso.filepath1 === "" ? "Load Excel File 1" : "File 1: " + cuaso.filepath1
                    width: 200; height: 100
                    background: Rectangle { color: rootWindow.buttonColor; radius: 5 }
                    contentItem: Text {
                        text: parent.text
                        color: rootWindow.buttonTextColor
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: cuaso.loadFile1()
                }


                Button {
                    icon.source: "qrc:/icons8-upload-file-48.png"
                    text: cuaso.filepath2 === "" ? "Load Excel File 2" : "File 2: " + cuaso.filepath2
                    width: 200; height: 100
                    background: Rectangle { color: rootWindow.buttonColor; radius: 5 }
                    contentItem: Text {
                        text: parent.text
                        color: rootWindow.buttonTextColor
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: cuaso.loadFile2()
                }

                Button {
                    text: "Compare Files"
                    icon.source: "qrc:/icons8-upload-file-48.png"
                    width: 200; height: 100
                    background: Rectangle {
                            color: cuaso.filepath1 !== "" && cuaso.filepath2 !== ""
                                   ? rootWindow.secondaryColor : "#BDC3C7"
                            radius: 5
                        }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked:{

                        if(cuaso.filepath1 !== "" && cuaso.filepath2 !== "")
                        {
                            cuaso.compareSheets();
                            infoDialog.text = "Comparison completed successfully!";
                            infoDialog.open()
                        }
                        else{
                            infoDialog.open()
                        }
                    }
                }
                MessageDialog {
                        id: infoDialog
                        title: qsTr("Information")
                        text: qsTr("Not import enough file to compare")
                }

                Button {
                    text: "Export QM File"
                    width: 200; height: 100
                    background: Rectangle { color: rootWindow.buttonColor; radius: 5 }
                    contentItem: Text {
                        text: parent.text
                        color: rootWindow.buttonTextColor
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: exportDialog.open()
                }
            }

            ColumnLayout {
                id: tabArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: cuaso.totalCells > 0

                TabBar {
                    id: bar
                    width:implicitWidth

                    TabButton {
                        text: qsTr("FILE OLD")
                        font.bold: true
                        onClicked: stackLayout.currentIndex = 0
                        width: Math.max(100, implicitWidth + 20)
                    }
                    TabButton {
                        text: qsTr("FILE NEW")
                        font.bold: true
                        onClicked: stackLayout.currentIndex = 1
                        width: Math.max(100, implicitWidth + 20)
                    }
                    TabButton {
                        text: qsTr("COMPARED FILE")
                        font.bold: true
                        onClicked: stackLayout.currentIndex = 2
                        width: Math.max(100, implicitWidth + 20)
                    }
                    TabButton {
                        text: qsTr("REPORT")
                        font.bold: true
                        onClicked: stackLayout.currentIndex = 3
                        width: Math.max(100, implicitWidth + 20)
                    }
                }

                StackLayout {
                    id: stackLayout
                    width: parent.width
                    Layout.fillHeight: true
                    currentIndex: bar.currentIndex
                    Item {
                        id: tabFileOld
                        ScrollView{
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
                                    var textWidth = tableFileOld.fontMetrics.boundingRect(model.data(model.index(row, col), Qt.DisplayRole)).width();
                                    maxWidth = Math.max(maxWidth, textWidth + 20);
                                }
                                return maxWidth;
                            }


                            delegate: Rectangle {
                                height: Math.max(40, textItem_3.implicitHeight + 8)  // Đảm bảo chiều cao phù hợp
                                width: textItem_3.implicitWidth + 16
                                border.width: 1
                                border.color: "#D5D8DC"
                                color: ( model.row === 0 || model.column === 0) ? "gray" : "transparent"

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

                        }}
                    }
                    Item {
                        id: tabFileNew
                        ScrollView{
                             anchors.fill: parent
                        TableView {
                            id: tableFileNew
                            anchors.fill: parent
                            model: cuaso.file2Model
                            clip: true
                            columnSpacing: 1
                            rowSpacing: 1
                            columnWidthProvider: function(col) {
                                var maxWidth = 100;  // Độ rộng tối thiểu
                                for (var row = 0; row < model.rowCount; row++) {
                                    var textWidth = tableFileOld.fontMetrics.boundingRect(model.data(model.index(row, col), Qt.DisplayRole)).width();
                                    maxWidth = Math.max(maxWidth, textWidth + 20);
                                }
                                return maxWidth;
                            }


                            delegate: Rectangle {
                                height: Math.max(40, textItem_2.implicitHeight + 10)  // Đảm bảo chiều cao phù hợp
                                width: textItem_2.implicitWidth + 20
                                border.width: 1
                                border.color: "#D5D8DC"
                                color: ( model.row === 0 || model.column === 0) ? "gray" : "transparent"


                                Text {
                                    id: textItem_2
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
                        id: tabCompare
                        ScrollView{
                             anchors.fill: parent
                                TableView {
                                    id: tableCompare
                                    anchors.fill: parent
                                    model: cuaso.compareModel
                                    clip: true
                                    columnSpacing: 1
                                    rowSpacing: 1
                                    columnWidthProvider: function(col) {
                                        var maxWidth = 100;  // Độ rộng tối thiểu
                                        for (var row = 0; row < model.rowCount; row++) {
                                            var textWidth = tableFileOld.fontMetrics.boundingRect(model.data(model.index(row, col), Qt.DisplayRole)).width();
                                            maxWidth = Math.max(maxWidth, textWidth + 20);  // Cộng thêm padding
                                        }
                                        return maxWidth;
                                    }



                                    delegate: Rectangle {
                                        height: Math.max(40, textItem_1.implicitHeight + 8)  // Đảm bảo chiều cao phù hợp
                                        width: textItem_1.implicitWidth + 16  // Chiều rộng theo nội dung + padding
                                        border.width: 1
                                        border.color: "#D5D8DC"
                                        color: ( model.row === 0 || model.column === 0) ? "gray" : "transparent"

                                        Text {
                                            id: textItem_1
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
                                            label: qsTr("Matching components: %1").arg(cuaso.totalCells - cuaso.diffCount)
                                            value: cuaso.totalCells - cuaso.diffCount
                                            labelVisible: true
                                        }
                                        PieSlice {
                                            label: qsTr("Different components: %2").arg(cuaso.diffCount)
                                            value: cuaso.diffCount
                                            labelVisible: true
                                        }
                                    }
                                }
                            }
                            Button {
                                text: qsTr("EXPORT DIFFERENCE FILE")
                                Layout.alignment: Qt.AlignHCenter
                                onClicked: cuaso.exportDifferences()
                                background: Rectangle { color: "#adff2f"; radius: 5 }
                                contentItem: Text {
                                     text: parent.text
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
            let folderPath = exportDialog.currentFolder.toString().replace("file:///", "");
            cuaso.exportQm(folderPath)
        }
    }
}
