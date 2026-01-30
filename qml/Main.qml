import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import custom.AppController 1.0
import custom.FileScheduler 1.0

ApplicationWindow {
    id: root
    width: 900
    height: 600
    visible: true
    title: "File Modifier"

    RowLayout {
        anchors.fill: parent
        spacing: 16

        /* =========================
         * Left panel – configuration
         * ========================= */
        ColumnLayout {
            Layout.preferredWidth: 320
            Layout.maximumWidth: 320
            Layout.fillHeight: true
            spacing: 10

            GroupBox {
                title: "Directories"
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: 8

                    RowLayout {
                        Label {
                            text: "Input dir:"
                            width: 110
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "./input"
                            text: AppController.inputDir
                            onTextChanged: AppController.inputDir = text
                            enabled: !AppController.running
                        }
                    }

                    RowLayout {
                        Label {
                            text: "Output dir:"
                            width: 110
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "./output"
                            text: AppController.outputDir
                            onTextChanged: AppController.outputDir = text
                            enabled: !AppController.running
                        }
                    }
                }
            }

            GroupBox {
                title: "Processing"
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: 8

                    RowLayout {
                        Label {
                            text: "File mask:"
                            width: 110
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "*.bin;*.txt"
                            text: AppController.fileMask
                            onTextChanged: AppController.fileMask = text
                            enabled: !AppController.running
                        }
                    }

                    RowLayout {
                        Label {
                            text: "Byte mask:"
                            width: 110
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "0011223344556677"
                            text: AppController.byteMask
                            onTextChanged: AppController.byteMask = text
                            enabled: !AppController.running
                        }
                    }

                    RowLayout {
                        Label {
                            text: "Interval (ms):"
                            width: 110
                        }
                        SpinBox {
                            Layout.fillWidth: true
                            from: 100
                            to: 9999999
                            stepSize: 100
                            editable: true
                            value: AppController.queryIntervalMs
                            onValueChanged: AppController.queryIntervalMs = value
                            enabled: !AppController.running
                        }
                    }
                }
            }

            GroupBox {
                title: "Options"
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: 6

                    RowLayout {
                        Label {
                            text: "Single shot:"
                            Layout.fillWidth: true
                        }
                        Switch {
                            checked: AppController.singleShot
                            onToggled: AppController.singleShot = checked
                            enabled: !AppController.running
                        }
                    }

                    RowLayout {
                        Label {
                            text: "Delete on modify:"
                            Layout.fillWidth: true
                        }
                        Switch {
                            checked: AppController.deleteOnModify
                            onToggled: AppController.deleteOnModify = checked
                            enabled: !AppController.running
                        }
                    }

                    RowLayout {
                        Label {
                            text: "Repeat policy:"
                            width: 110
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            textRole: "text"
                            valueRole: "value"
                            model: [
                                {
                                    text: "Copy",
                                    value: FileScheduler.Copy
                                },
                                {
                                    text: "Pass",
                                    value: FileScheduler.Pass
                                },
                                {
                                    text: "Overwrite",
                                    value: FileScheduler.Overwrite
                                }
                            ]
                            onActivated: AppController.repeatAction = currentValue
                            enabled: !AppController.running
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

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
                Layout.fillWidth: true
                text: AppController.status
                wrapMode: Text.Wrap
                color: "#555"
            }
        }

        /* =========================
         * Right panel – tasks & logs
         * ========================= */
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            Label {
                text: "Tasks"
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: AppController.tasks

                delegate: RowLayout {
                    width: ListView.view.width
                    height: 36
                    spacing: 8

                    Label {
                        text: name
                        Layout.preferredWidth: 200
                        Layout.maximumWidth: 300
                        elide: Text.ElideMiddle
                        horizontalAlignment: Text.AlignLeft
                    }

                    ProgressBar {
                        Layout.fillWidth: true
                        from: 0
                        to: 100
                        value: progress
                    }

                    Label {
                        text: status
                        Layout.preferredWidth: 80
                        Layout.minimumWidth: 80
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }

            Label {
                text: "Logs"
                font.bold: true
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.preferredHeight: 160

                TextArea {
                    readOnly: true
                    wrapMode: Text.Wrap
                    text: AppController.log
                    onTextChanged: cursorPosition = length
                }
            }
        }
    }
}
