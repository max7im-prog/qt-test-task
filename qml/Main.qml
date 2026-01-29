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
                            to: 60000
                            stepSize: 100
                            value: AppController.queryIntervalMs
                            onValueChanged: AppController.queryIntervalMs = value
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
                                    text: "Pass",
                                    value: FileScheduler.Pass
                                },
                                {
                                    text: "Overwrite",
                                    value: FileScheduler.Overwrite
                                },
                                {
                                    text: "Copy",
                                    value: FileScheduler.Copy
                                }
                            ]

                            onActivated: AppController.repeatAction = currentValue
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

            /* Task list takes all remaining vertical space */
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: AppController.tasks

                delegate: Rectangle {
                    width: ListView.view.width
                    height: 36
                    color: index % 2 === 0 ? "#00000000" : "#08000000"

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 6
                        spacing: 8

                        Label {
                            text: name
                            width: 220
                            elide: Text.ElideMiddle
                        }

                        ProgressBar {
                            Layout.fillWidth: true
                            from: 0
                            to: 100
                            value: progress
                        }

                        Label {
                            text: status
                            width: 80
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }

            /* Logs area with fixed height */
            Label {
                text: "Logs"
                font.bold: true
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.preferredHeight: 160  // fixed log height

                TextArea {
                    readOnly: true
                    wrapMode: Text.Wrap
                    text: AppController.log
                }
            }
        }
    }
}
