import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import custom.AppController

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true
    title: "File Modifier"

    GridLayout {
        anchors.fill: parent
        columns: 2
        columnSpacing: 12
        rowSpacing: 12
        anchors.margins: 12

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            TextField {
                Layout.fillWidth: true
                placeholderText: "Input directory"
                text: AppController.inputDir
                onTextChanged: AppController.inputDir = text
            }

            TextField {
                Layout.fillWidth: true
                placeholderText: "Output directory"
                text: AppController.outputDir
                onTextChanged: AppController.outputDir = text
            }

            TextField {
                placeholderText: "File mask (e.g. *.bin;*.txt)"
                Layout.fillWidth: true
                text: AppController.fileMask
                onTextChanged: AppController.fileMask = text
            }

            TextField {
                placeholderText: "XOR key (8 bytes hex, e.g. 0011223344556677)"
                Layout.fillWidth: true
                // text: AppController.byteMask
                onTextChanged: AppController.byteMask = text
            }

            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "Start"
                    Layout.fillWidth: true
                    onClicked: AppController.start()
                }

                Button {
                    text: "Stop"
                    Layout.fillWidth: true
                    onClicked: AppController.stop()
                }
            }

            Label {
                text: AppController.status
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }

            Item {
                Layout.fillHeight: true
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: "Logs"
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                TextArea {
                    id: logArea
                    readOnly: true
                    wrapMode: Text.Wrap
                    text: AppController.log
                }
            }
        }
    }
}
