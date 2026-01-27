#include "fileScheduler.hpp"
#include <QCoreApplication>
#include <qdebug.h>

int main(int argc, char *argv[]) {

  QCoreApplication app(argc, argv);

  FileScheduler::Task task;
  task._run = true;
  task._fromDirectory = "./input";
  task._toDirectory = "./output";
  task._inputFileMask = "*.bin;*.txt";
  task._fileRepeatAction = FileScheduler::Task::FileRepeatAction::Copy;
  task._queryInterval = std::chrono::milliseconds(1000);
  task._singleShot = false;
  task._byteMask = QByteArray::fromHex("0102030405060708");

  FileScheduler scheduler{task};

  QObject::connect(&scheduler, &FileScheduler::showUserInfo,
                   [](const QString &info) { qInfo().noquote() << info; });

  return app.exec();
}
