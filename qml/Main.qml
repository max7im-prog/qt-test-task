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
                placeholderText: "Input directory"
                Layout.fillWidth: true
                text: AppController.inputDir
                onTextChanged: AppController.inputDir = text
            }

            TextField {
                placeholderText: "Output directory"
                Layout.fillWidth: true
            }

            TextField {
                placeholderText: "File mask (e.g. *.bin;*.txt)"
                Layout.fillWidth: true
            }

            TextField {
                placeholderText: "XOR key (8 bytes hex, e.g. 0011223344556677)"
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "Start"
                    Layout.fillWidth: true
                    // onClicked: scheduler.start()
                    onClicked: AppController.start()
                }

                Button {
                    text: "Stop"
                    Layout.fillWidth: true
                    // onClicked: scheduler.stop()
                }
            }

            Label {
                // text: scheduler.status
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }

            Item {
                Layout.fillHeight: true
            } // spacer
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: "Log"
            }

            TextArea {
                id: logArea
                readOnly: true
                wrapMode: Text.Wrap
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
