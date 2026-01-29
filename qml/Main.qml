import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import custom.AppController 1.0
import custom.FileScheduler 1.0
import custom.TaskModel 1.0

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

            RowLayout {
                Text {
                    text: "Input dir"
                }
                TextField {
                    Layout.fillWidth: true
                    placeholderText: "(e.g ./input)"
                    text: AppController.inputDir
                    onTextChanged: AppController.inputDir = text
                }
            }

            RowLayout {
                Text {
                    text: "Output dir:"
                }
                TextField {
                    Layout.fillWidth: true
                    placeholderText: "(e.g ./input)"
                    text: AppController.outputDir
                    onTextChanged: AppController.outputDir = text
                }
            }

            RowLayout {
                Text {
                    text: "File mask:"
                }
                TextField {
                    placeholderText: "(e.g. *.bin;*.txt)"
                    Layout.fillWidth: true
                    text: AppController.fileMask
                    onTextChanged: AppController.fileMask = text
                }
            }

            RowLayout {
                Text {
                    text: "Byte mask:"
                }
                TextField {
                    placeholderText: "XOR key (8 bytes hex, e.g. 0011223344556677)"
                    Layout.fillWidth: true
                    text: AppController.byteMask
                    onTextChanged: AppController.byteMask = text
                }
            }

            RowLayout {
                Text {
                    text: "Query interval, ms:"
                }
                TextField {
                    placeholderText: "(e.g. 1000)"
                    Layout.fillWidth: true
                    text: AppController.queryIntervalMs.toString()
                    onTextChanged: AppController.queryIntervalMs = parseInt(text)
                }
            }

            RowLayout {
                Text {
                    text: "Single shot:"
                }
                Switch {
                    checked: AppController.singleShot
                    onCheckedChanged: AppController.singleShot = checked
                }
            }
            RowLayout {
                Text {
                    text: "Delete on modify:"
                }
                Switch {
                    checked: AppController.deleteOnModify
                    onCheckedChanged: AppController.deleteOnModify = checked
                }
            }
            RowLayout {
                Text {
                    text: "File repeat policy:"
                }
                ComboBox {
                    model: [
                        {
                            value: FileScheduler.Pass,
                            text: qsTr("Pass")
                        },
                        {
                            value: FileScheduler.Overwrite,
                            text: qsTr("Overwrite")
                        },
                        {
                            value: FileScheduler.Copy,
                            text: qsTr("Copy")
                        }
                    ]

                    textRole: "text"
                    valueRole: "value"

                    onActivated: AppController.repeatAction = currentValue
                }
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
            ListView {
                model: AppController.tasks
                delegate: RowLayout {
                    Label {
                        text: name
                        width: 160
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: progress
                    }
                    Label {
                        text: status
                    }
                }
            }
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
                    onTextChanged: cursorPosition = length
                }
            }
        }
    }
}
